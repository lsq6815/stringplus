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

# Export compile commands for clang-tidy
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

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

# clang-tidy integration (must be before add_library/add_executable)
find_program(CLANG_TIDY_EXE NAMES clang-tidy)
if(CLANG_TIDY_EXE)
    set(CLANG_TIDY_EXTRA_ARGS "")
    if(WIN32 AND CMAKE_C_COMPILER_ID MATCHES "GNU")
        # 帮助 clang-tidy 找到 MinGW 系统头文件
        get_filename_component(COMPILER_DIR "${CMAKE_C_COMPILER}" DIRECTORY)
        get_filename_component(COMPILER_ROOT "${COMPILER_DIR}" DIRECTORY)
        set(MINGW_SYS_INCLUDE_CANDIDATES
            "${COMPILER_ROOT}/x86_64-w64-mingw32/include"
            "${COMPILER_ROOT}/i686-w64-mingw32/include"
            "${COMPILER_ROOT}/mingw32/include"
            "${COMPILER_ROOT}/include"
        )
        foreach(candidate ${MINGW_SYS_INCLUDE_CANDIDATES})
            if(EXISTS "${candidate}/stdio.h")
                list(APPEND CLANG_TIDY_EXTRA_ARGS "--extra-arg=-isystem${candidate}")
                break()
            endif()
        endforeach()
        # 获取 GCC target triplet 并传给 clang-tidy，确保正确解析 MinGW 头文件
        execute_process(
            COMMAND ${CMAKE_C_COMPILER} -dumpmachine
            OUTPUT_VARIABLE GCC_TARGET
            OUTPUT_STRIP_TRAILING_WHITESPACE
        )
        if(GCC_TARGET)
            list(APPEND CLANG_TIDY_EXTRA_ARGS "--extra-arg=--target=${GCC_TARGET}")
        endif()
    endif()
    # 设置编译时自动运行 clang-tidy
    set(CMAKE_C_CLANG_TIDY "${CLANG_TIDY_EXE}" ${CLANG_TIDY_EXTRA_ARGS})
    message(STATUS "clang-tidy found: ${CLANG_TIDY_EXE}")
else()
    message(STATUS "clang-tidy not found. Skipping clang-tidy checks.")
endif()

set (LIB_SRCS ./src/stringplus.c ./src/ccary.c)

option(BUILD_SHARED_LIBS "Build stringplus as a shared library" OFF)

if(BUILD_SHARED_LIBS)
    add_library (stringplus SHARED ${LIB_SRCS})
else()
    add_library (stringplus STATIC ${LIB_SRCS})
endif()

set_target_properties (stringplus PROPERTIES PUBLIC_HEADER "./include/stringplus.h;./include/ccary.h")

install (
        TARGETS stringplus
        ARCHIVE DESTINATION "lib"
        LIBRARY DESTINATION "lib"
        RUNTIME DESTINATION "bin"
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

# Lint target (runs Doxygen and clang-tidy)
set(LINT_COMMANDS)
set(LINT_COMMENTS)

if(DOXYGEN_FOUND)
    list(APPEND LINT_COMMANDS
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
    )
    list(APPEND LINT_COMMENTS "Doxygen")
endif()

if(CLANG_TIDY_EXE)
    list(APPEND LINT_COMMANDS
        COMMAND ${CLANG_TIDY_EXE}
            -p ${CMAKE_BINARY_DIR}
            ${CLANG_TIDY_EXTRA_ARGS}
            ${CMAKE_SOURCE_DIR}/src/*.c
            ${CMAKE_SOURCE_DIR}/include/*.h
    )
    list(APPEND LINT_COMMENTS "clang-tidy")
endif()

if(LINT_COMMANDS)
    set(LINT_COMMENT_STRING "")
    foreach(comment ${LINT_COMMENTS})
        if(LINT_COMMENT_STRING)
            set(LINT_COMMENT_STRING "${LINT_COMMENT_STRING} + ${comment}")
        else()
            set(LINT_COMMENT_STRING "${comment}")
        endif()
    endforeach()
    add_custom_target(lint
        ${LINT_COMMANDS}
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        COMMENT "Running ${LINT_COMMENT_STRING}..."
        VERBATIM
    )
endif()
