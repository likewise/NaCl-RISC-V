/*
version 20080912
D. J. Bernstein
Public domain.
*/

#define ROUNDS 20

typedef unsigned int uint32;

static uint32 rotate(uint32 u,int c)
{
  return (u << c) | (u >> (32 - c));
}

static uint32 load_littleendian(const unsigned char *x)
{
  return
      (uint32) (x[0]) \
  | (((uint32) (x[1])) << 8) \
  | (((uint32) (x[2])) << 16) \
  | (((uint32) (x[3])) << 24)
  ;
}

static void store_littleendian(unsigned char *x,uint32 u)
{
  x[0] = u; u >>= 8;
  x[1] = u; u >>= 8;
  x[2] = u; u >>= 8;
  x[3] = u;
}

int crypto_core(
        unsigned char *out,
  const unsigned char *in,
  const unsigned char *k,
  const unsigned char *c
)
{
  uint32 x0, x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, x11, x12, x13, x14, x15;
  int i;

  x0 = load_littleendian(c + 0);
  x1 = load_littleendian(k + 0);
  x2 = load_littleendian(k + 4);
  x3 = load_littleendian(k + 8);
  x4 = load_littleendian(k + 12);
  x5 = load_littleendian(c + 4);
  x6 = load_littleendian(in + 0);
  x7 = load_littleendian(in + 4);
  x8 = load_littleendian(in + 8);
  x9 = load_littleendian(in + 12);
  x10 = load_littleendian(c + 8);
  x11 = load_littleendian(k + 16);
  x12 = load_littleendian(k + 20);
  x13 = load_littleendian(k + 24);
  x14 = load_littleendian(k + 28);
  x15 = load_littleendian(c + 12);

  for (i = ROUNDS;i > 0;i -= 2) {
     x4 ^= rotate( x0+x12, 7);
     x8 ^= rotate( x4+ x0, 9);
    x12 ^= rotate( x8+ x4,13);
     x0 ^= rotate(x12+ x8,18);
     x9 ^= rotate( x5+ x1, 7);
    x13 ^= rotate( x9+ x5, 9);
     x1 ^= rotate(x13+ x9,13);
     x5 ^= rotate( x1+x13,18);
    x14 ^= rotate(x10+ x6, 7);
     x2 ^= rotate(x14+x10, 9);
     x6 ^= rotate( x2+x14,13);
    x10 ^= rotate( x6+ x2,18);
     x3 ^= rotate(x15+x11, 7);
     x7 ^= rotate( x3+x15, 9);
    x11 ^= rotate( x7+ x3,13);
    x15 ^= rotate(x11+ x7,18);
     x1 ^= rotate( x0+ x3, 7);
     x2 ^= rotate( x1+ x0, 9);
     x3 ^= rotate( x2+ x1,13);
     x0 ^= rotate( x3+ x2,18);
     x6 ^= rotate( x5+ x4, 7);
     x7 ^= rotate( x6+ x5, 9);
     x4 ^= rotate( x7+ x6,13);
     x5 ^= rotate( x4+ x7,18);
    x11 ^= rotate(x10+ x9, 7);
     x8 ^= rotate(x11+x10, 9);
     x9 ^= rotate( x8+x11,13);
    x10 ^= rotate( x9+ x8,18);
    x12 ^= rotate(x15+x14, 7);
    x13 ^= rotate(x12+x15, 9);
    x14 ^= rotate(x13+x12,13);
    x15 ^= rotate(x14+x13,18);
  }

  store_littleendian(out + 0,x0);
  store_littleendian(out + 4,x5);
  store_littleendian(out + 8,x10);
  store_littleendian(out + 12,x15);
  store_littleendian(out + 16,x6);
  store_littleendian(out + 20,x7);
  store_littleendian(out + 24,x8);
  store_littleendian(out + 28,x9);

  return 0;
}
