#pragma once

#include "coconut/env.hpp"

#include "GoogleOauth2.h"

namespace coconut {
namespace google {
class Oauth2Handler final : public coconut::v1::GoogleOauth2If {
 public:
  Oauth2Handler() = default;
};
}  // namespace google
}  // namespace coconut
