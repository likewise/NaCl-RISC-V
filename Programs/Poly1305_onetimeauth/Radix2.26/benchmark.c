#include "benchmark.h"

    void dobenchmark(uint64_t *timings, unsigned char a[16]) {
        static unsigned char c[150] = {0x26, 0x48, 0xe4, 0x20, 0x66, 0xa, 0x53, 0x80, 0x3b, 0x30, 0xd0, 0x76, 0x8, 0x8e, 0x7e, 0xd0, 0x85, 0xdf, 0xae, 0x46, 0xe2, 0x2b, 0x8a, 0x82, 0x46, 0x81, 0x82, 0x66, 0x7a, 0x3f, 0x92, 0xc4, 0x17, 0xbb, 0xd, 0x81, 0x1d, 0x2b, 0x93, 0x49, 0xad, 0x4c, 0xb5, 0x9a, 0xbd, 0x3b, 0xd4, 0xc0, 0x37, 0xa8, 0x77, 0xee, 0x9e, 0x3d, 0x21, 0x5b, 0xe7, 0x73, 0x67, 0x10, 0x23, 0xfe, 0x86, 0x2b, 0xb4, 0x0, 0x67, 0x23, 0xd4, 0x28, 0x68, 0xd5, 0x21, 0x5, 0xa8, 0x35, 0xb6, 0x37, 0x6b, 0x3f, 0x13, 0xb0, 0xb0, 0xaa, 0x13, 0x2e, 0xb4, 0x64, 0x71, 0x29, 0xc6, 0xac, 0x37, 0xc3, 0x28, 0x6e, 0xe4, 0x17, 0xe2, 0x13, 0xf6, 0xa2, 0xaf, 0x64, 0xcc, 0x12, 0x7b, 0x5c, 0x1b, 0xc4, 0xb2, 0x67, 0xf7, 0x1b, 0xf0, 0x74, 0x6, 0xf6, 0x93, 0xbb, 0x6e, 0xa8, 0xf5, 0xe3, 0xb2, 0x87, 0x1a, 0x87, 0x81, 0xc5, 0xa2, 0x8a, 0x7c, 0x23, 0x89, 0xf7, 0x29, 0x39, 0xa2, 0x53, 0xa6, 0xbe, 0xb, 0x43, 0xd8, 0x95, 0x1d, 0x71, 0x33, 0x7e};
        static unsigned char rs[32] = {0x23, 0xb2, 0xdd, 0x2f, 0x25, 0x79, 0x6d, 0x4f, 0x9d, 0x29, 0x75, 0xd3, 0xf4, 0xfa, 0x41, 0xa1, 0x30, 0x1f, 0xb7, 0x28, 0x33, 0x3c, 0x45, 0xaa, 0xa1, 0xc9, 0x4f, 0x8e, 0xb8, 0xe0, 0xac, 0xa7};

        uint32_t oldcount, newcount;
        unsigned char x = 5, y = 10;
        oldcount = getcycles();
        crypto_onetimeauth(a, c, 150, rs);
        newcount = getcycles();
        timings[0] = newcount - oldcount;
    }
