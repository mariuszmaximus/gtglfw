/*
 * Copyright 2022 - 2024 Rafał Jopek ( rafaljopek at hotmail com )
 */

#include "gtglfw.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
bool gtRun( const char *commandLine )
{
   const int commandBufferSize = 512;
   const int commandPrefixMaxSize = 10;

   if( strlen( commandLine ) > ( commandBufferSize - commandPrefixMaxSize -1 ) )
   {
      fprintf( stderr, "Command is too long\n" );
      return F;
   }

   char command[ commandBufferSize ]; // Create a buffer for command

#if defined( _WIN32 ) || defined( _WIN64 )
   snprintf( command, sizeof( command ), "start %s", commandLine );
#elif defined( __APPLE__ ) || defined( __MACH__ )
   snprintf( command, sizeof( command ), "open %s", commandLine );
#elif defined( __linux__ )
   snprintf( command, sizeof( command ), "xdg-open %s", commandLine );
#else
   fprintf( stderr, "Unsupported platform\n" );
   return F;
#endif

   // Execute the command and check the result
   int result = system( command );
   if( result != 0 )
   {
      fprintf( stderr, "Failed to open command \n" );
      return F;
   }

   return T;
}

bool gtRunApp( const char *commandLine )
{
   const int commandBufferSize = 512;
   const int commandPrefixMaxSize = 10;

   if( strlen( commandLine ) > ( commandBufferSize - commandPrefixMaxSize -1 ) )
   {
      fprintf( stderr, "Command is too long\n" );
      return F;
   }

   char command[ commandBufferSize ]; // Create a buffer for command

#if defined( _WIN32 ) || defined( _WIN64 )
   snprintf( command, sizeof( command ), "start %s", commandLine );
#elif defined( __APPLE__ ) || defined( __MACH__ )
   snprintf( command, sizeof( command ), "%s", commandLine );
#elif defined( __linux__ )
   snprintf( command, sizeof( command ), "%s", commandLine );
#else
   fprintf( stderr, "Unsupported platform\n" );
   return F;
#endif

   // Execute the command and check the result
   int result = system( command );
   if( result != 0 )
   {
      fprintf( stderr, "Failed to open command \n" );
      return F;
   }

   return T;
}
