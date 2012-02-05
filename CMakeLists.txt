cmake_minimum_required(VERSION 2.8)

project(paste-binouse C CXX)

enable_testing()

find_package(GnuTLS REQUIRED)

add_definitions(
  -std=c++0x
  -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -DGOOGLE_PROTOBUF_NO_RTTI
  -Wall -Wextra -Werror=return-type
  -fsplit-stack -fno-exceptions -fno-rtti
  ${GNUTLS_DEFINITIONS})

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_subdirectory(src)
