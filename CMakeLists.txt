cmake_minimum_required(VERSION 3.16)
include(ExternalProject)
include(FetchContent)

project(
    coconut
    VERSION 2024.1.19
    DESCRIPTION "A flexible authentication solution by CPP."
    LANGUAGES C CXX)
# --------------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# --------------------------------------------------------

execute_process(COMMAND git describe --tags --always --dirty
    OUTPUT_VARIABLE GIT_REV
    ERROR_QUIET
)
string(STRIP "${GIT_REV}" GIT_REV)
string(TIMESTAMP BUILD_TIME UTC)
configure_file(daffodil/include/coconut/version.h.in ${PROJECT_SOURCE_DIR}/daffodil/include/coconut/version.hpp)

# --------------------------------------------------------

add_subdirectory(sdk/cpp)
add_subdirectory(daffodil)
add_subdirectory(apps)
add_subdirectory(tests)

