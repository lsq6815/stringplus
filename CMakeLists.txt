# without obj file(s)
# cmake_minimum_required (VERSION 3.10.0)
# project (stringplus)
# include_directories (./include)
# file (GLOB SOURCES "./src/*.c")
# add_executable (app ${SOURCES})

# with obj file(s)
cmake_minimum_required (VERSION 3.10.0)
project (stringplus)

include_directories (./include)

set (MAIN_SRCS ./src/main.c)
set (LIB_SRCS ./src/stringplus.c ./src/ccary.c)

add_library (stringplus STATIC ${LIB_SRCS})
set_target_properties (stringplus PROPERTIES PUBLIC_HEADER "./include/stringplus.h;./include/ccary.h")
add_executable (app ${MAIN_SRCS})
target_link_libraries (app stringplus)
install (
        TARGETS stringplus
        ARCHIVE DESTINATION "lib"
        PUBLIC_HEADER DESTINATION  "include"
    )
