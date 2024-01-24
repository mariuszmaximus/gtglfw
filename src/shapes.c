/*
 * Copyright 2022 - 2024 Rafał Jopek ( rafaljopek at hotmail com )
 */

#include "gtglfw.h"

//* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
void gtPoint( int x, int y, unsigned int color )
{
   set_color_from_hex( color );
   glPointSize( 1.0 );
   glBegin( GL_POINTS );
      glVertex2i( x, y + 1 );
   glEnd();
}

void gtPointSize( int x, int y, int pointSize, unsigned int color )
{
   if( pointSize < 1 )
   {
      fprintf( stderr, "Point size should be greater than 0.\n" );
      pointSize = 1;
   }

   set_color_from_hex( color );
   glPointSize( pointSize );
   glBegin( GL_POINTS );
      glVertex2i( x, y + 1 );
   glEnd();
}

void gtLine( int x, int y, int width, int height, unsigned int color )
{
   set_color_from_hex( color );
   glBegin( GL_LINES );
      glVertex2i( x, y );
      glVertex2i( x + width, y + height );
   glEnd();
}

void gtLineWidth( int x, int y, int width, int height, int lineWidth, unsigned int color )
{
   if( lineWidth < 1 )
   {
      fprintf( stderr, "Line width should be greater than 0.\n" );
      lineWidth = 1;
   }

   set_color_from_hex( color );
   glLineWidth( lineWidth );
   glBegin( GL_LINES );
      glVertex2i( x, y );
      glVertex2i( x + width, y + height );
   glEnd();
}

void gtRect( int x, int y, int width, int height, unsigned int color )
{
   set_color_from_hex( color );
   glBegin( GL_LINE_LOOP );
      glVertex2i( x + 1, y );
      glVertex2i( x + width, y );
      glVertex2i( x + width, y + height );
      glVertex2i( x, y + height - 1);
   glEnd();
}

void gtRectWidthToInside( int x, int y, int width, int height, int numberLines, unsigned int color )
{
   if( numberLines < 1 )
   {
      fprintf( stderr, "Number of lines should be greater than 0.\n" );
      numberLines = 1;
   }

   set_color_from_hex( color );
   for( int i = 0; i < numberLines; i++ )
   {
      // Top
      gtLine( x, y + i, width, 0, color );
      // Right
      gtLine( x + width - i, y + i, 0, height - i, color );
      // Bottom
      gtLine( x, y + height - i, width, 0, color );
      //Lef
      gtLine( x + i, y + i, 0, height - i, color );
   }
}

void gtRectWidthToCenter( int x, int y, int width, int height, int numberLines, unsigned int color )
{
   if( numberLines < 1 )
   {
      fprintf( stderr, "Number of lines should be greater than 0.\n" );
      numberLines = 1;
   }

   int halfLines = numberLines / 2;

   set_color_from_hex( color );
   for( int i = 0; i < halfLines; i++ )
   {
      // Drawing inwards
      // Top
      gtLine( x, y + i, width, 0, color );
      // Right
      gtLine( x + width - i, y + i, 0, height - i, color );
      // Bottom
      gtLine( x, y + height - i, width, 0, color );
      //Lef
      gtLine( x + i, y + i, 0, height - i, color );
   }

   for( int i = 0; i < numberLines - halfLines; i++ )
   {
      // Drawing outdoors
      // Top
      gtLine( x - i, y - i, width + i * 2, 0, color );
      // Right
      gtLine( x + width + i, y - i, 0, height + i * 2, color );
      // Bottom
      gtLine( x - 1 - i, y + height + i, width + 1 + i * 2, 0, color );
      // Left
      gtLine( x - i, y - i, 0, height + i * 2, color );
   }
}

void gtRectWidthToOutside( int x, int y, int width, int height, int numberLines, unsigned int color )
{
   if( numberLines < 1 )
   {
      fprintf( stderr, "Number of lines should be greater than 0.\n" );
      numberLines = 1;
   }

   set_color_from_hex( color );
   for( int i = 0; i < numberLines; i++ )
   {
      // Top
      gtLine( x - i, y - i, width + i * 2, 0, color );
      // Right
      gtLine( x + width + i, y - i, 0, height + i * 2, color );
      // Bottom
      gtLine( x - 1 - i, y + height + i, width + 1 + i * 2, 0, color );
      // Left
      gtLine( x - i, y - i, 0, height + i * 2, color );
   }
}

void gtFillgtRect( int x, int y, int width, int height, unsigned int color )
{
   set_color_from_hex( color );
   glBegin( GL_QUADS );
   glVertex2f( x, y );
   glVertex2f( x + width, y );
   glVertex2f( x + width, y + height );
   glVertex2f( x, y + height );
   glEnd();
}
