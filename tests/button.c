/*
 *
 */

#include "gtglfw.h"

void myOnClickFunction()
{
   printf( "Button was clicked!\n" );
   gtopenURL( "https://github.com/dev-harbour/gtglfw" );
}

int main()
{
   App *app = gtCreateWindow( 830, 450, "Test Button" );
   Button *b = gtButtonNew( app, myOnClickFunction );

   gtBackground( app, 0xEAEAEA );

   while( ! gtMainLoop( app ) )
   {
      BEGINDRAWING( app );

         int startX = ( app->width - b->width  ) / 2;
         int startY = ( app->height - b->height ) / 2;

         gtDrawButton( b, startX, startY, "Click Me", 0x323232, 0xFF0000 );

      ENDDRAWING( app );
      gtWaitEvents();
   }

   gtFreeButton( b );
   gtCloseAll( app );
}
