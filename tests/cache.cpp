#include <catch2/catch_test_macros.hpp>

#include "coconut/cache.hpp"

#include <iostream>

TEST_CASE("Redis", "[set-get]") {
  auto config = toml::parse_file("config.toml");
  auto node = config["redis"].as_table();
  const coconut::redis::Config cfg(*node);
  std::cout << "connect " << cfg << std::endl;

  const std::string key = "hello";
  const std::string val = "Hi, Palm!";
  coconut::redis::Client cli(cfg);
  {
    cli.set(key, val, std::chrono::hours(1));
    const auto tmp = cli.get(key);
    REQUIRE(tmp.value() == val);
  }
}
