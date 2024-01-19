#include "coconut/monitor.hpp"
#include "coconut/version.hpp"

void coconut::monitor::HealthHandler::check() {
  spdlog::info("call {}", __PRETTY_FUNCTION__);
}

void coconut::monitor::HealthHandler::version(std::string& response) {
  spdlog::debug("call {}", __PRETTY_FUNCTION__);
  response = coconut::GIT_VERSION;
}
