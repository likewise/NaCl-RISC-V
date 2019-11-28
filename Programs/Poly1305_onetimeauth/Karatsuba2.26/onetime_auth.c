#include <stdint.h>
#include <stdio.h>

extern uint64_t getcycles();
// add the two numbers together without reduction
static void add(unsigned int h[17], const unsigned int c[17]) {
  unsigned int j;
  unsigned int u;
  u = 0;
  for (j = 0; j < 17; ++j) {
    u += h[j] + c[j];
    h[j] = u & 255;
    u >>= 8;
  }
}

static void add226(unsigned int h[5], const unsigned int c[5]) {

  unsigned int j;
  unsigned int u;
  u = 0;
  for (j = 0; j < 5; ++j) {
    u += h[j] + c[j];
    h[j] = u & 0x3FFFFFF;
    u >>= 26;
  }
  h[4] += u << 26;
}

// used in mulmod
static void squeeze226(unsigned int h[6]) {
  unsigned int j;
  uint64_t u;
  u = h[5] * 5;
  for (j = 0; j < 4; ++j) {
    u += h[j];
    h[j] = u & 0x3FFFFFF;
    u >>= 26;
  }
  u += h[4];
  h[4] = u & 0x3FFFFFF;
  u >>= 26;
  u *= 5;
  for (j = 0; j < 4; ++j) {
    u += h[j];
    h[j] = u & 0x3FFFFFF;
    u >>= 26;
  }
  u += h[4];
  h[4] = u;
}

void karatsuba226_2(unsigned int out[4], unsigned int a[2],
                    const unsigned int b[2]) {
  int64_t l = (int64_t)a[0] * b[0];
  unsigned int l_low = l & 0x3FFFFFF;
  unsigned int l_high = l >> 26;

  int64_t h = (int64_t)a[1] * b[1];
  unsigned int h_low = h & 0x3FFFFFF;
  unsigned int h_high = h >> 26;

  int m_a = a[0] - a[1];
  int m_b = b[0] - b[1];
  int64_t m = (int64_t)m_a * m_b;
  int m_low = m & 0x3ffffff;
  int m_high = m >> 26;

  out[0] = l_low;
  out[1] = l_low + l_high + h_low - m_low;
  out[2] = l_high + h_high + h_low - m_high;
  out[3] = h_high;
  out[2] += out[1] >> 26;
  out[1] &= 0x3FFFFFF;
  out[3] += out[2] >> 26;
  out[2] &= 0x3FFFFFF;
}

void karatsuba226_3(unsigned int out[5], unsigned int a[3], unsigned int b[3]) {
  unsigned int x[4];
  unsigned int m[4];

  int64_t h = (int64_t)a[2] * b[2];
  unsigned int h_low = h & 0x3ffffff;
  unsigned int h_high = h >> 26;

  karatsuba226_2(x, a, b);

  int64_t m_a = (int64_t)a[0];
  int64_t m_b = (int64_t)b[0];
  m_a += (int64_t)a[1] << 26;
  m_b += (int64_t)b[1] << 26;
  m_a -= a[2];
  m_b -= b[2];
  unsigned int m_ai[2] = {m_a & 0x3ffffff, m_a >> 26};
  unsigned int m_bi[2] = {m_b & 0x3ffffff, m_b >> 26};
  karatsuba226_2(m, m_ai, m_bi);

  out[0] = x[0] + 5 * (x[3] - m[3] + h_high);
  out[1] = x[1];
  out[2] = x[0] + x[2] - m[0] + h_low;
  out[3] = x[1] + x[3] - m[1] + h_high;
  out[4] = x[2] - m[2] + h_low;
  out[0] += (out[4]>>26)*5;
  unsigned int zeros[5] = {0};
  add226(out, zeros);
}

static const unsigned int minusp[17] = {5, 0, 0, 0, 0, 0, 0, 0,  0,
                                        0, 0, 0, 0, 0, 0, 0, 252};

// reduce the number from 2^133 to 2^130-5
static void freeze(unsigned int h[17]) {
  unsigned int horig[17];
  unsigned int j;
  unsigned int negative;
  for (j = 0; j < 17; ++j)
    horig[j] = h[j];
  add(h, minusp);
  negative = -(h[16] >> 7);
  for (j = 0; j < 17; ++j)
    h[j] ^= negative & (horig[j] ^ h[j]);
}

