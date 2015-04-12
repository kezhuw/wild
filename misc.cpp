#include "misc.hpp"

#include <execinfo.h>

#include <stdlib.h>

#if defined(__FreeBSD__)
using backtrace_len_t = size_t;
#else
using backtrace_len_t = int;
#endif

namespace wild {

string
StackTrace(uintreg level) {
    void *stacks[128];
    backtrace_len_t n = ::backtrace(stacks, static_cast<backtrace_len_t>(nelem(stacks)));
    char **symbols = ::backtrace_symbols(stacks, n);
    if (symbols == NULL) {
        return "(nil stack)\n";
    }
    string stackString;
    for (uintreg i=level; i<(uintreg)n; ++i) {
        stackString.append(symbols[i]);
        stackString.push_back('\n');
    }
    free(symbols);
    return stackString;
}

}
