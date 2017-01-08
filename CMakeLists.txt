cmake_minimum_required(VERSION 2.8)

project(dfuse_pack)

set(CMAKE_C_FLAGS "-fdata-sections -ffunction-sections ${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS "-std=gnu11 -Wextra -Wno-unused-parameter -ffast-math -fvisibility=hidden ${CMAKE_C_FLAGS}")
set(CMAKE_C_FLAGS "-Wshadow -Wimplicit-function-declaration -Wredundant-decls -Wmissing-prototypes -Wstrict-prototypes ${CMAKE_C_FLAGS}")

include_directories("${PROJECT_SOURCE_DIR}")

# dfuse_pack
add_executable(dfuse_pack dfuse_pack.c)
install(TARGETS dfuse_pack DESTINATION bin)
