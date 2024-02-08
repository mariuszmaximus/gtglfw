#ifndef GTGLFW_H_
#define GTGLFW_H_

#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <wchar.h>

#if defined( _WIN32 ) || defined( _WIN64 )
   #include <direct.h>
   #include <windows.h>
   #define gt_GetCurrentDir _getcwd
#else
   #include <dirent.h>
   #include <sys/stat.h>
   #include <unistd.h>
   #define gt_GetCurrentDir getcwd
   #define _POSIX_C_SOURCE 200809L
#endif

#include <GLFW/glfw3.h>

#define DEFAULT_COLORS_GTGLFW

#ifdef DEFAULT_COLORS_GTGLFW
   #include "default_colors_gtglfw.h"
#endif

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */

#define BDF_VERSION_MAJOR   0
#define BDF_VERSION_MINOR   0
#define BDF_VERSION_RELEASE 0
#define BDF_VERSION_STATUS  "dev"
#define BDF_DESCRIPTION     "Text environment based on GLFW"
#define BDF_COPYRIGHT       "Copyright (c) 2022 - 2024 Harbour development, https://github.com/dev-harbour/gtglfw"
#define BDF_LICENSE         "MIT License"

#define ENCODING            65534
#define BITMAP_WIDTH        9
#define BITMAP_HEIGHT       18
extern unsigned int fontData[ ENCODING ][ BITMAP_HEIGHT ];

#define MENU_BAR_MAX          15
#define MENU_BAR_HEIGHT       24
#define MENU_BAR_TEXT_MARGIN  10

#define MENU_ITEM_MAX         25
#define MENU_ITEM_HEIGHT      24
#define MENU_ITEM_TEXT_MARGIN 24

typedef enum bool
{
   F = 0,
   T = ( ! 0 )
} bool;

typedef struct _App      App;
typedef struct _Button   Button;
typedef struct _MenuItem MenuItem;
typedef struct _MenuBar  MenuBar;
typedef struct _Menu     Menu;
typedef struct _FileInfo FileInfo;

struct _Button
{
   App         *pApp;
   int          x;
   int          y;
   int          width;
   int          height;
   int          textMargin;
   bool         state;
   bool         mouseOver;
   bool         isClicked;
   unsigned int buttonID;
   unsigned int color;
   void         ( *onClick )();
};

struct _MenuItem
{
   Menu       *pMenu;
   int         x;
   int         y;
   int         width;
   int         height;
   const char *selectTitle;
   int         textMargin;
   bool        mouseOver;
   bool        isClicked;
   int         shortcutKey;
   char        shortcutName[ 64 ];
   void        ( *onClick )( MenuItem *pMenuItem );
};

struct _MenuBar
{
   Menu       *pMenu;
   int         x;
   int         y;
   int         width;
   int         height;
   const char *title;
   int         textMargin;
   bool        mouseOver;
   bool        isClicked;
   MenuItem   *pMenuItems[ MENU_ITEM_MAX ];
   int         iMenuItemsCount;
   int         iMenuItemsWidth;
   int         iMenuItemsWidthShortcut;
   bool        bMenuItemDisplayed;
};

struct _Menu
{
   App     *pApp;
   MenuBar *pMenuBars[ MENU_BAR_MAX ];
   int      iMenuBarsCount;
   bool     isVisible;
};

struct _FileInfo
{
   char name[ 512 ];
   char size[ 20 ];
   char date[ 11 ];
   char time[ 9 ];
   char attr[ 6 ];
   bool state;
};

struct _App
{
   GLFWwindow  *window;
   int          width;
   int          height;
   const char  *title;
   bool         closeFlag;
   double       cursorX;
   double       cursorY;
   double       scrollXOffset;
   double       scrollYOffset;
   int          keyCode;
   int          keyScancode;
   int          keyAction;
   int          keyMods;
   char         keyChar[ 5 ];
   int          mouseButton;
   int          mouseAction;
   int          mouseMods;
   int          winMaximized;
   unsigned int background;
   double       previousTime;
};

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// API functions
App  *gtCreateWindow( int width, int height, const char *title );
bool  gtMainLoop( App *pApp );
void  gtExitApp( App *pApp );
void  gtBeginDrawing( App *pApp );
void  gtEndDrawing( const App *pApp );
int   gtCloseAll( App *pApp );
void  gtBackground( App *pApp, unsigned int color );
void  gtPollEvents( void );
void  gtWaitEvents( void );
void  gtWaitEventsTimeout( double timeout );
void  gtSetTargetFPS( App *pApp, int targetFPS );

// button.c
Button *gtButtonNew( App *pApp, void ( *onClick )() );
void    gtDrawButton( Button *pButton, int x, int y, const char *text, unsigned int background, unsigned int foreground );
void    gtFreeButton( Button *pButton );

// character_functions.c
const char *gtAddStr( const char *firstStr, ... );

/**
 * Returns the position of the first occurrence of a substring in a string.
 *
 * The `gtAt` function searches a given string (`target`) to find the first
 * occurrence of a specified substring (`search`). It returns the position
 * of the substring in the string, starting the indexing from 1. If the substring
 * is not found, the function returns 0.
 *
 * @param search The substring whose position is to be found.
 * @param target The string in which to search for the substring.
 * @return The position of the first occurrence of the substring in the string, starting from 1.
 *         Returns 0 if the substring is not found.
 *
 * Usage examples:
 * gtAt("a", "abcde") returns 1.
 * gtAt("bcd", "abcde") returns 2.
 * gtAt("a", "bcde") returns 0.
 */
