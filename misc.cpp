#include "misc.hpp"

#include <execinfo.h>

#include <stdlib.h>

namespace wild {

string
StackTrace(uintreg level) {
    void *stacks[128];
    int n = ::backtrace(stacks, static_cast<int>(nelem(stacks)));
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
