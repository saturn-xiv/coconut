#pragma once

#include "coconut/cache.hpp"
#include "coconut/email.hpp"
#include "coconut/orm.hpp"
#include "coconut/queue.hpp"
#include "coconut/s3.hpp"
#include "coconut/sms.hpp"

namespace coconut {
namespace orchid {
namespace application {

class Config {
 public:
  Config(const toml::table& node)
      : _postgresql(*node["postgresql"].as_table()),
        _redis(*node["redis"].as_table()),
        _minio(*node["minio"].as_table()),
        _rabbitmq(*node["rabbitmq"].as_table()),
        _smtp(*node["smtp"].as_table()),
        _twilio(*node["twilio"].as_table()) {}

 private:
  coconut::postgresql::Config _postgresql;
  coconut::redis::Config _redis;
  coconut::minio::Config _minio;
  coconut::smtp::Config _smtp;
  coconut::rabbitmq::Config _rabbitmq;
  coconut::twilio::Config _twilio;
};

void rpc(const uint16_t port, std::optional<coconut::Ssl> ssl);
};  // namespace application
}  // namespace orchid
}  // namespace coconut
