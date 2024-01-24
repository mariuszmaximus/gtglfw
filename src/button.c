/*
 * Copyright 2022 - 2024 Rafał Jopek ( rafaljopek at hotmail com )
 */

#include "gtglfw.h"

//* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
static bool is_point_inside( Button *pButton, int x, int y )
{
   return x >= pButton->x && x <= ( pButton->x + pButton->width ) && y >= pButton->y && y <= ( pButton->y + pButton->height );
}

Button *gtButtonNew( App *pApp, void ( *onClick )() )
{
   Button *pButton = malloc( sizeof( Button ) );
   if( ! pButton )
   {
      fprintf( stderr, "Failed to allocate memory for new button.\n" );
      return NULL;
   }
   memset( pButton, 0, sizeof( Button ) );

   pButton->pApp = pApp;
   pButton->height = 30;
   pButton->textMargin = 10;
   pButton->onClick = onClick;

   return pButton;
}

void gtDrawButton( Button *pButton, int x, int y, const char *text, unsigned int background, unsigned int foreground )
{
   if( pButton )
   {
      // Calculate the text width
      int len = strlen( text );
      int textWidth = BITMAP_WIDTH * len;

      // Setting the button width
      pButton->x = x;
      pButton->y = y;
      pButton->width = textWidth + 2 * pButton->textMargin;

      if( is_point_inside( pButton, pButton->pApp->cursorX, pButton->pApp->cursorY ) )
      {
         pButton->mouseOver = T;
      }
      else
      {
         pButton->mouseOver = F;
      }

      if( pButton->pApp->mouseButton == GLFW_MOUSE_BUTTON_LEFT && pButton->pApp->mouseAction == GLFW_PRESS )
      {
         if( pButton->mouseOver )
         {
            pButton->isClicked = T;
            if( pButton->onClick )
            {
               pButton->onClick();
            }
         }
      }
      else if( pButton->pApp->mouseButton == GLFW_MOUSE_BUTTON_LEFT && pButton->pApp->mouseAction == GLFW_RELEASE )
      {
         pButton->isClicked = F;
      }

      // Drawing the rectangle for the background
      gtFillgtRect( x, y, pButton->width, pButton->height, background );

      if( pButton->isClicked )
      {
         gtRectWidthToInside( x, y, pButton->width, pButton->height, 2, 0x0 );
      }
      else if( pButton->mouseOver )
      {
         gtRectWidthToOutside( x, y, pButton->width, pButton->height, 2, 0x0 );
      }

      // Drawing the text
      int textX = x + ( pButton->width - BITMAP_WIDTH * len ) / 2;
      int textY = y + ( pButton->height - BITMAP_HEIGHT ) / 2;
      gtDrawText( textX, textY, text, foreground );
   }
   else
   {
      printf( "Warning: Attempted to free a NULL pointer.\n" );
   }
}

void gtFreeButton( Button *pButton )
{
   if( pButton )
   {
      free( pButton );
   }
   else
   {
      printf( "Warning: Attempted to free a NULL pointer.\n" );
   }
}
