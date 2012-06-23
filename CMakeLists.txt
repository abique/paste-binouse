cmake_minimum_required(VERSION 2.8)

project(paste-binouse C CXX)

enable_testing()

find_package(GnuTLS REQUIRED)

add_definitions(
  -std=c++0x
  -D_GNU_SOURCE -D_LARGEFILE64_SOURCE -DGOOGLE_PROTOBUF_NO_RTTI
  -Wall -Wextra -Werror=return-type
  ${GNUTLS_DEFINITIONS})

add_subdirectory(mimosa EXCLUDE_FROM_ALL)
include_directories(mimosa)
add_subdirectory(src)

install(FILES scripts/paste-binouse-init
  DESTINATION bin
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE)

install(DIRECTORY www
  DESTINATION share/paste-binouse)

install(DIRECTORY sql
  DESTINATION share/paste-binouse)
