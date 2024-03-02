/*
 * Copyright 2022 - 2024 Rafał Jopek ( rafaljopek at hotmail com )
 */

#include "gtglfw.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// static
static void cursor_position_callback( GLFWwindow *window, double x, double y )
{
   App *pApp = glfwGetWindowUserPointer( window );

   pApp->cursorX = x;
   pApp->cursorY = y;
}

static void scroll_callback( GLFWwindow *window, double xoffset, double yoffset )
{
   App *pApp = glfwGetWindowUserPointer( window );

   pApp->scrollXOffset = xoffset;
   pApp->scrollYOffset = yoffset;
}

static void key_callback( GLFWwindow *window, int key, int scancode, int action, int mods )
{
   App *pApp = glfwGetWindowUserPointer( window );

   if( action != GLFW_PRESS )
   {
      return;
   }

   pApp->keyCode     = key;
   pApp->keyScancode = scancode;
   pApp->keyAction   = action;
   pApp->keyMods     = mods;
}

static void char_callback( GLFWwindow *window, unsigned int codepoint )
{
   App *pApp = glfwGetWindowUserPointer( window );

   char string[ 5 ] = "";
   gt_utf8_encode( string, codepoint );

   strcpy( pApp->keyChar, string );
}

static void mouse_button_callback( GLFWwindow *window, int button, int action, int mods )
{
   App *pApp = glfwGetWindowUserPointer( window );

   pApp->mouseButton = button;
   pApp->mouseAction = action;
   pApp->mouseMods   = mods;
}

static void window_maximize_callback( GLFWwindow *window, int maximized )
{
   App *pApp = glfwGetWindowUserPointer( window );

   pApp->winMaximized = maximized;
}

static void window_size_callback( GLFWwindow *window, int width, int height )
{
   App *pApp = glfwGetWindowUserPointer( window );

   pApp->width  = width;
   pApp->height = height;
}

