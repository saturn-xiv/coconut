#pragma once

#include "coconut/cache.hpp"
#include "coconut/email.hpp"
#include "coconut/orm.hpp"
#include "coconut/queue.hpp"
#include "coconut/sms.hpp"

namespace coconut {
namespace application {

class Config {
 private:
  PostgreSql postgresql;
  RabbitMQ rabbitmq;
  coconut::redis::Config redis;
  Smtp smtp;
};

void launch(const uint16_t port, std::optional<coconut::Ssl> ssl);
};  // namespace application
}  // namespace coconut
