#pragma once


#include "coconut/thrift.hpp"

#include "Health.h"

namespace coconut {
namespace monitor {
class HealthHandler final : public coconut::v1::HealthIf {
 public:
  HealthHandler() = default;

  void check() override;
  void version(std::string& response) override;
};
}  // namespace monitor
}  // namespace coconut
