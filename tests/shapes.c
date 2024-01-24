/*
 *
 */

#include "gtglfw.h"

int main()
{
   App *app = gtCreateWindow( 830, 450, "Test shapes" );

   gtBackground( app, 0xEAEAEA );

   while( ! gtMainLoop( app ) )
   {
      BEGINDRAWING( app );

         int xMax = app->width;
         int yMax = app->height;

         for( int i = 0; i < 1000; i++ )
         {
            int x = 1 + rand() / ( ( RAND_MAX + 1u ) / xMax );
            int y = 1 + rand() / ( ( RAND_MAX + 1u ) / yMax );
            unsigned int color = rand() & 0xFFFFFF;
            gtPoint( x, y, color );
         }

         for( int i = 0; i < 10; i++ )
         {
            int x = 1 + rand() / ( ( RAND_MAX + 1u ) / xMax );
            int y = 1 + rand() / ( ( RAND_MAX + 1u ) / yMax );
            int size = rand() % 10 + 1;
            unsigned int color = rand() & 0xFFFFFF;
            gtPointSize( x, y, size, color );
         }

         for( int i = 0; i < 5; i++ )
         {
            int x = 1 + rand() / ( ( RAND_MAX + 1u ) / xMax );
            int y = 1 + rand() / ( ( RAND_MAX + 1u ) / yMax );
            int width  = 1 + rand() / ( ( RAND_MAX + 1u ) / xMax );
            int height = 1 + rand() / ( ( RAND_MAX + 1u ) / yMax );
            unsigned int color = rand() & 0xFFFFFF;
            gtLine( x, y, width, height, color );
         }

         for( int i = 0; i < 5; i++ )
         {
            int x = 1 + rand() / ( ( RAND_MAX + 1u ) / xMax );
            int y = 1 + rand() / ( ( RAND_MAX + 1u ) / yMax );
            int width  = 1 + rand() / ( ( RAND_MAX + 1u ) / xMax );
            int height = 1 + rand() / ( ( RAND_MAX + 1u ) / yMax );
            int size = rand() % 5 + 1;
            unsigned int color = rand() & 0xFFFFFF;
            gtLineWidth( x, y, width, height, size, color );
         }

      ENDDRAWING( app );
      gtWaitEvents();
   }

   gtCloseAll( app );
}
