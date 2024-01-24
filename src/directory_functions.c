/*
 * Copyright 2022 - 2024 Rafał Jopek ( rafaljopek at hotmail com )
 */

#include "gtglfw.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
bool gtIsValidPath( const char *path )
{
   return( path != NULL && path[ 0 ] != '\0' );
}

bool gtDirExists( const char *path )
{
   #if defined( _WIN32 ) || defined( _WIN64 )
      DWORD ftyp = GetFileAttributesA( path );
      if( ftyp == INVALID_FILE_ATTRIBUTES )
         return F;

      if( ftyp & FILE_ATTRIBUTE_DIRECTORY )
         return T;

      return F;
   #else
      struct stat statbuf;
      if( stat( path, &statbuf ) != 0 )
         return F;

      return S_ISDIR( statbuf.st_mode );
    #endif
}

int gtDirMake( const char *path )
{
   #if defined( _WIN32 ) || defined( _WIN64 )
      if( CreateDirectory( path, NULL ) )
         return 0;
      if( GetLastError() == ERROR_ALREADY_EXISTS )
         return EEXIST;
      return -1;
    #else
      if( mkdir( path, 0755 ) == 0 )
         return 0;
      return errno;
    #endif
}

int gtDirRemove( const char *path )
{
   #if defined( _WIN32 ) || defined( _WIN64 )
      if( RemoveDirectory( path ) )
         return 0; // Sukces
      if( GetLastError() == ERROR_DIRECTORY )
         return EEXIST;
      return -1;
    #else
      if( rmdir( path ) == 0 )
         return 0;
      return errno;
    #endif
}

const char *gtDirDeleteLastPath( const char *path )
{
   static char result[ 512 ];
   char *lastPath;

   strncpy( result, path, sizeof( result ) - 1 );
   result[ sizeof( result ) - 1 ] = '\0';

   char separator = gtPathSeparator()[0];

   lastPath = strrchr( result, separator );
   if( lastPath != NULL )
   {
      char *secondlastPath = NULL;
      *lastPath = '\0';
      secondlastPath = strrchr( result, separator );

      if( secondlastPath != NULL )
      {
         *lastPath = separator;
         *( secondlastPath + 1 ) = '\0';
      }
      else
      {
         *lastPath = separator;
      }
   }
   return result;
}

const char *gtDirLastName( const char *path )
{
   static char result[ 512 ];
   int pathLength;

   if( path == NULL || ( pathLength = strlen( path ) ) == 0 )
   {
      result[ 0 ] = '\0';
      return result;
   }

   char separator = gtPathSeparator()[ 0 ];

   if( path[ pathLength - 1 ] == separator )
   {
      pathLength--;
   }

   for( int i = pathLength - 1; i >= 0; i-- )
   {
      if( path[ i ] == separator )
      {
         int length = pathLength - i - 1;
         strncpy( result, path + i + 1, length );
         result[ length ] = '\0';
         return result;
      }
   }

   strncpy( result, path, pathLength );
   result[ pathLength ] = '\0';

   return result;
}

const char *gtGetCurDir()
{
   static char result[ 512 ];
   size_t len;

   if( gt_GetCurrentDir( result, sizeof( result ) ) != NULL )
   {
      len = strlen( result );
      char separator = gtPathSeparator()[0];

      // Checking if there is already a separator at the end of the path
      if( result[ len - 1 ] != separator )
      {
         if( len < ( sizeof( result ) - 2 ) )
         {
            result[ len ] = separator;
            result[ len + 1 ] = '\0';
            }
            else
            {
               fprintf( stderr, "Błąd: Ścieżka przekracza dopuszczalny rozmiar bufora. \n" );
            }
        }

      return result;
   }
   else
   {
      fprintf( stderr, "getCurrentDirectory error. \n" );
      return NULL;
   }
}

#if defined( _WIN32 ) || defined( _WIN64 )
struct tm ConvertSystemTimeToTm( const SYSTEMTIME *st )
{
   struct tm tm;
   memset( &tm, 0, sizeof( tm ) );
   tm.tm_year = st->wYear - 1900; // Years since 1900
   tm.tm_mon  = st->wMonth - 1;   // The months are from 0 to 11
   tm.tm_mday = st->wDay;
   tm.tm_hour = st->wHour;
   tm.tm_min  = st->wMinute;
   tm.tm_sec  = st->wSecond;
   return tm;
}
#endif

