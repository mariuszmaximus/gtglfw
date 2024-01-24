/*
 *
 */

#include "gtglfw.h"

int main()
{
   App *app = gtCreateWindow( 720, 450, "Screen functions" );

   gtBackground( app, 0xEAEAEA );

   while( ! gtMainLoop( app ) )
   {
      BEGINDRAWING( app );

          gtDispBox( 0, 0, gtMaxCol( app ), gtMaxRow( app ), 0xFF0000 );

      ENDDRAWING( app );
      gtWaitEvents();
   }

   gtCloseAll( app );
}
