#pragma once

#include "coconut/env.hpp"

#include "WechatMiniProgram.h"
#include "WechatOauth2.h"

namespace coconut {
namespace wechat {
class Oauth2Handler final : public coconut::v1::WechatOauth2If {
 public:
  Oauth2Handler() = default;
};

class MiniProgramHandler final : public coconut::v1::WechatMiniProgramIf {
 public:
  MiniProgramHandler() = default;
};
}  // namespace wechat
}  // namespace coconut
