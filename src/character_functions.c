/*
 * Copyright 2022 - 2024 Rafał Jopek ( rafaljopek at hotmail com )
 */

#include "gtglfw.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
static size_t gt_strnlen( const char *s, size_t maxlen )
{
   size_t len;
   for( len = 0; len < maxlen && s[ len ]; len++ );
   return len;
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
const char *gtAddStr( const char *firstStr, ... )
{
   static char result[ 512 ];
   va_list args;
   va_start( args, firstStr );

   result[ 0 ] = '\0';
   size_t currentLength = 0;

   const char *str = firstStr;
   while( str != NULL )
   {
      size_t spaceLeft = sizeof( result ) - currentLength -1;
      size_t strLength = gt_strnlen( str, spaceLeft );

      if( strLength < spaceLeft )
      {
         strncat( result, str, strLength );
         currentLength += strLength;
      }
      else
      {
         fprintf( stderr, "Out of buffer space. \n" );
         break;
      }
      str = va_arg( args, const char * );
   }

   va_end( args );
   return result;
}

int gtAt( const char *search, const char *target )
{
   char *occurrence = strstr( target, search );
   if( occurrence == NULL )
   {
      return 0;
   }
   return ( int )( occurrence - target + 1 );
}

const char *gtPadL( const char *str, int length )
{
   static char result[ 512 ];
   int len = gt_utf8_strlen_single_byte( str );
   int byteLen = strlen( str );

   if( len >= length )
   {
      int copyLen = byteLen > 511 ? 511 : byteLen;
      memcpy( result, str, copyLen );
      result[ copyLen ] = '\0';
    }
    else
    {
      int padding = length - len;
      memset( result, ' ', padding );
      int copyLen = byteLen > ( 511 - padding) ? ( 511 - padding ) : byteLen;
      memcpy( result + padding, str, copyLen );
      result[ padding + copyLen ] = '\0';
   }

   return result;
}

const char *gtPadC( const char *str, int length )
{
   static char result[ 512 ];
   int len = gt_utf8_strlen_single_byte( str );
   int byteLen = strlen( str );

   if( len >= length )
   {
      int copyLen = byteLen > 511 ? 511 : byteLen;
      memcpy( result, str, copyLen );
      result[ copyLen ] = '\0';
   }
   else
   {
      int totalPadding = length - len;
      int paddingLeft  = totalPadding / 2;
      int paddingRight = totalPadding - paddingLeft;

      memset( result, ' ', paddingLeft );
      int copyLen = byteLen > ( 511 - paddingLeft ) ? ( 511 - paddingLeft ) : byteLen;
      memcpy( result + paddingLeft, str, copyLen );
      memset( result + paddingLeft + copyLen, ' ', paddingRight );
      result[ paddingLeft + copyLen + paddingRight ] = '\0';
    }

   return result;
}

const char *gtPadR( const char *str, int length )
{
   static char result[ 512 ];
   int len = gt_utf8_strlen_single_byte( str );
   int byteLen = strlen( str );

   if( len >= length )
   {
      int copyLen = byteLen > 511 ? 511 : byteLen;
      memcpy( result, str, copyLen );
      result[ copyLen ] = '\0';
   }
   else
   {
      int padding = length - len;
      memcpy( result, str, byteLen );
      memset( result + byteLen, ' ', padding );
      result[ byteLen + padding ] = '\0';
   }

   return result;
}

const char *gtSpace(int count) {
    static char result[512];

    if (count >= (int)sizeof(result)) {
        count = sizeof(result) -1;
    }

    if (count <= 0) {
        result[0] = '\0';
        return result;
    }

    memset(result, ' ', count);
    result[count] = '\0';

    return result;
}

const char *gtStrFormat( const char *str, const char *strTemplate )
{
   static char result[ 512 ];
   int lenStr = strlen( str );
   int lenTemplate = strlen( strTemplate );
   int j = lenStr - 1;

   // Wypełnij cały wynikowy ciąg spacjami
   for( int i = 0; i < lenTemplate; i++ )
   {
      result[ i ] = ' ';
   }

   // Formatuj ciąg od końca
   for( int i = lenTemplate - 1; i >= 0; i-- )
   {
      if( strTemplate[ i ] == '9' )
      {
         if( j >= 0 )
         {
            result[ i ] = str[ j ];
               j--;
         }
         else
         {
            // Gdy skończą się cyfry, przerwij pętlę
            break;
         }
      }
   }

   result[ lenTemplate ] = '\0';
   return result;
}

const char *gtSubStr( const char *str, int start, int count )
{
   static char result[ 512 ];
   int nSize = gt_utf8_strlen_multibyte( str );

   if( start > 0 )
   {
      if( --start > nSize ) count = 0;
   }

   if( count > 0 )
   {
      if( start < 0 )
      {
         start = nSize + start;
      }
      if( start < 0 )
      {
         start = 0;
      }
      if( start + count > nSize )
      {
         count = nSize - start;
      }
   }
   else
   {
      if( start < 0 )
      {
         start = nSize + start;
      }
      if( start < 0 )
      {
         start = 0;
      }
      count = nSize - start;
   }

   const char *byteStart = gt_utf8_offset_to_pointer( str, start );
   const char *byteEnd   = gt_utf8_offset_to_pointer( byteStart, count );

   // Copy byte by byte with proper handling for UTF-8
   size_t i = 0;
   while( byteStart < byteEnd && i < 511 )
   {
      result[ i++ ] = *byteStart++;
   }
   result[ i ] = '\0';

   return result;
}

const char *gtLeft( const char *str, int count )
{
   static char result[ 512 ];
   int len = gt_utf8_strlen_single_byte( str );

   if( count <= 0 )
   {
      return "";
   }

   if( count >= len )
   {
      count = len; // To avoid going beyond the string
   }

   const char *byteEnd = gt_utf8_offset_to_pointer( str, count );
   int byteCount = byteEnd - str;

   if( byteCount > 511 ) byteCount = 511; // Limited to buffer size

   memcpy( result, str, byteCount );
   result[ byteCount ] = '\0';

   return result;
}

const char *gtRight( const char *str, int count )
{
   static char result[ 512 ];
   int len = gt_utf8_strlen_single_byte( str );

   if( count <= 0 )
   {
      return "";
   }

   if( count > len )
   {
      count = len; // To avoid going beyond the string
   }

   const char *byteStart = gt_utf8_offset_to_pointer( str, len - count );
   int byteCount = str + strlen( str ) - byteStart;

   if( byteCount > 511 ) byteCount = 511; // Limited to buffer size

   memcpy( result, byteStart, byteCount );
   result[ byteCount ] = '\0';

   return result;
}

int gtRAt( const char *search, const char *target )
{
   int targetLen = gt_utf8_strlen_single_byte( target );
   int searchLen = gt_utf8_strlen_single_byte( search );

   if( searchLen > targetLen )
   {
      return 0;
   }

   for( int i = targetLen - searchLen; i >= 0; i-- )
   {
      const char *subString = gt_utf8_offset_to_pointer( target, i );

      if( strncmp( subString, search, strlen( search ) ) == 0 )
      {
         return i + 1;
      }
   }

   return 0;
}

const char *gtStuff( const char *str, size_t start, size_t delete, const char *insert )
{
   static char result[ 512 ];

   if( start > 0 )
   {
      if( --start > gt_utf8_strlen_multibyte( str ) ) delete = 0;
   }

   size_t len = gt_utf8_strlen_multibyte( str );
   size_t insertLen = gt_utf8_strlen_multibyte( insert );
   size_t resultSize = len + insertLen - delete;

   if( resultSize > 0 && resultSize < sizeof( result ) )
   {
      const char *byteStart = gt_utf8_offset_to_pointer( str, start );
      size_t byteCount = len - ( start + delete );

      // Kopiowanie bajtów z uwzględnieniem UTF-8
      size_t i = 0;
      while( i < 511 && str < byteStart )
      {
         result[ i++ ] = *str++;
      }

      // Kopiowanie wstawki
      i += snprintf( result + i, 511 - i, "%s", insert );

      // Pomijanie usuwanych bajtów
      str = gt_utf8_offset_to_pointer( str, delete );

      // Kopiowanie reszty
      while( i < 511 && byteCount-- )
      {
         result[ i++ ] = *str++;
      }

      result[ i ] = '\0';

      return result;
   }
   else
   {
      return "";
   }
}

const char *gtOs( void )
{
   #if defined( _WIN32) || defined( _WIN64 )
      return "Windows";
   #elif defined( __APPLE__ ) || defined( __MACH__ )
      return "macOS";
   #elif defined( __linux__ )
      return "Linux";
   #elif defined( __FreeBSD__ ) || defined( __NetBSD__ ) || defined( __OpenBSD__ ) || defined( __bsdi__ ) || defined( __DragonFly__ )
      return "BSD";
   #else
      return "Unknown";
   #endif
}
