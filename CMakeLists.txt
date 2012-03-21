cmake_minimum_required(VERSION 2.8)

project(paste-binouse C CXX)

enable_testing()

find_package(GnuTLS REQUIRED)

add_definitions(
  -std=c++0x
  -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -DGOOGLE_PROTOBUF_NO_RTTI
  -Wall -Wextra -Werror=return-type
  -fsplit-stack
  ${GNUTLS_DEFINITIONS})

set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -std=c99")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_subdirectory(src)

install(FILES scripts/paste-binouse-init
  DESTINATION bin
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

install(DIRECTORY www
  DESTINATION share/paste-binouse)

install(DIRECTORY sql
  DESTINATION share/paste-binouse)
