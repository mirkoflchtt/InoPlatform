#include "InoBase64.h"

static const char* base64_enc_map = 
  "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
  "abcdefghijklmnopqrstuvwxyz"
  "0123456789+/";
/*
{
    'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',
    'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd',
    'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
    'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
    'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', '+', '/'
};
*/
static const ino_u8 base64_dec_map[128] = {
  127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
  127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
  127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
  127, 127, 127, 127, 127, 127, 127, 127, 127, 127,
  127, 127, 127,  62, 127, 127, 127,  63,  52,  53,
   54,  55,  56,  57,  58,  59,  60,  61, 127, 127,
  127,  64, 127, 127, 127,   0,   1,   2,   3,   4,
    5,   6,   7,   8,   9,  10,  11,  12,  13,  14,
   15,  16,  17,  18,  19,  20,  21,  22,  23,  24,
   25, 127, 127, 127, 127, 127, 127,  26,  27,  28,
   29,  30,  31,  32,  33,  34,  35,  36,  37,  38,
   39,  40,  41,  42,  43,  44,  45,  46,  47,  48,
   49,  50,  51, 127, 127, 127, 127, 127
};

INO_NAMESPACE

ino_error encodeBase64(
  char* dst, ino_size* dst_size,
  const ino_u8* src, const ino_size src_size)
{
  ino_size i, n;
  ino_i32 C1, C2, C3;
  char* p;

  if (src_size == 0) {
    *dst_size = 0;
    return BASE64_ERROR_NONE;
  }

  i = src_size % 3;
  n = (src_size / 3) + (i != 0);
  n *= 4;

  if( ( *dst_size < n + 1 ) || ( NULL == dst ) ) {
    *dst_size = n + 1;
    return BASE64_ERROR_BUFFER_SMALL;
  }

  *dst_size = n;
  src = src + src_size - 1;
  p = dst + n;
  *p-- = 0;

  if (i > 0) {
    C2 = (i > 1) ? *src-- : 0;
    C1 = *src--;

    *p-- = '=';
    *p-- = (i > 1) ? base64_enc_map[((C2 & 15) << 2) & 0x3F] : '=';
    *p-- = base64_enc_map[(((C1 & 3) << 4) + (C2 >> 4)) & 0x3F];
    *p-- = base64_enc_map[(C1 >> 2) & 0x3F];
  }

  for ( ; p > dst; ) {
    C3 = *src--;
    C2 = *src--;
    C1 = *src--;

    *p-- = base64_enc_map[C3 & 0x3F];
    *p-- = base64_enc_map[(((C2 & 15) << 2) + (C3 >> 6)) & 0x3F];
    *p-- = base64_enc_map[(((C1 &  3) << 4) + (C2 >> 4)) & 0x3F];
    *p-- = base64_enc_map[(C1 >> 2) & 0x3F];  
  }

  return BASE64_ERROR_NONE;
}

ino_error decodeBase64(
  ino_u8* dst, ino_size* dst_size,
  const char* src, const ino_size src_size)
{
  const ino_u8* s = (const ino_u8*)src;
  ino_size i, n;
  ino_u32 j, x;
  ino_u8* p;

  /* First pass: check for validity and get output length */
  for( i = n = j = 0; i < src_size; i++ ) {
    /* Skip spaces before checking for EOL */
    x = 0;
    while ((i < src_size) && (s[i] == ' ')) {
      ++i; ++x;
    }

    /* Spaces at end of buffer are OK */
    if(i == src_size)
      break;

    if ((src_size - i) >= 2 &&
        s[i] == '\r' && s[i + 1] == '\n')
      continue;

    if (s[i] == '\n')
      continue;

    /* Space inside a line is an error */
    if (x != 0)
      return BASE64_ERROR_INVALID_CHARACTER;

    if((s[i] == '=') && (++j > 2))
      return BASE64_ERROR_INVALID_CHARACTER;

    if ((s[i] > 127) || (base64_dec_map[s[i]] == 127))
      return BASE64_ERROR_INVALID_CHARACTER;

    if ((base64_dec_map[s[i]] < 64) && (j != 0))
      return BASE64_ERROR_INVALID_CHARACTER;
    n++;
  }

  if (n == 0) {
    *dst_size = 0;
    return BASE64_ERROR_NONE;
  }

  /* The following expression is to calculate the following formula without
   * risk of integer overflow in n:
   *     n = ( ( n * 6 ) + 7 ) >> 3;
   */
  n = (6 * (n >> 3)) + ((6 * (n & 0x7) + 7) >> 3);
  n -= j;

  if ((dst == NULL) || (*dst_size < n)) {
    *dst_size = n;
    return BASE64_ERROR_BUFFER_SMALL;
  }

  for (j = 3, n = x = 0, p = dst; i > 0; i--, s++)
  {
    if ((*s == '\r') || (*s == '\n') || (*s == ' '))
      continue;

    j -= (base64_dec_map[*s] == 64);
    x  = (x << 6) | (base64_dec_map[*s] & 0x3F);

    if (++n == 4) {
      n = 0;
      if( j > 0 ) *p++ = (ino_u8)(x >> 16);
      if( j > 1 ) *p++ = (ino_u8)(x >>  8);
      if( j > 2 ) *p++ = (ino_u8)(x      );
    }
  }

  *dst_size = p - dst;

  return BASE64_ERROR_NONE;
}

INO_NAMESPACE_END