int         gtAt( const char *search, const char *target );

const char *gtPadL( const char *str, int length );
const char *gtPadC( const char *str, int length );
const char *gtPadR( const char *str, int length );
const char *gtSpace( int count );
const char *gtStrFormat( const char *str, const char *strTemplate );
const char *gtSubStr( const char *str, int start, int count );
const char *gtLeft( const char *str, int count );
const char *gtRight( const char *str, int count );
int         gtRAt( const char *search, const char *target );
const char *gtStuff( const char *string, size_t start, size_t delete, const char *insert );

// directory_functions.c
bool        gtIsValidPath( const char *path );
bool        gtDirExists( const char *path );
int         gtDirMake( const char *path );
int         gtDirRemove( const char *path );
const char *gtDirDeleteLastPath( const char *path );
const char *gtDirLastName( const char *path );
const char *gtGetCurDir( void );
FileInfo   *gtDirectory( const char *currentDir, int *size );
void        gtFreeDirectory( FileInfo *pFiles );
const char *gtPathSeparator( void );

// http_functions.c
bool gtOpenURL( const char *url );
bool gtOpenEmailClient( const char *emailAddress );

// menu.c
Menu    *gtMenuNew( App *pApp );
MenuBar *gtMenuBarNew( Menu *pMenu, const char *title );
void     gtMenuBarAddItem( MenuBar *pMenuBar, const char *selectTitle, int key, int mods, void ( *onClick )( MenuItem * ) );
void     gtDrawMenu( Menu *pMenu );
void     gtFreeMenu( Menu *pMenu );
void     gtPrintMenuItemStruct( const MenuItem *pMenuItem );

// screen_functions.c
void gtDispBox( int col, int row, int colSpan, int rowSpan, unsigned int color );
int  gtMaxCol( App *pApp );
int  gtMaxRow( App *pApp );

// shapes.c
void gtPoint( int x, int y, unsigned int color );
void gtPointSize( int x, int y, int pointSize, unsigned int color );
void gtLine( int x, int y, int width, int height, unsigned int color );
void gtLineWidth( int x, int y, int width, int height, int lineWidth, unsigned int color );
void gtRect( int x, int y, int width, int height, unsigned int color );
void gtRectWidthToInside( int x, int y, int width, int height, int lineWidth, unsigned int color );
void gtRectWidthToCenter( int x, int y, int width, int height, int numberLines, unsigned int color );
void gtRectWidthToOutside( int x, int y, int width, int height, int numberLines, unsigned int color );
void gtFillgtRect( int x, int y, int width, int height, unsigned int color );

// statements_functions.c
/**
 * Executes a system command or opens files based on the provided command line string.
 *
 * The `gtRun` function is used to execute system commands, open URLs, or run executable files
 * using the system's default handlers or command-line interfaces. It determines the
 * appropriate command format based on the operating system and appends the provided
 * `commandLine` argument to it. This function is designed to be cross-platform,
 * supporting Windows, macOS, and Linux.
 *
 * Usage of the `system` function presents potential security risks, especially if
 * `commandLine` includes user-supplied data. Ensure that inputs are validated and sanitized.
 *
 * @param commandLine The command, URL, or file path to be executed or opened. This should be specified
 *                    either as a literal or as a character string.
 * @return Returns `true` if the command was executed successfully, `false` otherwise.
 *
 * Example:
 * bool success = gtRun("https://www.example.com"); // Opens a URL
 * bool executed = gtRun("/path/to/executable");    // Executes an executable file
 *
 * Note: The function uses predefined buffer sizes and command prefixes for different
 * platforms. Ensure the total command length does not exceed these limits.
 */
bool gtRun( const char *commandLine );
bool gtRunApp( const char *commandLine );

// text.c
void gtDrawText( int x, int y, const char *text, unsigned int foreground );
void gtDrawTextBG( int x, int y, const char *text, unsigned int background, unsigned int foreground );
void gtDispOutAt( int col, int row, const char *text, const char *color );

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// internal
void          check_open_gl_error( const char *stmt, const char *fname, int line, GLenum *errCode );
void          set_color_from_hex( unsigned int hexColor );
unsigned int  convert_hex_to_int( const char *hex );
char         *gt_strdup( const char *str );
size_t        strlen_utf8( const char* str );
const char   *utf8_offset_to_pointer( const char *str, int offset );
size_t        encode_utf8( char* s, unsigned int ch );
size_t        utf8_strlen( const char *str );

void GenerateShortcutName( char *buffer, int bufferSize, int key, int mods );

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// macros
#define REPORT_OPENGL_ERROR( stmt ) \
   GLenum errCode; \
   check_open_gl_error( stmt, __FILE__, __LINE__, &errCode ); \
   if( errCode != GL_NO_ERROR ) return;

#define MAX( a, b ) ( ( a ) < ( b ) ? ( b ) : ( a ) )
#define MIN( a, b ) ( ( a ) < ( b ) ? ( a ) : ( b ) )
#define UNUSED( n ) ( ( void )( n ) )
#define LEN( n ) ( sizeof( n ) / sizeof( n )[ 0 ] )

#define BEGINDRAWING( pApp ) do { gtBeginDrawing( pApp )
#define ENDDRAWING( pApp ) gtEndDrawing( pApp ); } while( 0 )

#endif /* End GTGLFW_H_ */
