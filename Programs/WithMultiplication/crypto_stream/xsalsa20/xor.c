/*
version 20080913
D. J. Bernstein
Public domain.
*/

#include "../../includes/lib.h"

static const unsigned char sigma[16] = "expand 32-byte k";

int crypto_stream_xsalsa20_xor(
        unsigned char *c,
  const unsigned char *m,unsigned long long mlen,
  const unsigned char *n,
  const unsigned char *k
)
{
  unsigned char subkey[32];
  crypto_core_hsalsa20(subkey,n,k,sigma);
  return crypto_stream_salsa20_xor(c,m,mlen,n + 16,subkey);
}
