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
   App *app = gtCreateWindow( 830, 450, "Harbour Commander" );

   leftPanel = PanelInit( app );
   rightPanel = PanelInit( app );

   PanelFetchList( leftPanel, gtGetCurDir() );
   PanelFetchList( rightPanel, gtGetCurDir() );

   activePanel = leftPanel;

   gtBackground( app, 0xEAEAEA );

   while( ! gtMainLoop( app ) )
   {
      BEGINDRAWING( app );

         Autosize( app );
         DrawPanel( leftPanel );
         DrawPanel( rightPanel );
         DrawComdLine( app, activePanel );

         if( app->keyAction == GLFW_PRESS )
         {
            switch( app->keyCode )
            {
            case GLFW_KEY_ESCAPE:

               gtExitApp( app );
               break;

            case GLFW_KEY_ENTER:

               int index = activePanel->rowBar + activePanel->rowNo;
               if( gtAt( "D", activePanel->pFiles[ index ].attr ) == 0 )
               {
                  gtRun( gtAddStr( activePanel->currentDir, activePanel->pFiles[ index ].name, NULL ) );
               }
               else
               {
                  ChangeDir( activePanel );
               }

               app->keyAction = GLFW_RELEASE;
               break;

            case GLFW_KEY_RIGHT:

               app->keyAction = GLFW_RELEASE;
               break;

            case GLFW_KEY_LEFT:

               app->keyAction = GLFW_RELEASE;
               break;

            case GLFW_KEY_DOWN:

               if( activePanel->rowBar < activePanel->maxRow - 3 && activePanel->rowBar <= activePanel->nFilesCount - 2 )
               {
                  ++activePanel->rowBar;
               }
               else
               {
                  if( activePanel->rowNo + activePanel->rowBar <= activePanel->nFilesCount - 3 )
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

            default:
               break;
            }
         }

      //PrintPanelStructure( activePanel );

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
      strncpy( pPanel->currentDir, dir, sizeof( pPanel->currentDir ) - 1 );
      pPanel->currentDir[ sizeof( pPanel->currentDir ) - 1 ] = '\0';
   }
   else
   {
      strncpy( pPanel->currentDir, currentDir, sizeof( pPanel->currentDir ) - 1 );
      pPanel->currentDir[ sizeof( pPanel->currentDir ) - 1 ] = '\0';
   }
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
      return isHiddenA ? 1 : -1;
      return IIF( isHiddenA, 1, -1 );
   }

   return strcmp( fileA->name, fileB->name );
}

static void Autosize( App *pApp )
{
   Resize( leftPanel, 0, 0, gtMaxCol( pApp ) / 2, gtMaxRow( pApp ) - 1 );
   Resize( rightPanel, gtMaxCol( pApp ) / 2, 0, gtMaxCol( pApp ) / 2, gtMaxRow( pApp ) - 1 );
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
      gtDispBox( pPanel->col, pPanel->row, pPanel->maxCol, pPanel->maxRow, 0x000000 );
   }

   i += pPanel->rowNo;
   for( row = pPanel->row + 1; row < pPanel->maxRow - 1; row++ )
   {
      if( i < pPanel->nFilesCount )
      {
         gtDispOutAt( pPanel->col + 1, row,
            gtPadR( PaddedString( longestName, longestSize,
               pPanel->pFiles[ i ].name,
               pPanel->pFiles[ i ].size,
               pPanel->pFiles[ i ].attr ), pPanel->maxCol - 2 ),
            IIF( activePanel == pPanel && i == pPanel->rowBar + pPanel->rowNo, "000000/00FF00", "EAEAEA/000000" ) );
         ++i;
      }
      else
      {
         break;
      }
   }
}

static const char *PaddedString( int longestName, int longestSize, const char *name, const char *size, const char *attr )
{
   static char fileName[ 512 ];
   static char fileSize[ 20 ];

   if( strcmp( name, ".." ) == 0 )
   {
      const char *tempStr = gtPadR( gtAddStr( "[", name, "]", NULL ), longestName );
      SafeStrCopy( fileName, tempStr, sizeof( fileName ) );
   }
   else
   {
      const char *tempStr = gtPadR( name, longestName );
      SafeStrCopy( fileName, tempStr, sizeof( fileName ) );
   }

   if( strchr(attr, 'D' ) != NULL )
   {
      const char *tempStr = gtPadL( "DIR", longestSize );
      SafeStrCopy( fileSize, tempStr, sizeof( fileSize ) );
   }
   else
   {
      const char *tempStr = gtPadL( size, longestSize );
      SafeStrCopy( fileSize, tempStr, sizeof( fileSize ) );
   }

   return gtAddStr( fileName, "   ", fileSize, NULL );
}

void SafeStrCopy( char *dest, const char *src, size_t destSize )
{
   if( destSize == 0 ) return;

   strncpy( dest, src, destSize - 1 );
   dest[ destSize - 1 ] = '\0';
}

static void DrawComdLine( App *pApp, Panel *pPanel )
{
   int width = pApp->width;
   int height = pApp->height;

   gtDrawTextBG( 0, height -18,
      gtPadR( pPanel->currentDir, width / 9 + 1 ), 0x000000, 0x00FF00 );
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
      if( lastPosition > activePanel->maxRow - 3 )
      {
         activePanel->rowNo  = lastPosition % ( activePanel->maxRow - 3 );
         activePanel->rowBar = activePanel->maxRow - 3;
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
   strncpy( pPanel->currentDir, newDir, sizeof( pPanel->currentDir ) - 1 );
   pPanel->currentDir[ sizeof( pPanel->currentDir ) - 1 ] = '\0';
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
   }
   else
   {
      printf( "   No file selected or index out of range \n" );
   }

   printf( "}\n" );
   printf( "\n" );
}
