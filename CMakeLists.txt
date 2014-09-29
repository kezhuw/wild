cmake_minimum_required(VERSION 2.6)
project(wild)

set(wild_SRCS
adler32.cpp
exception.cpp
misc.cpp
module.cpp
string.cpp
io.cpp
)

set(wild_INCS
fd.hpp
io.hpp
id_allocator.hpp
types.hpp
errno.hpp
adler32.hpp
exception.hpp
likely.hpp
misc.hpp
module.hpp
module_init_failed.hpp
scope_guard.hpp
string.hpp
unique_name.hpp
spinlock.hpp
with_lock.hpp
forward_list.hpp
freelist.hpp
any.hpp
)

set(CXX_FLAGS
-std=c++1y
-stdlib=libc++
-g
-Wall
-Wextra
-Werror
-Wconversion
-Wshadow
)

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

add_library(wild_shared SHARED ${wild_SRCS})
add_library(wild_static STATIC ${wild_SRCS})
set_target_properties(wild_shared PROPERTIES OUTPUT_NAME wild)
set_target_properties(wild_static PROPERTIES OUTPUT_NAME wild)

install(TARGETS wild_shared wild_static DESTINATION lib)
install(FILES ${wild_INCS} DESTINATION include/wild)
