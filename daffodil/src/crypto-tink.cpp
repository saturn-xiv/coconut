#include "coconut/crypto.hpp"

#include <boost/lexical_cast.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>

#include <tink/aead.h>
#include <tink/aead/aead_config.h>
#include <tink/aead/aes_gcm_key_manager.h>
#include <tink/binary_keyset_reader.h>
#include <tink/binary_keyset_writer.h>
#include <tink/cleartext_keyset_handle.h>
#include <tink/jwt/internal/jwt_mac_impl.h>
#include <tink/jwt/internal/jwt_mac_internal.h>
#include <tink/jwt/jwt_key_templates.h>
#include <tink/jwt/jwt_validator.h>
#include <tink/jwt/raw_jwt.h>
#include <tink/mac/mac_factory.h>
#include <tink/public_key_sign.h>
#include <tink/public_key_verify.h>
#include <tink/signature/signature_key_templates.h>
#include <tink/tink_config.h>
#include <tink/util/status.h>

std::string coconut::Jwt::sign(const std::string& issuer,
                               const std::string& subject,
                               const std::string& audience,
                               const std::chrono::seconds& ttl,
                               const std::string& payload) {
  const auto jwt_id = coconut::uuid();
  spdlog::info("generate jwt token for ({}, {}, {}, {}, {})", issuer, subject,
               audience, jwt_id, ttl.count());
  auto now = absl::Now();
  auto raw_r = crypto::tink::RawJwtBuilder()
                   .SetIssuer(issuer)
                   .SetSubject(subject)
                   .AddAudience(audience)
                   .AddStringClaim(coconut::Jwt::PAYLOAD_CLAIM_NAME, payload)
                   .SetJwtId(jwt_id)
                   .SetNotBefore(now - absl::Seconds(1))
                   .SetIssuedAt(now)
                   .SetExpiration(now + absl::Seconds(ttl.count()))
                   .Build();
  this->check(raw_r);
  auto raw = std::move(raw_r.value());
  auto jwt = this->load();
  auto token_r = jwt->ComputeMacAndEncode(raw);
  this->check(token_r);
  auto token = std::move(token_r.value());
  return token;
}

std::tuple<std::string, std::string, std::string> coconut::Jwt::verify(
    const std::string& token, const std::string& issuer,
    const std::string& audience) {
  spdlog::debug("{}", token);
  auto validator_r = crypto::tink::JwtValidatorBuilder()
                         .IgnoreTypeHeader()
                         .ExpectIssuer(issuer)
                         .ExpectAudience(audience)
                         .Build();
  this->check(validator_r);
  auto validator = std::move(validator_r.value());

  auto jwt = this->load();
  auto body_r = jwt->VerifyMacAndDecode(token, validator);
  this->check(body_r);
  auto body = std::move(body_r.value());

  auto subject_r = body.GetSubject();
  this->check(subject_r);
  auto subject = std::move(subject_r.value());

  auto jwt_id_r = body.GetJwtId();
  this->check(jwt_id_r);
  auto jwt_id = std::move(jwt_id_r.value());

  auto payload_r = body.GetStringClaim(coconut::Jwt::PAYLOAD_CLAIM_NAME);
  this->check(payload_r);
  auto payload = std::move(payload_r.value());

  spdlog::debug("get ({}, {}, {})", jwt_id, subject, payload);

  return std::make_tuple(subject, jwt_id, payload);
}

std::unique_ptr<crypto::tink::JwtMac> coconut::Jwt::load() {
  auto keyset = this->Keyset::load(crypto::tink::JwtHs512Template());
  auto jwt_r = keyset->GetPrimitive<crypto::tink::JwtMac>(
      crypto::tink::ConfigGlobalRegistry());
  this->check(jwt_r);
  auto jwt = std::move(jwt_r.value());
  return jwt;
}

std::string coconut::HMac::sign(const std::string& plain) {
  auto mac = this->load();
  auto code_r = mac->ComputeMac(plain);
  this->check(code_r);
  auto code = std::move(code_r.value());
  return code;
}

