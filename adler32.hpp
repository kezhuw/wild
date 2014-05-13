#ifndef __WILD_ADLER32_HPP_
#define __WILD_ADLER32_HPP_

#include "types.hpp"

namespace wild {
namespace adler32 {

uint32 Calculate(byte_t const* bytes, size_t len, uint32 startingChecksum = 1);

class Checksum {
public:
    explicit Checksum(uint32 value = 1) : _value(value) {}

    Checksum(byte_t const* bytes, size_t len)
        : Checksum() {
        Update(bytes, len);
    }

    void Update(byte_t const* bytes, size_t len) {
        _value = Calculate(bytes, len, _value);
    }


    uint32 Value() const { return _value; }

private:
    uint32 _value;
};

}
}

#endif
