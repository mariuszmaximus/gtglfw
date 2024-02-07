/*
 * Copyright 2022 - 2024 Rafał Jopek ( rafaljopek at hotmail com )
 */

#include "gtglfw.h"
#include "hc.h"

static Panel *leftPanel = NULL;
static Panel *rightPanel = NULL;
static Panel *activePanel = NULL;

int main()
{
   App *app = gtCreateWindow( 837, 450, "Harbour Commander" );
   double lastClickTime = 0;
   bool firstClick = F;
   int index;
   double currentTime;

   leftPanel = PanelInit( app );
   rightPanel = PanelInit( app );

   PanelFetchList( leftPanel, gtGetCurDir() );
   PanelFetchList( rightPanel, gtGetCurDir() );

   activePanel = leftPanel;

   gtBackground( app, 0xEAEAEA );

   SetIcon( app->window );

   while( ! gtMainLoop( app ) )
   {
      BEGINDRAWING( app );

         Autosize( app );
         DrawPanel( leftPanel );
         DrawPanel( rightPanel );
         DrawComdLine( app, activePanel );

         // keyAction
         if( app->keyAction == GLFW_PRESS )
         {
            switch( app->keyCode )
            {
            case GLFW_KEY_ESCAPE:

               if( app->keyMods & GLFW_MOD_CONTROL )
               {
                  PrintPanelStructure( activePanel );
                  app->keyAction = GLFW_RELEASE;
                  break;
               }

               gtExitApp( app );
               break;

            case GLFW_KEY_ENTER:

               index = activePanel->rowBar + activePanel->rowNo;
               if( gtAt( "D", activePanel->pFiles[ index ].attr ) == 0 )
               {
                  const char *commandLine = gtAddStr( activePanel->currentDir, activePanel->pFiles[ index ].name, NULL );
                  if( strcmp( activePanel->pFiles[ index ].attr, "AE" ) == 0 )
                  {
                     gtRunApp( commandLine );
                  }
                  else
                  {
                     gtRun( commandLine );
                  }
               }
               else
               {
                  ChangeDir( activePanel );
               }

               app->keyAction = GLFW_RELEASE;
               break;

            case GLFW_KEY_RIGHT:

               if( ( size_t )activePanel->cmdCol < gtMaxCol( app ) - strlen( activePanel->currentDir ) && ( size_t )activePanel->cmdCol < strlen( activePanel->cmdLine ) - 1 )
               {
                  activePanel->cmdCol++;
               }
               else if( activePanel->cmdColNo + ( size_t )activePanel->cmdCol < strlen( activePanel->cmdLine ) - 1 )
               {
                  activePanel->cmdColNo++;
               }

               app->keyAction = GLFW_RELEASE;
               break;

            case GLFW_KEY_LEFT:

               if( activePanel->cmdCol > 0 )
               {
                  activePanel->cmdCol--;
               }
               else if( activePanel->cmdColNo >= 1 )
               {
                  activePanel->cmdColNo--;
               }

               app->keyAction = GLFW_RELEASE;
               break;

            case GLFW_KEY_DOWN:

               if( activePanel->rowBar < activePanel->maxRow -3 && activePanel->rowBar <= activePanel->nFilesCount -2 )
               {
                  ++activePanel->rowBar;
               }
               else
               {
                  if( activePanel->rowNo + activePanel->rowBar <= activePanel->nFilesCount -3 )
                  {
                     ++activePanel->rowNo;
                  }
               }

               app->keyAction = GLFW_RELEASE;
               break;

            case GLFW_KEY_UP:

               if( activePanel->rowBar > 0 )
               {
                  --activePanel->rowBar;
               }
               else
               {
                  if( activePanel->rowNo > 0 )
                  {
                     --activePanel->rowNo;
                  }
               }

               app->keyAction = GLFW_RELEASE;
               break;

            case GLFW_KEY_PAGE_DOWN:

               if( activePanel->rowBar >= gtMaxRow( app ) -4 )
               {
                  if( activePanel->rowNo + gtMaxRow( app ) <= activePanel->nFilesCount )
                  {
                     activePanel->rowNo += gtMaxRow( app ) - activePanel->rowBar;
                  }
               }
               activePanel->rowBar = MIN( gtMaxRow( app ) -4, activePanel->nFilesCount - activePanel->rowNo -1 );

               app->keyAction = GLFW_RELEASE;
               break;

            case GLFW_KEY_PAGE_UP:

               if( activePanel->rowBar <= 1 )
               {
                  if( activePanel->rowNo - gtMaxRow( app ) >= 0 )
                  {
                     activePanel->rowNo -= gtMaxRow( app );
                  }
                  else
                  {
                     activePanel->rowNo = 0; // Jeśli nie ma wystarczająco dużo wierszy do przewinięcia, ustaw na początek.
                  }
               }
               activePanel->rowBar = 0;

               app->keyAction = GLFW_RELEASE;
               break;

            case GLFW_KEY_TAB:

               if( activePanel == leftPanel )
               {
                  activePanel = rightPanel;
               }
               else
               {
                  activePanel = leftPanel;
               }
               app->keyAction = GLFW_RELEASE;
               break;

            case GLFW_KEY_INSERT:

               index = activePanel->rowBar + activePanel->rowNo;
               // Sprawdzenie, czy aktualny element nie jest katalogiem nadrzędnym
               if( strcmp( activePanel->pFiles[ index ].name, ".." ) != 0 )
               {
                  // Przełączanie stanu 'state' wybranego pliku
                  if( activePanel->pFiles[ index ].state )
                  {
                     activePanel->pFiles[ index ].state = F;
                  }
                  else
                  {
                     activePanel->pFiles[ index ].state = T;
                  }

                  // Przewijanie listy plików w panelu, jeśli to możliwe
                  if( activePanel->rowBar < activePanel->maxRow -3 && activePanel->rowBar <= activePanel->nFilesCount -2 )
                  {
                     ++activePanel->rowBar;
                  }
                  else
                  {
                     if( activePanel->rowNo + activePanel->rowBar <= activePanel->nFilesCount -3 )
                     {
                        ++activePanel->rowNo;
                     }
                  }
               }

               app->keyAction = GLFW_RELEASE;
               break;

            default:
               break;
            }
         }

         // mouseAction
         if( app->mouseAction == GLFW_PRESS )
         {
            switch( app->mouseButton )
            {
            case GLFW_MOUSE_BUTTON_LEFT:

               // Logika dla podwójnego kliknięcia
               currentTime = glfwGetTime();
               if( firstClick && currentTime - lastClickTime < DOUBLE_CLICK_TIME )
               {
                  int index = activePanel->rowBar + activePanel->rowNo;
                  if( gtAt( "D", activePanel->pFiles[ index ].attr ) == 0 )
                  {
                     const char *commandLine = gtAddStr( activePanel->currentDir, activePanel->pFiles[ index ].name, NULL );
                     if( strcmp( activePanel->pFiles[ index ].attr, "AE" ) == 0 )
                     {
                        gtRunApp( commandLine );
                     }
                     else
                     {
                        gtRun( commandLine );
                     }
                  }
                  else
                  {
                     ChangeDir( activePanel );
                  }
                  firstClick = F; // Reset stanu
               }
               else
               {
                  firstClick = T;
                  lastClickTime = currentTime;
               }

               double cursorX = app->cursorX / 9;
               double cursorY = app->cursorY / 18;

               int rowIndex = ( int ) cursorY - 1;

               if( rowIndex >= 0 && rowIndex < gtMaxRow( app ) )
               {
                  if( cursorX < gtMaxCol( app ) / 2 )
                  {
                     activePanel = leftPanel;
                  }
                  else
                  {
                     activePanel = rightPanel;
                  }

                  if( rowIndex < activePanel->nFilesCount )
                  {
                     activePanel->rowBar = rowIndex;
                  }
               }
               app->mouseAction = GLFW_RELEASE;
               break;

            case GLFW_MOUSE_BUTTON_RIGHT:

               app->mouseAction = GLFW_RELEASE;
               break;

            default:
               break;
            }
         }

         if( app->scrollYOffset != 0 )
         {
            if( app->scrollYOffset > 0 )
            {
               // Logika przewijania w górę
               if( activePanel->rowBar > 0 )
               {
                  --activePanel->rowBar;
               }
               else
               {
                  if( activePanel->rowNo > 0 )
                  {
                     --activePanel->rowNo;
                  }
               }
            }
            else if( app->scrollYOffset < 0 )
            {
               // Logika przewijania w dół
               if( activePanel->rowBar < activePanel->maxRow -3 && activePanel->rowBar <= activePanel->nFilesCount -2 )
               {
                  ++activePanel->rowBar;
               }
               else
               {
                  if( activePanel->rowNo + activePanel->rowBar <= activePanel->nFilesCount -3 )
                  {
                     ++activePanel->rowNo;
                  }
               }
            }
            DrawPanel( activePanel );
            app->scrollXOffset = 0;
            app->scrollYOffset = 0;
         }

      ENDDRAWING( app );
      gtWaitEvents();
   }

   PanelFree( leftPanel );
   PanelFree( rightPanel );
   activePanel = NULL;

   gtCloseAll( app );
}

