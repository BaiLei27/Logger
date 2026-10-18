cmake_minimum_required(VERSION 3.25...3.27)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_COLOR_DIAGNOSTICS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(Logger
    VERSION "0.0.2"
    HOMEPAGE_URL "https://github.com/Baiyi27/Logger.git"
    LANGUAGES CXX
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT _ipo_supported)

if(_ipo_supported)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        add_compile_options(-flto=full)
        add_link_options(-flto=full -fuse-ld=lld)
    else()
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    endif()
endif()

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
link_libraries(Threads::Threads)

add_compile_options(
    -ffunction-sections
    -fdata-sections
    -funroll-loops
    -fomit-frame-pointer
    "$<$<CONFIG:Debug>:-O0;-ggdb3;-DDEBUG_>"
    $<$<CONFIG:Release,MinSizeRel>:-DNDEBUG_>
)

# Independent of DEBUG / NDEBUG / the parent project's DEBUG_.
option(enableLogger "Compile Logger calls and output" ON)
add_compile_definitions(LOGGER_ENABLED_=$<BOOL:${enableLogger}>)

if(CMAKE_SYSTEM_PROCESSOR MATCHES "^(x86_|amd|AMD)64$")
    add_compile_options(
        -march=native
        -mtune=native
        -mprefer-vector-width=256
    )
endif()

add_link_options(
    -Wl,--gc-sections
    -lm
    $<$<CONFIG:Release,MinSizeRel>:-s>
)
add_library(${PROJECT_NAME} SHARED)
add_executable(
    ${PROJECT_NAME}_test
    test.cc
)
set_target_properties(${PROJECT_NAME}_test PROPERTIES
    SUFFIX ".exe"
)

set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION ${PROJECT_VERSION_MAJOR}
)
target_link_libraries(${PROJECT_NAME}_test PRIVATE
    $<$<CONFIG:Debug>:Logger>
)
target_sources(${PROJECT_NAME} PRIVATE
    Logger.cc
)
target_sources(${PROJECT_NAME} PUBLIC
    FILE_SET HEADERS
    FILES Logger.hh
)

# SetupCodeChecks(${PROJECT_NAME})
