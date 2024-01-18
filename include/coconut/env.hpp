#pragma once

#include <bits/stdc++.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <algorithm>
#include <chrono>
#include <climits>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <exception>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <optional>
#include <random>
#include <ranges>
#include <set>
#include <sstream>
#include <stdexcept>
#include <string>
#include <typeinfo>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#define TOML_EXCEPTIONS 1
#include <toml++/toml.h>

#include <hiredis/hiredis.h>
#include <sw/redis++/redis++.h>
#include <inja/inja.hpp>
#include <pqxx/pqxx>

// deprecated: Since OpenSSL 3.0
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <tink/internal/ssl_unique_ptr.h>
#pragma GCC diagnostic pop

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#include <thrift/Thrift.h>
#pragma GCC diagnostic pop

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
