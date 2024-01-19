#include "coconut/utils.hpp"
#include "coconut/cache.hpp"
#include "coconut/orm.hpp"
#include "coconut/queue.hpp"
#include "coconut/version.hpp"

#include <boost/algorithm/string/join.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>
#include <boost/date_time/time_facet.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/version.hpp>

#include <event2/event.h>
#include <libpq-fe.h>
#include <openssl/opensslv.h>
#include <thrift/version.h>
#include <tink/aead/aead_config.h>
#include <tink/config/tink_config.h>
#include <tink/hybrid/hybrid_config.h>
#include <tink/jwt/jwt_mac_config.h>
#include <tink/jwt/jwt_signature_config.h>
#include <tink/mac/mac_config.h>
#include <tink/signature/signature_config.h>
#include <tink/version.h>

void coconut::init(bool debug) {
  {
    spdlog::set_level(debug ? spdlog::level::debug : spdlog::level::info);
    spdlog::debug("run on debug mode {}({})", coconut::GIT_VERSION,
                  coconut::BUILD_TIME);

    spdlog::debug("OpenSSL v{}", OPENSSL_VERSION_STR);
    spdlog::debug("libevent v{}", event_get_version());
    spdlog::debug("Tink v{}", crypto::tink::Version::kTinkVersion);
    spdlog::debug(
        "Protocol Buffers v{}",
        google::protobuf::internal::VersionString(GOOGLE_PROTOBUF_VERSION));
    spdlog::debug("Thrift v{}", THRIFT_VERSION);
    {
      const auto v = PQlibVersion();
      spdlog::debug("libpq v{}.{}.{}", v / 100 / 100, v / 100 % 100, v % 100);
    }
    spdlog::debug("pqxx v{}", PQXX_VERSION);
    spdlog::debug("librabbitmq v{}", AMQP_VERSION_STRING);
    spdlog::debug("hiredis v{}.{}.{}", HIREDIS_MAJOR, HIREDIS_MINOR,
                  HIREDIS_PATCH);
    spdlog::debug("Boost v{}.{}.{}", BOOST_VERSION / 100000,
                  BOOST_VERSION / 100 % 1000, BOOST_VERSION % 100);
  }

  {
    const auto status = crypto::tink::TinkConfig::Register();
    if (!status.ok()) {
      spdlog::error("failed to register tink");
      std::string msg(status.message());
      throw std::invalid_argument(msg);
    }
  }
  {
    const auto status = crypto::tink::SignatureConfig::Register();
    if (!status.ok()) {
      spdlog::error("failed to register tink signature");
      std::string msg(status.message());
      throw std::invalid_argument(msg);
    }
  }
  {
    const auto status = crypto::tink::MacConfig::Register();
    if (!status.ok()) {
      spdlog::error("failed to register tink mac");
      std::string msg(status.message());
      throw std::invalid_argument(msg);
    }
  }
  {
    const auto status = crypto::tink::HybridConfig::Register();
    if (!status.ok()) {
      spdlog::error("failed to register tink hybird");
      std::string msg(status.message());
      throw std::invalid_argument(msg);
    }
  }
  {
    const auto status = crypto::tink::AeadConfig::Register();
    if (!status.ok()) {
      spdlog::error("failed to register tink aead");
      std::string msg(status.message());
      throw std::invalid_argument(msg);
    }
  }
  {
    const auto status = crypto::tink::JwtMacRegister();
    if (!status.ok()) {
      spdlog::error("failed to register tink jwt-mac");
      std::string msg(status.message());
      throw std::invalid_argument(msg);
    }
  }
  {
    const auto status = crypto::tink::JwtSignatureRegister();
    if (!status.ok()) {
      spdlog::error("failed to register tink jwt-signature");
      std::string msg(status.message());
      throw std::invalid_argument(msg);
    }
  }
}

bool coconut::is_stopped() { return std::filesystem::exists(".stop"); }

std::string coconut::uuid() {
  boost::uuids::uuid uuid = boost::uuids::random_generator()();
  std::string it = boost::lexical_cast<std::string>(uuid);
  return it;
}


std::string coconut::random(const size_t len) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> dist(0, 255);

  std::vector<uint8_t> buf;
  auto gen = std::bind(dist, mt);
  buf.resize(len);
  std::generate(buf.begin(), buf.end(), gen);

  std::string str(buf.begin(), buf.end());
  return str;
}

std::string coconut::timestamp() {
  //   const std::chrono::time_point<std::chrono::system_clock> now =
  //       std::chrono::system_clock::now();
  //   const std::time_t tc = std::chrono::system_clock::to_time_t(now);
  //   std::stringstream ss;
  //   ss << std::put_time(std::localtime(&tc), "%Y%m%d%H%M%S");
  //   return ss.str();

  std::ostringstream ss;
  ss.imbue(std::locale(std::cout.getloc(),
                       new boost::posix_time::time_facet("%Y%m%d%H%M%S%f")));
  ss << boost::posix_time::microsec_clock::local_time();
  return ss.str();
}
