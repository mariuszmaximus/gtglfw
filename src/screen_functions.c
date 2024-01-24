/*
 * Copyright 2022 - 2024 Rafał Jopek ( rafaljopek at hotmail com )
 */

#include "gtglfw.h"

//* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
void gtDispBox( int col, int row, int colSpan, int rowSpan, unsigned int color )
{
   gtRect( col * 9 + 5, row * 18 + 9, colSpan * 9 - 10, rowSpan * 18 - 18, color );
}

int gtMaxCol( App *pApp )
{
   return pApp->width / 9;
}

int gtMaxRow( App *pApp )
{
   return pApp->height / 18;
}
