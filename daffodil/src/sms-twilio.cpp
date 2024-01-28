#include "coconut/sms.hpp"

#include <cpr/cpr.h>

// https://www.twilio.com/docs/messaging/api/message-resource

coconut::twilio::Config::Config(const toml::table& node) {
  this->_account_sid = node["account-sid"].value<std::string>().value();
  this->_auth_token = node["auth-token"].value<std::string>().value();
  this->_from = node["from"].value<std::string>().value();
  this->_callback = node["callback"].value<std::string>().value();
}

void coconut::twilio::Config::send(const std::string& to,
                                   const std::string& message) const {
  spdlog::info("send message to {}", to);
  spdlog::debug("{}", message);

  cpr::Response response =
      cpr::Post(cpr::Url{this->messages_api()},
                cpr::Payload{{"From", this->_from},
                             {"To", to},
                             {"Message", message},
                             {"StatusCallback", this->callback_url("sms")}},
                cpr::Authentication{this->_account_sid, this->_auth_token,
                                    cpr::AuthMode::BASIC});

  if (response.status_code != 204) {
    spdlog::error("{}: {}", response.status_code, response.text);
    return;
  }
  spdlog::debug("{}", response.text);
  auto js = nlohmann::json ::parse(response.text);
  const auto it = js.get<coconut::twilio::models::MessagingSendResponse>();
  spdlog::info("{} {}", it.sid, it.status);
}
