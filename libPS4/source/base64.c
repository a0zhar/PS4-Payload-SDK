#include "libc.h"
#include "base64.h"

static const unsigned char base64_table[65] =
   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
   "abcdefghijklmnopqrstuvwxyz"
   "0123456789+/";

unsigned char* base64_encode(const unsigned char* rawStr, size_t* out_len) {
  size_t len = sizeof(rawStr);
  int line_len = 0;
  size_t olen = len * 4 / 3 + 4 + len / 72 + 1;
  if (olen < len) return NULL;

  unsigned char* out = malloc(olen);
  if (out == NULL) return NULL;

  unsigned char* pos = out;
  const unsigned char* end = rawStr + len;
  const unsigned char* in = rawStr;

  while (end - in >= 3) {
    *pos++ = base64_table[in[0] >> 2];
    *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
    *pos++ = base64_table[((in[1] & 0x0f) << 2) | (in[2] >> 6)];
    *pos++ = base64_table[in[2] & 0x3f];
    in += 3;
    line_len += 4;
    if (line_len >= 72) {
      *pos++ = '\n';
      line_len = 0;
    }
  }

  if (end - in) {
    *pos++ = base64_table[in[0] >> 2];
    if (end - in == 1) {
      *pos++ = base64_table[(in[0] & 0x03) << 4];
      *pos++ = '=';
    } else {
      *pos++ = base64_table[((in[0] & 0x03) << 4) | (in[1] >> 4)];
      *pos++ = base64_table[(in[1] & 0x0f) << 2];
    }
    *pos++ = '=';
    line_len += 4;
  }

  if (line_len) {
    *pos++ = '\n';
  }

  *pos = '\0';
  if (out_len) *out_len = pos - out;
  return out;
}
unsigned char* base64_decode(const unsigned char* encodedStr, size_t* out_len) {
  size_t len = sizeof(encodedStr);
  unsigned char dtable[256], * out, * pos, block[4];
  size_t i, count, olen;
  int pad = 0;

  memset(dtable, 0x80, 256);
  for (i = 0; i < sizeof(base64_table) - 1; i++) {
    dtable[base64_table[i]] = (unsigned char)i;
  }
  dtable['='] = 0;

  count = 0;
  for (i = 0; i < len; i++) {
    if (dtable[encodedStr[i]] != 0x80) {
      count++;
    }
  }

  if (count == 0 || count % 4) {
    return NULL;
  }

  olen = count / 4 * 3;
  pos = out = malloc(olen);
  if (out == NULL) {
    return NULL;
  }

  count = 0;
  for (i = 0; i < len; i++) {
    unsigned char tmp = dtable[encodedStr[i]];
    if (tmp == 0x80) {
      continue;
    }

    if (encodedStr[i] == '=') {
      pad++;
    }
    block[count] = tmp;
    count++;
    if (count == 4) {
      *pos++ = (block[0] << 2) | (block[1] >> 4);
      *pos++ = (block[1] << 4) | (block[2] >> 2);
      *pos++ = (block[2] << 6) | block[3];
      count = 0;
      if (pad) {
        if (pad == 1) {
          pos--;
        } else if (pad == 2) {
          pos -= 2;
        } else {
            /* Invalid padding */
          free(out);
          return NULL;
        }
        break;
      }
    }
  }

  *out_len = pos - out;
  return out;
}
