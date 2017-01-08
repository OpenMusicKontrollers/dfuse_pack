cmake_minimum_required(VERSION 2.8)

project(dfuse_pack)

include_directories("${PROJECT_SOURCE_DIR}")

# dfuse_pack
add_executable(dfuse_pack dfuse_pack.c)
install(TARGETS dfuse_pack DESTINATION bin)
