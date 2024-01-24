/*
 *
 */

#include "gtglfw.h"

int main()
{
   App *app = gtCreateWindow( 830, 450, "Test text" );

   gtBackground( app, 0xF1F1F1 );

   while( ! gtMainLoop( app ) )
   {
      BEGINDRAWING( app );

         int startX = ( app->width - 225 ) / 2;
         int startY = ( app->height - 18 ) / 2;

         gtDrawTextBG( startX, startY, "Ą, Ć, Ę, Ł, Ń, Ó, Ś, Ź, Ż", 0xFAFAFA, 0x0 );

      ENDDRAWING( app );
      gtWaitEvents();
   }

   gtCloseAll( app );
}
