cmake_minimum_required(VERSION 2.8)

project(dfuse_pack)

set(LIBS "")

# pkg-config
find_package(PkgConfig REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}")

# eina
pkg_search_module(EINA REQUIRED eina)
include_directories(${EINA_INCLUDE_DIRS})
set(LIBS ${LIBS} ${EINA_LDFLAGS})

# dfuse_pack
add_executable(dfuse_pack dfuse_pack.c)
target_link_libraries(dfuse_pack ${LIBS})
install(TARGETS dfuse_pack DESTINATION bin)
