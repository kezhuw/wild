#include "adler32.hpp"

namespace wild {
namespace adler32 {

uint32
Calculate(byte_t const* bytes, size_t len, uint32 startingChecksum) {
    static const uint32 mod = 65521;
    static const uint32 max = 5552;

    uint32 a = startingChecksum & 0xFFFF;
    uint32 b = startingChecksum >> 16;
    while (len > 0) {
        size_t n = len;
        if (len > max) {
            n = max;
        }
        for (size_t i=0; i<n; ++i) {
            a += (uint32)bytes[i];
            b += a;
        }
        a %= mod;
        b %= mod;
        bytes += n;
        len -= n;
    }
    return (b<<16) | a;
}

}
}
