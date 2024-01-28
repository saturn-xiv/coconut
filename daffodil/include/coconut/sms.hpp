#pragma once

#include "coconut/env.hpp"

namespace coconut {
namespace twilio {

namespace models {

struct MessagingSendResponse {
  std::string account_sid;
  std::string api_version;
  std::string body;
  std::string date_created;
  std::string date_sent;
  std::string date_updated;
  std::string direction;
  std::optional<int32_t> error_code;
  std::optional<std::string> error_message;
  std::string from;
  std::string messaging_service_sid;
  std::string num_media;
  std::string num_segments;
  std::optional<std::string> price;
  std::string price_unit;
  std::string sid;
  std::string status;
  std::map<std::string, std::string> subresource_uris;
  std::string to;
  std::string uri;
};
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(MessagingSendResponse, account_sid,
                                   api_version, body, date_created, date_sent,
                                   date_updated, direction, error_code,
                                   error_message, from, messaging_service_sid,
                                   num_media, num_segments, price, price_unit,
                                   sid, status, subresource_uris, to, uri)
}  // namespace models

class Config {
 public:
  Config(const std::string& account_sid, const std::string& auth_token,
         const std::string& from, const std::string& callback)
      : _account_sid(account_sid),
        _auth_token(auth_token),
        _from(from),
        _callback(callback) {}
  Config(const toml::table& node);

  void send(const std::string& to, const std::string& message) const;

  friend std::ostream& operator<<(std::ostream& os, const Config& it) {
    os << it._account_sid << "(" << it._from << ")";
    return os;
  }

 private:
  inline std::string messages_api() const {
    std::stringstream ss;
    ss << "https://api.twilio.com/2010-04-01/Accounts/" << this->_account_sid
       << "/Messages.json";
    return ss.str();
  }
  inline std::string callback_url(const std::string& action) const {
    std::stringstream ss;
    ss << this->_callback << "/twilio/callback/" << action;
    return ss.str();
  }

  std::string _account_sid;
  std::string _auth_token;
  std::string _from;
  std::string _callback;
};
}  // namespace twilio
}  // namespace coconut