FileInfo *gtDirectory( const char *currentDir, int *size )
{
   FileInfo *pFiles = NULL;
   int count = 0;
   int parentIndex = -1;

   #if defined( _WIN32 ) || defined( _WIN64 )
      WIN32_FIND_DATA findFileData;
      char searchPath[ 512 ];
      snprintf( searchPath, sizeof( searchPath ), "%s\\*", currentDir );
      HANDLE hFind = FindFirstFile( searchPath, &findFileData );

      if( hFind == INVALID_HANDLE_VALUE )
      {
         fprintf( stderr, "Directory cannot be opened. \n" );
         return NULL;
      }
      else
      {
         do
         {
            // Skipping the current directory
            if( strcmp( findFileData.cFileName, "." ) == 0 )
            {
               continue;
            }

            // Parent directory support
            if( strcmp( findFileData.cFileName, ".." ) == 0 )
            {
               parentIndex = count;
            }

            strncpy( pFiles[ count ].name, findFileData.cFileName, sizeof( pFiles[ count ].name ) - 1 );
            pFiles[ count ].name[ sizeof( pFiles[ count ].name ) - 1 ] = '\0';

            // Setting the file size
            LARGE_INTEGER fileSize;
            fileSize.LowPart = findFileData.nFileSizeLow;
            fileSize.HighPart = findFileData.nFileSizeHigh;
            snprintf( pFiles[ count ].size, sizeof( pFiles[ count ].size ), "%lld", fileSize.QuadPart );

            // Date and time conversion
            FILETIME ft = findFileData.ftLastWriteTime;
            SYSTEMTIME st;
            FileTimeToLocalFileTime( &ft, &ft );
            FileTimeToSystemTime( &ft, &st );

            struct tm tm = ConvertSystemTimeToTm( &st );
            strftime( pFiles[ count ].date, sizeof( pFiles[ count ].date ), "%Y-%m-%d", &tm );
            strftime( pFiles[ count ].time, sizeof( pFiles[ count ].time ), "%H:%M:%S", &tm );

            // Setting attributes
            strcpy( pFiles[ count ].attr, "" );
            if( findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
            {
               strcat( pFiles[ count ].attr, "D" ); // Directory
            }
            if( findFileData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN )
            {
               strcat( pFiles[ count ].attr, "H" ); // Hidden file
            }

            count++;
         }
         while( FindNextFile( hFind, &findFileData ) != 0 );
         FindClose( hFind );

         // Moving the parent directory to the first position
         if( parentIndex > 0 )
         {
            FileInfo temp = pFiles[ 0 ];
            pFiles[ 0 ] = pFiles[ parentIndex ];
            pFiles[ parentIndex ] = temp;
         }
      }
   #else
      DIR *pDir;
      struct dirent *entry;
      struct stat fileInfo;

      pDir = opendir( currentDir );
      if( pDir == NULL )
      {
         fprintf( stderr, "Directory cannot be opened.\n" );
         return NULL;
      }

      while( ( entry = readdir( pDir ) ) != NULL )
      {
         // Skipping the current directory
         if( strcmp( entry->d_name, "." ) == 0 )
         {
            continue;
         }

         // Parent directory support
         if( strcmp( entry->d_name, ".." ) == 0 )
         {
            parentIndex = count;
         }

         char fullPath[ 512 ];
         snprintf( fullPath, sizeof( fullPath ), "%s/%s", currentDir, entry->d_name );
         if( stat( fullPath, &fileInfo ) == -1 )
         {
            continue;
         }

         pFiles = realloc( pFiles, ( count + 1 ) * sizeof( FileInfo ) );
         if( ! pFiles )
         {
            closedir( pDir );
            return NULL;
         }

         strncpy( pFiles[ count ].name, entry->d_name, sizeof( pFiles[ count ].name ) - 1 );
         pFiles[ count ].name[ sizeof( pFiles[ count ].name ) - 1 ] = '\0';

         snprintf( pFiles[ count ].size, sizeof( pFiles[ count ].size ), "%ld", fileInfo.st_size );

         struct tm *tm = localtime( &fileInfo.st_mtime );
         strftime( pFiles[ count ].date, sizeof( pFiles[ count ].date ), "%Y-%m-%d", tm );
         strftime( pFiles[ count ].time, sizeof( pFiles[ count ].time ), "%H:%M:%S", tm );

         strcpy( pFiles[ count ].attr, "" );
         if( S_ISREG( fileInfo.st_mode ) )
         {
            strcat( pFiles[ count ].attr, "A" ); // Regular file
            if( fileInfo.st_mode & S_IXUSR )
            {
               strcat( pFiles[ count ].attr, "E" ); // Executable file
            }
         }
         if( S_ISDIR( fileInfo.st_mode ) )
         {
            strcat( pFiles[ count ].attr, "D" ); // Directory
         }
         if( entry->d_name[ 0 ] == '.' )
         {
            strcat( pFiles[ count ].attr, "H" ); // Hidden file
         }

         count++;
      }

      closedir( pDir );

      // Moving the parent directory to the first position
      if( parentIndex > 0 )
      {
         FileInfo temp = pFiles[ 0 ];
         pFiles[ 0 ] = pFiles[ parentIndex ];
         pFiles[ parentIndex ] = temp;
      }
   #endif
   *size = count; // Record the number of found files/directories
   return pFiles;
}

void gtFreeDirectory( FileInfo *pFiles )
{
   free( pFiles );
}

const char *gtPathSeparator()
{
   #if defined( _WIN32 ) || defined( _WIN64 )
      static const char separator[] = "\\";
   #else
      static const char separator[] = "/";
   #endif

   return separator;
}