static Panel *PanelInit( App *pApp )
{
   Panel *pPanel = malloc( sizeof( Panel ) );
   if( ! pPanel )
   {
      fprintf( stderr, "Failed to allocate memory for Menu. \n" );
      return NULL;
   }

   memset( pPanel, 0, sizeof( Panel ) );
   pPanel->pApp = pApp;

   pPanel->cmdLine = "";

   pPanel->isFirstDirectory  = T;
   pPanel->isHiddenDirectory = F;
   pPanel->isFirstFile       = F;
   pPanel->isHiddenFile      = F;

   return pPanel;
}

static void PanelFree( Panel *pPanel )
{
   if( pPanel )
   {
      if( pPanel->pFiles )
      {
         gtFreeDirectory( pPanel->pFiles );
      }
      free( pPanel );
   }
}

static void PanelFetchList( Panel *pPanel, const char *currentDir )
{
   if( currentDir == NULL || ! gtIsValidPath( currentDir ) )
   {
      const char *dir = gtGetCurDir();
      strncpy( pPanel->currentDir, dir, sizeof( pPanel->currentDir ) -1 );
      pPanel->currentDir[ sizeof( pPanel->currentDir ) -1 ] = '\0';
   }
   else
   {
      strncpy( pPanel->currentDir, currentDir, sizeof( pPanel->currentDir ) -1 );
      pPanel->currentDir[ sizeof( pPanel->currentDir ) -1 ] = '\0';
   }

   free( pPanel->pFiles );
   pPanel->nFilesCount = 0;
   pPanel->pFiles = gtDirectory( pPanel->currentDir, &pPanel->nFilesCount );

   if( pPanel->isFirstDirectory )
   {
      qsort( pPanel->pFiles, pPanel->nFilesCount, sizeof( FileInfo ), CompareFiles );
   }
}

