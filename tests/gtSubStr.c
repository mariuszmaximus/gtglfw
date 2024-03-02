/*
 *
 */

#include "gtglfw.h"

int main()
{
   App *app = gtCreateWindow( 830, 450, "Test gtSubStr" );
   const char *string = "ĄĆĘŁŃÓŚŹŻ";

   gtBackground( app, 0xF1F1F1 );

   while( ! gtMainLoop( app ) )
   {
      BEGINDRAWING( app );

         int startX = ( app->width - 27 ) / 2;
         int startY = ( app->height - 18 ) /2;

         gtDrawTextBG( startX, startY, gtSubStr( string, 1, 5 ), 0xFAFAFA, 0x0 ); // Result: ĄĆĘŁŃ

      ENDDRAWING( app );
      gtWaitEvents();
   }

   gtCloseAll( app );
}
