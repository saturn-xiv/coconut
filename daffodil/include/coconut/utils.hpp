#pragma once

#include "coconut/env.hpp"

namespace coconut {
std::string random(const size_t len);
std::string uuid();
std::string timestamp();
bool is_stopped();
void init(bool debug = false);
}  // namespace coconut