int CompareFiles( const void *a, const void *b )
{
   FileInfo *fileA = ( FileInfo * ) a;
   FileInfo *fileB = ( FileInfo * ) b;

   // The ".." Directory always comes first
   if( strcmp( fileA->name, ".." ) == 0 ) return -1;
   if( strcmp( fileB->name, ".." ) == 0 ) return 1;

   // Directories before files
   bool isDirA = strchr( fileA->attr, 'D' ) != NULL;
   bool isDirB = strchr( fileB->attr, 'D' ) != NULL;
   if( isDirA != isDirB )
   {
      return IIF( isDirA, -1, 1 );
   }

   // Hidden files/directories after regular ones
   bool isHiddenA = strchr( fileA->attr, 'H' ) != NULL;
   bool isHiddenB = strchr( fileB->attr, 'H' ) != NULL;
   if( isHiddenA != isHiddenB )
   {
      return IIF( isHiddenA, 1, -1 );
   }

   return strcmp( fileA->name, fileB->name );
}

static void Autosize( App *pApp )
{
   Resize( leftPanel, 0, 0, gtMaxCol( pApp ) / 2, gtMaxRow( pApp ) -1 );
   Resize( rightPanel, gtMaxCol( pApp ) / 2, 0, gtMaxCol( pApp ) / 2, gtMaxRow( pApp ) -1 );
}

static void Resize( Panel *pPanel, int col, int row, int maxCol, int maxRow )
{
   pPanel->col    = col;
   pPanel->row    = row;
   pPanel->maxCol = maxCol;
   pPanel->maxRow = maxRow;
}

static void DrawPanel( Panel *pPanel )
{
   int row, i = 0;
   int longestName = 4;
   int longestSize = 0;

   longestName = MAX( longestName, FindLongestName( pPanel ) );
   longestSize = MAX( longestSize, FindLongestSize( pPanel ) );

   if( activePanel == pPanel )
   {
      gtDispBox( pPanel->col, pPanel->row, pPanel->maxCol, pPanel->maxRow, 0xFF0000 );
   }
   else
   {
      gtDispBox( pPanel->col, pPanel->row, pPanel->maxCol, pPanel->maxRow, 0x323232 );
   }

   i += pPanel->rowNo;
   for( row = pPanel->row + 1; row < pPanel->maxRow -1; row++ )
   {
      if( i < pPanel->nFilesCount )
      {
         gtDispOutAt( pPanel->col + 1, row,
            gtPadR( PaddedString( pPanel, longestName, longestSize,
               pPanel->pFiles[ i ].name,
               pPanel->pFiles[ i ].size,
               pPanel->pFiles[ i ].date,
               pPanel->pFiles[ i ].time,
               pPanel->pFiles[ i ].attr ), pPanel->maxCol -2 ),
            IIF( activePanel == pPanel && i == pPanel->rowBar + pPanel->rowNo,
            IIF( pPanel->pFiles[ i ].state == T, "323232/FF4D4D", "323232/00FF00" ),
            SelectColor( pPanel->pFiles[ i ].attr, pPanel->pFiles[ i ].state ) ) );
         ++i;
      }
      else
      {
         break;
      }
   }
}

