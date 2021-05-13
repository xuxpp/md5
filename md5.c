#include "md5.h"

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

#define LEFTROTATE(x, c) (((x) << (c)) | ((x) >> (32 - (c))))

static uint32_t shifts[] =
{
    7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
    5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20, 5,  9, 14, 20,
    4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
    6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21
};

static uint32_t sines[] =
{
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
    0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
    0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
    0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
    0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
    0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
    0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
    0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
    0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
    0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
    0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

void md5(const uint8_t *buf, size_t len, uint32_t *result)
{
    // Pre proc
    size_t pad_len = (len % 64) < 56 ? 64 - (len % 64) : 128 - (len % 64);
    size_t new_len = len + pad_len;
    uint8_t *data = calloc(1, new_len);
    memcpy(data, buf, len);
    // "Pad" 1
    data[len] = 0x80;
    // "Pad" original len (num bits) in the last 64 bits
    uint64_t *last_dw = (uint64_t *)&data[new_len - 8];
    *last_dw = len * 8;

    // Process 512-bit (64 bytes, 16 uint32_t) chunk
    size_t num_chunks = new_len / 64;
    uint32_t h0 = 0x67452301;
    uint32_t h1 = 0xefcdab89;
    uint32_t h2 = 0x98badcfe;
    uint32_t h3 = 0x10325476;

    for (size_t chunk_i = 0; chunk_i < num_chunks; chunk_i++)
    {
        uint32_t *w = (uint32_t *)&data[chunk_i * 64];
        uint32_t a = h0;
        uint32_t b = h1;
        uint32_t c = h2;
        uint32_t d = h3;
        for (size_t i = 0; i < 64; i++)
        {
            uint32_t f = 0;
            uint32_t g = 0;
            switch (i / 16)
            {
                case 0:
                {
                    f = (b & c) | (~b & d);
                    g = i;
                } break;
                case 1:
                {
                    f = (d & b) | (~d & c);
                    g = (5 * i + 1) % 16;
                } break;
                case 2:
                {
                    f = b ^ c ^ d;
                    g = (3 * i + 5) % 16;
                } break;
                case 3:
                {
                    f = c ^ (b | ~d);
                    g = (7 * i) % 16;
                } break;
                default: abort();
            }
            f += a + sines[i] + w[g];
            a = d;
            d = c;
            c = b;
            b = b + LEFTROTATE(f, shifts[i]);
        }
        h0 += a;
        h1 += b;
        h2 += c;
        h3 += d;
    }
    // Store hash
    result[0] = h0;
    result[1] = h1;
    result[2] = h2;
    result[3] = h3;

    free(data);
}