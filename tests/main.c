/*
 *
 */

#include "gtglfw.h"

int main()
{
   App *app = gtCreateWindow( 830, 450, "Test window" );

   gtBackground( app, 0xEAEAEA );

   while( ! gtMainLoop( app ) )
   {
      BEGINDRAWING( app );

         int startX = ( app->width - 225 ) / 2;
         int startY = ( app->height - 18 ) / 2;

         gtDrawTextBG( startX, startY, "------Hello, world!------", 0xF1F1F1, 0x0 );

      ENDDRAWING( app );
      gtWaitEvents();
   }

   gtCloseAll( app );
}