static const char *PaddedString( Panel *pPanel, int longestName, int longestSize, const char *name, const char *size, const char *date, const char *time, const char *attr )
{
   static char formattedLine[ 512 ];
   char fileName[ longestName + 1 ];
   char fileSize[ longestSize + 1 ];
   char fileDate[ 11 ];
   char fileTime[ 9 ];
   char fileAttr[ 6 ];

   int maxFileNameLength = pPanel->maxCol - longestSize - 11 - 9 - 6 - 2;

   if( strcmp( name, ".." ) == 0 )
   {
      SafeStrCopy( fileName, gtPadR( gtAddStr( "[", name, "]", NULL ), longestName - 11 - 9 - 6 - 2 ), sizeof( fileName ) );
   }
   else
   {
      int nameLen = strnlen( name, sizeof( fileName ) );
      int copyLength = IIF( nameLen < maxFileNameLength, nameLen, maxFileNameLength );
      SafeStrCopy( fileName, name, copyLength + 1 );
   }

   if( strchr( attr, 'D' ) != NULL )
   {
      SafeStrCopy( fileSize, gtPadL( "DIR", longestSize ), sizeof( fileSize ) );
   }
   else
   {
      SafeStrCopy( fileSize, gtPadL( size, longestSize ), sizeof( fileSize ) );
   }

   SafeStrCopy( fileAttr, gtPadL( attr, 5 ), sizeof( fileAttr ) );
   SafeStrCopy( fileDate, gtPadL( date, 10 ), sizeof( fileDate ) );
   SafeStrCopy( fileTime, gtPadL( time, 8 ), sizeof( fileTime ) );

   size_t fileSizeLength = IIF( fileSize[ 0 ] != '\0', strnlen( fileSize, longestSize ), 0 );
   int spacesNeeded = pPanel->maxCol - strlen_utf8( fileName ) - fileSizeLength - strlen( fileAttr ) - strlen( fileDate ) - strlen( fileTime ) - 5;

   snprintf( formattedLine, sizeof( formattedLine ), "%s%*s%s %s %s %s", fileName, spacesNeeded, "", fileSize, fileAttr, fileDate, fileTime );

   return formattedLine;
}

static const char *SelectColor( const char *attr, bool state )
{
   if( state == T )
   {
      return "EAEAEA/B30000"; // Kolor na zaznaczonych plikach
   }
   else if( strcmp( attr, "DH" ) == 0 || strcmp( attr, "AH" ) == 0 )
   {
      return "EAEAEA/72A0E5";
   }
   else
   {
      return "EAEAEA/323232"; // Kolor dla pozostałych plików
   }
}

void SafeStrCopy( char *dest, const char *src, size_t destSize )
{
   if( destSize == 0 ) return;

   strncpy( dest, src, destSize -1 );
   dest[ destSize -1 ] = '\0';
}

static void DrawComdLine( App *pApp, Panel *pPanel )
{
   int width = pApp->width;
   int height = pApp->height;

   gtDrawTextBG( 0, height -18,
      gtPadR( pPanel->currentDir, width / 9 + 1 ), 0x323232, 0x00FF00 );

   gtDrawText( strlen( pPanel->currentDir ) * 9 + pPanel->cmdCol * 9, height -18, "_", 0xFFFFFF );
}