void coconut::HMac::verify(const std::string& code, const std::string& plain) {
  auto mac = this->load();
  auto status = mac->VerifyMac(code, plain);
  this->check(status);
}

std::unique_ptr<crypto::tink::Mac> coconut::HMac::load() {
  auto keyset = this->Keyset::load(crypto::tink::MacKeyTemplates::HmacSha512());
  auto mac_r = keyset->GetPrimitive<crypto::tink::Mac>(
      crypto::tink::ConfigGlobalRegistry());
  this->check(mac_r);
  auto mac = std::move(mac_r.value());
  return mac;
}

std::string coconut::Aes::encrypt(const std::string& plain,
                                  const std::string& salt) {
  auto aes = this->load();
  auto code_r = aes->Encrypt(plain, salt);
  this->check(code_r);
  auto code = std::move(code_r.value());
  return code;
}

std::string coconut::Aes::decrypt(const std::string& code,
                                  const std::string& salt) {
  auto aes = this->load();
  auto plain_r = aes->Decrypt(code, salt);
  this->check(plain_r);
  auto plain = std::move(plain_r.value());
  return plain;
}

std::unique_ptr<crypto::tink::Aead> coconut::Aes::load() {
  auto keyset = this->Keyset::load(crypto::tink::AeadKeyTemplates::Aes256Gcm());
  auto aes_r = keyset->GetPrimitive<crypto::tink::Aead>(
      crypto::tink::ConfigGlobalRegistry());
  this->check(aes_r);
  auto aes = std::move(aes_r.value());
  return aes;
}

std::unique_ptr<crypto::tink::KeysetHandle> coconut::Keyset::load(
    const google::crypto::tink::KeyTemplate& tpl) {
  const std::lock_guard<std::mutex> lock(this->_locker);

  const auto file = this->keyset();
  if (std::filesystem::exists(file)) {
    spdlog::debug("load keyset from {}", file.string());

    if (std::filesystem::status(file).permissions() !=
        std::filesystem::perms::owner_read) {
      throw std::invalid_argument("key file too open");
    }

    std::unique_ptr<std::ifstream> in =
        std::make_unique<std::ifstream>(file, std::ios_base::binary);
    auto reader_r = crypto::tink::BinaryKeysetReader::New(std::move(in));
    this->check(reader_r);
    auto reader = std::move(reader_r.value());
    auto keyset_handle_r =
        crypto::tink::CleartextKeysetHandle::Read(std::move(reader));
    this->check(keyset_handle_r);
    auto keyset_handle = std::move(keyset_handle_r.value());
    return keyset_handle;

  } else {
    spdlog::warn("not exists, try to create {}", file.string());

    auto keyset_handle_r = crypto::tink::KeysetHandle::GenerateNew(
        tpl, crypto::tink::KeyGenConfigGlobalRegistry());
    this->check(keyset_handle_r);
    auto keyset_handler = std::move(keyset_handle_r.value());
    {
      std::unique_ptr<std::ofstream> out = std::make_unique<std::ofstream>();
      out->open(file, std::ios_base::binary);
      auto writer_r = crypto::tink::BinaryKeysetWriter::New(std::move(out));
      this->check(writer_r);
      auto writer = std::move(writer_r.value());
      const auto status = crypto::tink::CleartextKeysetHandle::Write(
          writer.get(), *keyset_handler.get());
      this->check(status);
    }
    std::filesystem::permissions(file, std::filesystem::perms::owner_read);
    return keyset_handler;
  }
}

std::string coconut::random(const size_t len) {
  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_int_distribution<uint8_t> dist(0, 255);

  std::vector<uint8_t> buf;
  auto gen = std::bind(dist, mt);
  buf.resize(len);
  std::generate(buf.begin(), buf.end(), gen);

  std::string str(buf.begin(), buf.end());
  return str;
}

std::string coconut::uuid() {
  boost::uuids::uuid it = boost::uuids::random_generator()();
  return boost::lexical_cast<std::string>(it);
}
