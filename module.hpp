#ifndef __WILD_MODULE_HPP_
#define __WILD_MODULE_HPP_

#include <functional>

namespace wild { namespace module {

typedef std::function<int()> InitFunction;

class Definition {
public:

    Definition(const char *name, InitFunction init, int order = 0)
        : Definition(0, name, init, order) {}

    Definition(int seq, const char *name, InitFunction init, int order = 0);

    const char *Name() const;
};

void Init();

}}

#endif