static void ChangeDir( Panel *pPanel )
{
   int i = pPanel->rowBar + pPanel->rowNo;
   const char *newDir;

   if( strcmp( pPanel->pFiles[ i ].name, ".." ) == 0 )
   {
      const char *tmpDir = gtDirLastName( pPanel->currentDir );
      newDir = gtDirDeleteLastPath( pPanel->currentDir );

      UpdatePanelFetchList( pPanel, newDir );

      int lastPosition = MAX( DirIndexName( pPanel, tmpDir ), 1 ) ;
      if( lastPosition > activePanel->maxRow -3 )
      {
         activePanel->rowNo  = lastPosition % ( activePanel->maxRow -3 );
         activePanel->rowBar = activePanel->maxRow -3;
      }
      else
      {
         activePanel->rowNo  = 0;
         activePanel->rowBar = lastPosition;
      }
   }
   else
   {
      newDir = gtAddStr( pPanel->currentDir, pPanel->pFiles[ i ].name, gtPathSeparator(), NULL );
      pPanel->rowBar = 0;
      pPanel->rowNo  = 0;

      UpdatePanelFetchList( pPanel, newDir );
   }
}

static void UpdatePanelFetchList( Panel *pPanel, const char *newDir )
{
   strncpy( pPanel->currentDir, newDir, sizeof( pPanel->currentDir ) -1 );
   pPanel->currentDir[ sizeof( pPanel->currentDir ) -1 ] = '\0';
   PanelFetchList( pPanel, newDir );
}

static int DirIndexName( Panel *pPanel, const char *tmpDir )
{
   for( int i = 0; i < pPanel->nFilesCount; i++ )
   {
      if( strcmp( pPanel->pFiles[ i ].name, tmpDir ) == 0 )
      {
         return i;
      }
   }
   return -1;
}

static int FindLongestName( Panel *pPanel )
{
   int longestName = 0;

   for( int i = 0; i < pPanel->nFilesCount; i++ )
   {
      int currentNameLength = strlen( pPanel->pFiles[ i ].name );
      if( currentNameLength > longestName )
      {
         longestName = currentNameLength;
      }
   }

   return longestName;
}

static int FindLongestSize( Panel *pPanel )
{
   int longestSize = 0;

   for( int i = 0; i < pPanel->nFilesCount; i++ )
   {
      int currentSizeLength = strlen( pPanel->pFiles[ i ].size );
      if( currentSizeLength > longestSize )
      {
         longestSize = currentSizeLength;
      }
   }

   return longestSize;
}

static void SetIcon( GLFWwindow* window )
{
   int x, y;
   unsigned char pixels[ 16 * 16 * 4 ];
   unsigned char *target = pixels;
   GLFWimage img = { 16, 16, pixels };

   for( y = 0;  y < img.width;  y++ )
   {
      for( x = 0;  x < img.height;  x++ )
      {
         if( hc_logo[ y ][ x ] == '0' )
         {
            memcpy( target, icon_colors[ 1 ], 4 ); // green color
         }
         else
         {
            memset( target, 0, 4 ); // black color
         }
         target += 4;
      }
   }

   glfwSetWindowIcon( window, 1, &img );
}

static void PrintPanelStructure( const Panel *pPanel )
{
   printf( "\033[2J" ); // Czyści ekran
   printf( "\033[H" );  // Ustawia kursor na początek

   printf( "Panel Structure \n" );
   printf( "{\n" );
   printf( "   col        : %d \n", pPanel->col );
   printf( "   row        : %d \n", pPanel->row );
   printf( "   maxCol     : %d \n", pPanel->maxCol );
   printf( "   maxRow     : %d \n", pPanel->maxRow );
   printf( "   currentDir : %s \n", pPanel->currentDir );
   printf( "   nFilesCount: %d \n", pPanel->nFilesCount );
   printf( "   rowBar     : %d \n", pPanel->rowBar );
   printf( "   rowNo      : %d \n", pPanel->rowNo );
   printf( "   cmdLine    : %s \n", pPanel->cmdLine );
   printf( "   cmdCol     : %d \n", pPanel->cmdCol );
   printf( "   cmdColNo   : %d \n", pPanel->cmdColNo );
   printf( "\n" );
   int index = pPanel->rowBar + pPanel->rowNo;
   if( index < pPanel->nFilesCount )
   {
      printf( "   Selected File Info: \n" );
      printf( "                Name : %s \n", pPanel->pFiles[ index ].name );
      printf( "                Size : %s \n", pPanel->pFiles[ index ].size );
      printf( "                Date : %s \n", pPanel->pFiles[ index ].date );
      printf( "                Time : %s \n", pPanel->pFiles[ index ].time );
      printf( "                Attr : %s \n", pPanel->pFiles[ index ].attr );
      printf( "                State: %d \n", pPanel->pFiles[ index ].state );
   }
   else
   {
      printf( "   No file selected or index out of range \n" );
   }

   printf( "}\n" );
   printf( "\n" );
}
