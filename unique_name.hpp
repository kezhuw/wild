#pragma once

#define _CONCAT_(name, mark, value)     name ## mark ## value
#define _CONCAT(name, mark, value)      _CONCAT_(name, mark, value)

#define _UNUSED_NAME_(name)     name ## _
#define _UNUSED_NAME(name)      _UNUSED_NAME_(name)

#define UNIQUE_NAME(name)       _UNUSED_NAME(_CONCAT(_CONCAT(name, _line, __LINE__), _counter, __COUNTER__))
