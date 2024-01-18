#pragma once

#include "coconut/env.hpp"

namespace coconut {
namespace application {
void launch(const uint16_t port, std::optional<coconut::Ssl> ssl);
};
}  // namespace coconut
