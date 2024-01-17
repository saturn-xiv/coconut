cmake_minimum_required(VERSION 3.16)
include(ExternalProject)
include(FetchContent)

project(
    coconut
    VERSION 2024.1.17
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
configure_file(include/coconut/version.h.in ${PROJECT_SOURCE_DIR}/include/coconut/version.hpp)

# --------------------------------------------------------

find_package(OpenSSL REQUIRED)
find_package(PostgreSQL REQUIRED)

# --------------------------------------------------------

FetchContent_Declare(spdlog
  GIT_REPOSITORY  "https://github.com/gabime/spdlog.git"
  GIT_TAG         "v1.12.0"
)
FetchContent_Declare(tomlplusplus
  GIT_REPOSITORY  "https://github.com/marzer/tomlplusplus.git"
  GIT_TAG         "v3.4.0"
)
FetchContent_Declare(argparse
  GIT_REPOSITORY  "https://github.com/p-ranav/argparse.git"
  GIT_TAG         "v3.0"
)
FetchContent_Declare(tink
  GIT_REPOSITORY  "https://github.com/tink-crypto/tink-cc.git"
  GIT_TAG         "v2.1.1" 
)
FetchContent_Declare(inja
  GIT_REPOSITORY    "https://github.com/pantor/inja.git"
  GIT_TAG           "v3.4.0"
)
FetchContent_Declare(httplib
  GIT_REPOSITORY    "https://github.com/yhirose/cpp-httplib.git"
  GIT_TAG           "v0.14.3"
)
FetchContent_Declare(pqxx
  GIT_REPOSITORY    "https://github.com/jtv/libpqxx.git"
  GIT_TAG           "7.8.1"
)
FetchContent_MakeAvailable(pqxx spdlog argparse tomlplusplus inja httplib tink)

# --------------------------------------------------------

add_subdirectory(sdk/cpp)

# --------------------------------------------------------

file(GLOB SOURCE_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")
file(GLOB GOURD_SOURCE_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/sdk/cpp/src/*.cpp")

add_executable(coconut ${GOURD_SOURCE_LIST} ${SOURCE_LIST})
set_target_properties(coconut PROPERTIES LINK_FLAGS_RELEASE -s)

target_link_libraries(coconut gourd
  ${OPENSSL_LIBRARIES} ${PostgreSQL_LIBRARIES}

  tink::static
  tink::jwt::jwt_key_templates tink::jwt::jwt_mac tink::jwt::jwt_mac_config tink::jwt::jwt_signature_config
)

target_include_directories(coconut PUBLIC
  ${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/sdk/cpp/include
    
  ${spdlog_SOURCE_DIR}/include
  ${argparse_SOURCE_DIR}/include
  ${tomlplusplus_SOURCE_DIR}/include

)


