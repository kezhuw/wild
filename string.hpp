#pragma once

namespace wild {

namespace os {

// Thread-safe strerror(3).
const char *strerror(int errnum);

}

}
