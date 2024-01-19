#include <catch2/catch_test_macros.hpp>

#include "coconut/orm.hpp"
#include "coconut/utils.hpp"

#include <iostream>

TEST_CASE("PostgreSQL", "[postgresql]") {
  auto config = toml::parse_file("config.toml");
  auto node = config["postgresql"].as_table();
  coconut::postgresql::Config cfg(*node);
  std::cout << "connect " << cfg << std::endl;

  auto pool = cfg.open();
  const auto max_size = pool->idle_size();

  {
    coconut::postgresql::PooledConnection con(pool);

    REQUIRE(pool->idle_size() == max_size - 1);
    SECTION("ping") {
      const auto size = pool->idle_size();
      REQUIRE(size > 0);
      coconut::postgresql::PooledConnection con(pool);
      REQUIRE(size < max_size);
      pqxx::work tx{*con.db};
      auto row = tx.exec1("SELECT 1");
      tx.commit();
    }
    SECTION("detect info") {
      pqxx::work tx{*con.db};
      {
        pqxx::row r = tx.exec1("SELECT VERSION()");
        std::cout << r[0].as<std::string>() << std::endl;
      }
      {
        pqxx::row r = tx.exec1("SELECT CURRENT_TIMESTAMP");

        const auto ts = r[0].as<std::string>();
        //   const auto tp = r[0].as<std::chrono::system_clock::time_point>();
        const auto tp = coconut::to_time_point(ts);
        std::cout << ts
#if _GLIBCXX_RELEASE > 12
                  << "\t" << tp
#endif
                  << std::endl;
      }
      tx.commit();
    }
  }
}

TEST_CASE("MySql", "[mysql]") { REQUIRE((1 + 1) == 2); }

TEST_CASE("Sqlite3", "[sqlite3]") { REQUIRE((1 + 1) == 2); }