static void set_clear_color_from_hex( unsigned int hexColor )
{
   if( ( hexColor <= 0xffffff ) )
   {
      float r, g, b;
      r = ( ( hexColor >> 16 ) & 0xFF ) / 255.0;
      g = ( ( hexColor >> 8 )  & 0xFF ) / 255.0;
      b = (   hexColor         & 0xFF ) / 255.0;
      glClearColor( r, g, b, 1.0 );
   }
   else if( ( hexColor <= 0xffffffff ) )
   {
      float r, g, b, a;
      r = ( ( hexColor >> 24 ) & 0xFF ) / 255.0;
      g = ( ( hexColor >> 16 ) & 0xFF ) / 255.0;
      b = ( ( hexColor >> 8 )  & 0xFF ) / 255.0;
      a = (   hexColor         & 0xFF ) / 255.0;
      glClearColor( r, g, b, a );
   }
   else
   {
      fprintf( stderr, "An invalid hexadecimal color value was passed." );
   }
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// internal
void gt_check_open_gl_error( const char *stmt, const char *fname, int line, GLenum *errCode )
{
   GLenum err = glGetError();
   if( err != GL_NO_ERROR )
   {
      printf( "OpenGL error %08x, at %s:%i - for %s\n", err, fname, line, stmt );
      *errCode = err;
   }
   else
   {
     *errCode = GL_NO_ERROR;
   }
}

void gt_set_color_from_hex( unsigned int hexColor )
{
   if( ( hexColor <= 0xffffff ) )
   {
      float r, g, b;
      r = ( ( hexColor >> 16 ) & 0xFF ) / 255.0f;
      g = ( ( hexColor >> 8 )  & 0xFF ) / 255.0f;
      b = (   hexColor         & 0xFF ) / 255.0f;
      glColor3f( r, g, b );
   }
   else if( ( hexColor <= 0xffffffff ) )
   {
      float r, g, b, a;
      r = ( ( hexColor >> 24 ) & 0xFF ) / 255.0f;
      g = ( ( hexColor >> 16 ) & 0xFF ) / 255.0f;
      b = ( ( hexColor >> 8 )  & 0xFF ) / 255.0f;
      a = (   hexColor         & 0xFF ) / 255.0f;
      glColor4f( r, g, b, a );
   }
   else
   {
      fprintf( stderr, "An invalid hexadecimal color value was passed." );
   }
}

unsigned int gt_convert_hex_to_int( const char *hex )
{
   return ( unsigned int ) strtol( hex, NULL, 16 );
}

char *gt_strdup( const char *str )
{
   int len = strlen( str ) + 1;
   char *new_str = malloc( len );
   if( new_str )
   {
      strcpy( new_str, str );
   }
   return new_str;
}

const char *gt_utf8_offset_to_pointer( const char *str, int character_offset )
{
   const char *s = str;
   mbstate_t state = { 0 };

   while( character_offset > 0 && *s )
   {
      wchar_t wc;
      size_t bytes = mbrtowc( &wc, s, MB_CUR_MAX, &state );
      if( bytes == ( size_t ) -1 || bytes == ( size_t ) -2 )
      {
         // Invalid or incomplete character, skip it
         s++;
      }
      else
      {
         character_offset--;
         s += bytes;
      }
   }

   return s;
}

size_t gt_utf8_encode( char *str, unsigned int code_point )
{
   size_t count = 0;

   if( code_point < 0x80 )
      str[ count++ ] = ( char )code_point;
   else if( code_point < 0x800 )
   {
      str[ count++ ] = ( code_point >> 6 ) | 0xc0;
      str[ count++ ] = ( code_point & 0x3f ) | 0x80;
   }
   else if( code_point < 0x10000 )
   {
      str[ count++ ] = ( code_point >> 12 ) | 0xe0;
      str[ count++ ] = ( ( code_point >> 6 ) & 0x3f ) | 0x80;
      str[ count++ ] = ( code_point & 0x3f ) | 0x80;
   }
   else if( code_point < 0x110000 )
   {
      str[ count++ ] = ( code_point >> 18 ) | 0xf0;
      str[ count++ ] = ( ( code_point >> 12 ) & 0x3f ) | 0x80;
      str[ count++ ] = ( ( code_point >> 6 ) & 0x3f ) | 0x80;
      str[ count++ ] = ( code_point & 0x3f ) | 0x80;
   }

   return count;
}

size_t gt_utf8_strlen_single_byte( const char *str )
{
   size_t len = 0;
   unsigned char c;

   while( ( c = ( unsigned char ) *str++ ) )
   {
      if( ( c & 0xc0 ) != 0x80 ) len++;
   }

   return len;
}

size_t gt_utf8_strlen_multibyte( const char *str )
{
   size_t len = 0;
   mbstate_t state = { 0 };

   while( *str )
   {
      wchar_t wc;
      size_t bytes = mbrtowc( &wc, str, MB_CUR_MAX, &state );
      if( bytes == ( size_t ) -1 || bytes == ( size_t ) -2 )
      {
         // Invalid or incomplete character, skip it
         str++;
      }
      else
      {
         len++;
         str += bytes;
      }
   }

   return len;
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// API functions
App *gtCreateWindow( int width, int height, const char *title )
{
   setlocale( LC_ALL, "" );

   if( ! glfwInit() )
   {
      exit( EXIT_FAILURE );
   }

   App *pApp = malloc( sizeof( App ) );
   if( ! pApp )
   {
      fprintf( stderr, "Memory allocation failed for App structure.\n" );
      return NULL;
   }

   memset( pApp, 0, sizeof( App ) );

   pApp->width  = width;
   pApp->height = height;
   pApp->title  = title;
   pApp->background = APP_BACKGROUND;

   pApp->window = glfwCreateWindow( pApp->width, pApp->height, pApp->title, NULL, NULL );
   if( ! pApp->window )
   {
      glfwTerminate();
      return NULL;
   }

   glfwMakeContextCurrent( pApp->window );
   glfwSetWindowUserPointer( pApp->window, pApp );
   glfwSwapInterval( 1 );

   glfwSetCursorPosCallback( pApp->window, cursor_position_callback );
   glfwSetScrollCallback( pApp->window, scroll_callback );
   glfwSetKeyCallback( pApp->window, key_callback );
   glfwSetCharCallback( pApp->window, char_callback );
   glfwSetMouseButtonCallback( pApp->window, mouse_button_callback );
   glfwSetWindowMaximizeCallback( pApp->window, window_maximize_callback );
   glfwSetWindowSizeCallback( pApp->window, window_size_callback );

   glfwSetWindowSizeLimits( pApp->window, pApp->width, pApp->height, GLFW_DONT_CARE , GLFW_DONT_CARE );
   glfwSetCursorPos( pApp->window, pApp->width / 2, pApp->height / 2 );

   return pApp;
}

bool gtMainLoop( App *pApp )
{
   bool state;

   pApp->closeFlag = glfwWindowShouldClose( pApp->window );
   glfwSetWindowShouldClose( pApp->window, GLFW_FALSE );

   state = glfwGetWindowAttrib( pApp->window, GLFW_ICONIFIED );
   if( state == T )
   {
      while( state == T )
      {
         state = glfwGetWindowAttrib( pApp->window, GLFW_ICONIFIED );

         glfwWaitEventsTimeout( 0.5 );
      }
   }
   return pApp->closeFlag;
}

void gtExitApp( App *pApp )
{
   if( pApp )
   {
      glfwSetWindowShouldClose( pApp->window, GLFW_TRUE );
   }
}

void gtBeginDrawing( App *pApp )
{
   glViewport( 0, 0, pApp->width, pApp->height );
   set_clear_color_from_hex( pApp->background );
   glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

   glDisable( GL_CULL_FACE );
   glDisable( GL_DEPTH_TEST );
   glEnable( GL_BLEND );
   glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

   glMatrixMode( GL_PROJECTION );
   glLoadIdentity();
   glOrtho( 0, pApp->width, pApp->height, 0, -1, 1 );
   glMatrixMode( GL_MODELVIEW );
   glLoadIdentity();
}

void gtEndDrawing( const App *pApp )
{
   // REPORT_OPENGL_ERROR( "End drawing: " );
   glfwSwapBuffers( pApp->window );
}

void gtBackground( App *pApp, unsigned int color )
{
   pApp->background = color;
}

int gtCloseAll( App *pApp )
{
   if( pApp )
   {
      glfwDestroyWindow( pApp->window );
      free( pApp );
      glfwTerminate();
      exit( EXIT_SUCCESS );
   }
   else
   {
      fprintf( stderr, "Error: CloseWindow called with NULL App pointer.\n" );
      exit( EXIT_FAILURE );
   }
}

void gtPollEvents()
{
   glfwPollEvents();
}

void gtWaitEvents()
{
   glfwWaitEvents();
}

void gtWaitEventsTimeout( double timeout )
{
   glfwWaitEventsTimeout( timeout );
}

void gtSetTargetFPS( App *pApp, int targetFPS )
{
   const double targetFrameTime = 1.0 / targetFPS;

   double currentTime = glfwGetTime();
   double frameTime = currentTime - pApp->previousTime;

   if( frameTime < targetFrameTime )
   {
      double sleepTime = targetFrameTime - frameTime;
      glfwWaitEventsTimeout( sleepTime );
   }

   pApp->previousTime = currentTime;
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
