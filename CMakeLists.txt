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
find_package(libpqxx CONFIG REQUIRED)
find_package(hiredis CONFIG REQUIRED)
find_package(redis++ CONFIG REQUIRED)
find_package(rabbitmq-c CONFIG REQUIRED)
find_package(msgpack-c CONFIG REQUIRED)
find_package(nlohmann_json CONFIG REQUIRED)
find_package(inja CONFIG REQUIRED)
find_package(httplib CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(argparse CONFIG REQUIRED)
find_package(mailio CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(cpr CONFIG REQUIRED)
find_package(Boost REQUIRED COMPONENTS locale)

find_package(PkgConfig REQUIRED)
pkg_check_modules(tomlplusplus REQUIRED IMPORTED_TARGET tomlplusplus)

find_path(CPPCODEC_INCLUDE_DIRS "cppcodec/base32_crockford.hpp")

# --------------------------------------------------------

FetchContent_Declare(tink
  GIT_REPOSITORY  "https://github.com/tink-crypto/tink-cc.git"
  GIT_TAG         "v2.1.1" 
)
FetchContent_MakeAvailable(tink)

# --------------------------------------------------------

add_subdirectory(sdk/cpp)

# --------------------------------------------------------

file(GLOB SOURCE_LIST CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_executable(coconut ${SOURCE_LIST})
set_target_properties(coconut PROPERTIES LINK_FLAGS_RELEASE -s)

target_link_libraries(coconut PRIVATE 
  gourd  

  OpenSSL::SSL OpenSSL::Crypto
  Boost::locale
  PostgreSQL::PostgreSQL libpqxx::pqxx
  hiredis::hiredis redis++::redis++_static
  rabbitmq::rabbitmq-static
  msgpack-c nlohmann_json::nlohmann_json pantor::inja PkgConfig::tomlplusplus
  httplib::httplib
  spdlog::spdlog_header_only
  argparse::argparse
  mailio
  CURL::libcurl cpr::cpr

  tink::static
  tink::jwt::jwt_key_templates tink::jwt::jwt_mac tink::jwt::jwt_mac_config tink::jwt::jwt_signature_config
)

target_include_directories(coconut PRIVATE
  ${PROJECT_SOURCE_DIR}/include

  ${CPPCODEC_INCLUDE_DIRS}
)