static void mulmod226(unsigned int h[6], const unsigned int r[5]) {
  unsigned int hr[6];
  unsigned int i;
  unsigned int j;
  int64_t u = 0;

  for (i = 0; i < 5; ++i) {
    for (j = 0; j <= i; ++j) {
      int64_t tmp = (int64_t)h[j] * r[i - j];
      u += tmp;
    }
    for (j = i + 1; j < 5; ++j) {
      int64_t tmp = (int64_t)h[j] * r[i + 5 - j];
      tmp *= 5;
      u += tmp;
    }

    hr[i] = u & 0x3FFFFFF;
    u >>= 26;
  }
  hr[5] = u;
  for (i = 0; i < 6; ++i)
    h[i] = hr[i];
  squeeze226(h);
}

void toradix28(unsigned int h[17]) {
  h[16] = (h[4] >> 24);
  h[15] = (h[4] >> 16) & 0xFF;
  h[14] = (h[4] >> 8) & 0xFF;
  h[13] = h[4] & 0xFF;
  h[12] = (h[3] >> 18) & 0xFF;
  h[11] = (h[3] >> 10) & 0xFF;
  h[10] = (h[3] >> 2) & 0xFF;
  h[9] = (h[2] >> 20) + ((h[3] & 3) << 6);
  h[8] = (h[2] >> 12) & 0xFF;
  h[7] = (h[2] >> 4) & 0xFF;
  h[6] = (h[1] >> 22) + ((h[2] & 0x0F) << 4);
  h[5] = (h[1] >> 14) & 0xFF;
  h[4] = (h[1] >> 6) & 0xFF;
  h[3] = (h[0] >> 24) + ((h[1] & 0x3f) << 2);
  h[2] = (h[0] >> 16) & 0xFF;
  h[1] = (h[0] >> 8) & 0xFF;
  h[0] = h[0] & 0xFF;
}

// input is in little endian
int crypto_onetimeauth(unsigned char *out, const unsigned char *in,
                       unsigned long long inlen, const unsigned char *k) {
  unsigned int j;
  unsigned int r[5];
  unsigned int h[17];
  unsigned int c[17];

  // create R from the first 16 bytes of the key
  r[0] = k[0] + (k[1] << 8) + (k[2] << 16) + ((k[3] & 3) << 24);
  r[1] = ((k[3] >> 2) & 3) + ((k[4] & 252) << 6) + (k[5] << 14) +
         ((k[6] & 15) << 22);
  r[2] = (k[6] >> 4) + ((k[7] & 15) << 4) + ((k[8] & 252) << 12) +
         ((k[9] & 63) << 20);
  r[3] =
      (k[9] >> 6) + (k[10] << 2) + ((k[11] & 15) << 10) + ((k[12] & 252) << 18);
  r[4] = k[13] + (k[14] << 8) + ((k[15] & 15) << 16);

  // set the state to 0
  for (j = 0; j < 17; ++j)
    h[j] = 0;

  while (inlen > 0) {
    for (j = 0; j < 17; ++j)
      c[j] = 0; // set c to 0
    int index = 0;
    int bitleft = 26;
    for (j = 0; (j < 16) && (j < inlen); ++j) {
      if (bitleft < 8) {
        int tmp = ((1 << bitleft) - 1);
        c[index] += (in[j] & tmp) << (26 - bitleft);
        index++;
        c[index] += in[j] >> bitleft;
        bitleft = 26 - (8 - bitleft);
      } else {
        c[index] +=
            in[j]
            << (26 -
                bitleft); // fill c with a chunk of 16 bytes from the in param
        bitleft -= 8;
      }
    }
    if (bitleft == 0) {
      index++;
      bitleft = 26;
    }
    c[index] += 1 << (26 - bitleft);

    in += 16;
    inlen -= j;      // update loop variants (inlen and increment in pointer)
    add226(h, c);    // c to the state
    mulmod226(h, r); // multiply state with the secret key modulo 2^130-5
  }

  // go back to radix 2.8
  toradix28(h);

  freeze(h); // calculate mod 2^130-5

  for (j = 0; j < 16; ++j)
    c[j] = k[j + 16];
  c[16] = 0;
  add(h, c); // add S to the state (which is the last 16 bytes of the key)
  for (j = 0; j < 16; ++j)
    out[j] = h[j]; // output the state modulo 2^128 (the last 16 bytes)
  return 0;
}