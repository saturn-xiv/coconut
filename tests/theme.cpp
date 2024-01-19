#include <catch2/catch_test_macros.hpp>

#include "coconut/lemon/models.hpp"

#include <iostream>

TEST_CASE("Universal theme", "[universal]") {
  auto env = coconut::themes::load("universal");
  SECTION("error page") {
    coconut::lemon::models::ErrorPage it{.status = 123};
    const auto htm = env.render_file("/error.html", it);
    std::cout << htm << std::endl;
  }
}
