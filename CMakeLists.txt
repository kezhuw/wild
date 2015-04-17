cmake_minimum_required(VERSION 3.0)
project(wild CXX)

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
-g
-Wall
-Wextra
-Werror
-Wconversion
-Wshadow
)

if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    list(INSERT CXX_FLAGS 1 -stdlib=libc++)
endif()

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CXX_FLAGS}")

if(POLICY CMP0042)
    cmake_policy(SET CMP0042 NEW)
endif()

add_library(wild_shared SHARED ${wild_SRCS})
add_library(wild_static STATIC ${wild_SRCS})
set_target_properties(wild_shared PROPERTIES OUTPUT_NAME wild)
set_target_properties(wild_static PROPERTIES OUTPUT_NAME wild)

if(CMAKE_SYSTEM_NAME STREQUAL "FreeBSD")
    target_link_libraries(wild_shared execinfo)
    target_link_libraries(wild_static execinfo)
endif()

set(INCLUDE_INSTALL_DIR include)
set(LIBRARY_INSTALL_DIR lib)

install(TARGETS wild_shared wild_static EXPORT wild-targets DESTINATION ${LIBRARY_INSTALL_DIR})
install(FILES ${wild_INCS} DESTINATION ${INCLUDE_INSTALL_DIR}/wild)

# create package config {

set(PACKAGE_INSTALL_DIR lib/wild)
export(EXPORT wild-targets FILE ${CMAKE_CURRENT_BINARY_DIR}/wild-targets.cmake)
install(EXPORT wild-targets DESTINATION ${PACKAGE_INSTALL_DIR})

include(CMakePackageConfigHelpers)
configure_package_config_file(wild-config.cmake.in ${CMAKE_CURRENT_BINARY_DIR}/wild-config.cmake
    INSTALL_DESTINATION ${PACKAGE_INSTALL_DIR}
    PATH_VARS INCLUDE_INSTALL_DIR LIBRARY_INSTALL_DIR)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/wild-config.cmake DESTINATION ${PACKAGE_INSTALL_DIR})

# }
