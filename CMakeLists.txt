# without obj file(s)
# cmake_minimum_required (VERSION 3.10.0)
# project (stringplus)
# include_directories (./include)
# file (GLOB SOURCES "./src/*.c")
# add_executable (app ${SOURCES})

# with obj file(s)
cmake_minimum_required (VERSION 3.10.0)
project (stringplus)

# Set C standard
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Werror
        -pedantic
        -Wstrict-prototypes
        -Wmissing-prototypes
        -Wold-style-definition
        -Wshadow
        -Wpointer-arith
        -Wcast-qual
    )
endif()

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

# Testing
enable_testing()
add_subdirectory(tests)

# Documentation generation with Doxygen
find_package(Doxygen)
if(DOXYGEN_FOUND)
    configure_file(
        ${CMAKE_SOURCE_DIR}/docs/Doxyfile.in
        ${CMAKE_BINARY_DIR}/Doxyfile
        @ONLY
    )
    add_custom_target(docs
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
else()
    message(STATUS "Doxygen not found. Documentation will not be generated.")
endif()

# clang-tidy integration
find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    set(CMAKE_C_CLANG_TIDY "${CLANG_TIDY_EXE}")
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
else()
    message(STATUS "clang-tidy not found. Skipping clang-tidy checks.")
endif()

# Lint target (runs clang-tidy manually)
if(CLANG_TIDY_EXE)
    add_custom_target(lint
        COMMAND ${CLANG_TIDY_EXE}
            ${CMAKE_SOURCE_DIR}/src/*.c
            ${CMAKE_SOURCE_DIR}/include/*.h
            --
            -I${CMAKE_SOURCE_DIR}/include
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running clang-tidy..."
    )
endif()
