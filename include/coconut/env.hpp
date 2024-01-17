#pragma once

namespace coconut {
class PostgreSql {};

class RabbitMQ {};

class Redis {};

class Smtp {};

class Config {
 private:
  PostgreSql postgresql;
  RabbitMQ rabbitmq;
  Redis redis;
  Smtp smtp;
};
}  // namespace coconut
