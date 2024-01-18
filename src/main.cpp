#include "coconut/application.hpp"
#include "coconut/crypto.hpp"
#include "coconut/version.hpp"

#include <amqp.h>
#include <event2/event.h>
#include <hiredis/hiredis.h>
#include <libpq-fe.h>
#include <openssl/opensslv.h>
#include <thrift/version.h>
#include <tink/aead/aead_config.h>
#include <tink/config/tink_config.h>
#include <tink/hybrid/hybrid_config.h>
#include <tink/jwt/jwt_mac_config.h>
#include <tink/jwt/jwt_signature_config.h>
#include <tink/mac/mac_config.h>
#include <tink/signature/signature_config.h>
#include <tink/version.h>
#include <argparse/argparse.hpp>
#include <boost/version.hpp>

namespace coconut {
static void set_thrift_logger(const char* s) { spdlog::debug("{}", s); }
}  // namespace coconut

int main(int argc, char** argv) {
  const std::string version =
      coconut::GIT_VERSION + "(" + coconut::BUILD_TIME + ")";
  argparse::ArgumentParser program(coconut::PROJECT_NAME, version);
  program.add_description(coconut::PROJECT_DESCRIPTION);
  program.add_epilog("https://github.com/saturn-xiv/palm");
  program.add_argument("-d", "--debug")
      .default_value(false)
      .help("run on debug mode")
      .implicit_value(true);

  argparse::ArgumentParser generate_token_command("generate-token");
  {
    generate_token_command.add_argument("-y", "--years")
        .default_value(10)
        .scan<'i', int>();
    generate_token_command.add_argument("-i", "--issuer").required();
    generate_token_command.add_argument("-s", "--subject").required();
    generate_token_command.add_argument("-a", "--audience").required();
  }

  argparse::ArgumentParser rpc_command("rpc");
  {
    rpc_command.add_argument("-p", "--port")
        .default_value(8080)
        .scan<'i', int>();
    rpc_command.add_argument("-s", "--ssl")
        .default_value(false)
        .help("enable mutual tls mode")
        .implicit_value(true);
    rpc_command.add_argument("--cert-file")
        .default_value(coconut::PROJECT_NAME + ".crt")
        .required();
    rpc_command.add_argument("--key-file")
        .default_value(coconut::PROJECT_NAME + ".key")
        .required();
    rpc_command.add_argument("--ca-file").default_value("ca.crt").required();
  }

  program.add_subparser(rpc_command);
  program.add_subparser(generate_token_command);

  try {
    program.parse_args(argc, argv);
  } catch (const std::runtime_error& err) {
    spdlog::error("{}", err.what());
    std::exit(1);
  }

  {
    spdlog::set_level(program.get<bool>("--debug") ? spdlog::level::debug
                                                   : spdlog::level::info);
    spdlog::debug("run on debug mode {}", version);

    spdlog::debug("OpenSSL v{}", OPENSSL_VERSION_STR);
    spdlog::debug("libevent v{}", event_get_version());
    spdlog::debug("Tink v{}", crypto::tink::Version::kTinkVersion);
    spdlog::debug(
        "Protocol Buffers v{}",
        google::protobuf::internal::VersionString(GOOGLE_PROTOBUF_VERSION));
    spdlog::debug("Thrift v{}", THRIFT_VERSION);
    {
      const auto v = PQlibVersion();
      spdlog::debug("libpq v{}.{}.{}", v / 100 / 100, v / 100 % 100, v % 100);
    }
    spdlog::debug("pqxx v{}", PQXX_VERSION);
    spdlog::debug("librabbitmq v{}", AMQP_VERSION_STRING);
    spdlog::debug("hiredis v{}.{}.{}", HIREDIS_MAJOR, HIREDIS_MINOR,
                  HIREDIS_PATCH);
    spdlog::debug("Boost v{}.{}.{}", BOOST_VERSION / 100000,
                  BOOST_VERSION / 100 % 1000, BOOST_VERSION % 100);
  }
  {
    const auto status = crypto::tink::TinkConfig::Register();
    if (!status.ok()) {
      spdlog::error("failed to register tink");
      return EXIT_FAILURE;
    }
  }
  {
    const auto status = crypto::tink::SignatureConfig::Register();
    if (!status.ok()) {
      spdlog::error("failed to register tink signature");
      return EXIT_FAILURE;
    }
  }
  {
    const auto status = crypto::tink::MacConfig::Register();
    if (!status.ok()) {
      spdlog::error("failed to register tink mac");
      return EXIT_FAILURE;
    }
  }
  {
    const auto status = crypto::tink::HybridConfig::Register();
    if (!status.ok()) {
      spdlog::error("failed to register tink hybird");
      return EXIT_FAILURE;
    }
  }
  {
    const auto status = crypto::tink::AeadConfig::Register();
    if (!status.ok()) {
      spdlog::error("failed to register tink aead");
      return EXIT_FAILURE;
    }
  }
  {
    const auto status = crypto::tink::JwtMacRegister();
    if (!status.ok()) {
      spdlog::error("failed to register tink jwt-mac");
      return EXIT_FAILURE;
    }
  }
  {
    const auto status = crypto::tink::JwtSignatureRegister();
    if (!status.ok()) {
      spdlog::error("failed to register tink jwt-signature");
      return EXIT_FAILURE;
    }
  }

  if (program.is_subcommand_used(rpc_command)) {
    const int port = rpc_command.get<int>("--port");
    const std::string cert_file = rpc_command.get<std::string>("--cert-file");
    const std::string key_file = rpc_command.get<std::string>("--key-file");
    const std::string ca_file = rpc_command.get<std::string>("--ca-file");
    const auto ssl =
        std::make_optional<coconut::Ssl>(cert_file, key_file, ca_file);

    apache::thrift::GlobalOutput.setOutputFunction(coconut::set_thrift_logger);

    coconut::application::launch(
        static_cast<uint16_t>(port),
        rpc_command.get<bool>("--ssl") ? ssl : std::nullopt);

  } else if (program.is_subcommand_used(generate_token_command)) {
    const int years = generate_token_command.get<int>("--years");
    const std::string issuer =
        generate_token_command.get<std::string>("--issuer");
    const std::string subject =
        generate_token_command.get<std::string>("--subject");
    std::string audience =
        generate_token_command.get<std::string>("--audience");
    spdlog::warn("generate token to (iss: {}, aud: {}, sub: {}) for {}-years",
                 issuer, audience, subject, years);

    const auto ttl = std::chrono::duration_cast<std::chrono::seconds>(
        std::chrono::years(years));
    coconut::Jwt jwt;
    const auto token = jwt.sign(issuer, subject, audience, ttl);
    std::cout << token << std::endl;
  }

  return EXIT_SUCCESS;
}
