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

#include <spdlog/spdlog.h>
#include <inja/inja.hpp>

namespace nlohmann {
template <typename T>
struct adl_serializer<std::optional<T>> {
  static void from_json(const json& j, std::optional<T>& opt) {
    if (j.is_null()) {
      opt = std::nullopt;
    } else {
      opt = j.get<T>();
    }
  }
  static void to_json(json& json, std::optional<T> t) {
    if (t) {
      json = *t;
    } else {
      json = nullptr;
    }
  }
};
}  // namespace nlohmann

namespace coconut {

struct Ssl {
  Ssl(const std::string& cert_file, const std::string& key_file,
      const std::string& ca_file)
      : cert_file(cert_file), key_file(key_file), ca_file(ca_file) {}

  std::string cert_file;
  std::string key_file;
  std::string ca_file;
};

}  // namespace coconut
