#pragma once

#include "coconut/env.hpp"

namespace coconut {
namespace twilio {
class Config {
 public:
  Config(const std::string& account_sid, const std::string& auth_token,
         const std::string from,
         const std::optional<std::string> callback = std::nullopt)
      : _account_sid(account_sid),_auth_token(auth_token),_from(from), _callback(callback) {}
  Config(const toml::table& node);

  void send(const std::string& to, const std::string& message) const;

  friend std::ostream& operator<<(std::ostream& os, const Config& it) {
    os << it._account_sid << "(" << it._from << ")";
    return os;
  }

 private:
  std::string _account_sid;
  std::string _auth_token;
  std::string _from;
  std::optional< std::string> _callback;
};
}  // namespace twilio
}  // namespace coconut
