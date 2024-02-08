/*
 *
 */

#include "gtglfw.h"

int main()
{
   App *app = gtCreateWindow( 830, 450, "Test gtStuff" );

   gtBackground( app, 0xF1F1F1 );

   while( ! gtMainLoop( app ) )
   {
      BEGINDRAWING( app );

         int startX = ( app->width - 54 ) / 2;
         int startY = ( app->height - 18 ) /2;

         gtDrawTextBG( startX, startY, gtStuff( "ABCDEF", 2, 3, "ąćę" ), 0xFAFAFA, 0x0 ); // Result: AąćęEF

      ENDDRAWING( app );
      gtWaitEvents();
   }

   gtCloseAll( app );
}
