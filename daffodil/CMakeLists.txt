file(GLOB HEADER_LIST CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/include/coconut/*.hpp")
file(GLOB SOURCE_LIST CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

add_library(daffodil STATIC ${SOURCE_LIST})

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
find_package(magic_enum CONFIG REQUIRED)

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


target_link_libraries(daffodil PRIVATE 
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
  magic_enum::magic_enum

  tink::static
  tink::jwt::jwt_key_templates tink::jwt::jwt_mac tink::jwt::jwt_mac_config tink::jwt::jwt_signature_config
)

target_include_directories(daffodil PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include

  ${CPPCODEC_INCLUDE_DIRS}
)

source_group(
  TREE "${CMAKE_CURRENT_SOURCE_DIR}/include"
  PREFIX "Header Files"
  FILES ${HEADER_LIST}
)
