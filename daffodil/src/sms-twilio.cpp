#include "coconut/sms.hpp"

coconut::twilio::Config::Config(const toml::table& node) {
  this->_account_sid = node["account-sid"].value<std::string>().value();
  this->_auth_token = node["auth-token"].value<std::string>().value();
  this->_from = node["from"].value<std::string>().value();
  this->_callback = node["callback"].value<std::string>();
}
