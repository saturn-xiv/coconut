#include "coconut/orchid.hpp"
#include "coconut/application.hpp"
#include "coconut/thrift.hpp"
#include "coconut/version.hpp"

#include <argparse/argparse.hpp>

namespace coconut {
static void set_thrift_logger(const char* s) { spdlog::debug("{}", s); }
}  // namespace coconut

int coconut::orchid::launch(int argc, char** argv) {
  const std::string version =
      coconut::GIT_VERSION + "(" + coconut::BUILD_TIME + ")";
  argparse::ArgumentParser program(coconut::PROJECT_NAME, version);
  program.add_description(coconut::PROJECT_DESCRIPTION);
  program.add_epilog("https://github.com/saturn-xiv/palm");
  program.add_argument("-d", "--debug")
      .default_value(false)
      .help("run on debug mode")
      .implicit_value(true);
  program.add_argument("-c", "--config")
      .default_value("config.toml")
      .help("configuration file")
      .required();

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

  argparse::ArgumentParser sms_worker_command("sms-worker");
  argparse::ArgumentParser email_worker_command("email-worker");

  program.add_subparser(rpc_command);
  program.add_subparser(generate_token_command);
  program.add_subparser(sms_worker_command);
  program.add_subparser(email_worker_command);

  try {
    program.parse_args(argc, argv);
  } catch (const std::runtime_error& err) {
    spdlog::error("{}", err.what());
    std::exit(1);
  }

  coconut::init(program.get<bool>("--debug"));

  if (program.is_subcommand_used(generate_token_command)) {
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
    return EXIT_SUCCESS;
  }

  if (coconut::is_stopped()) {
    spdlog::warn("stop file exists, will quit...");
    return EXIT_SUCCESS;
  }

  const std::string config_file = program.get<std::string>("--config");
  spdlog::debug("load configuration from {}", config_file);

  if (program.is_subcommand_used(rpc_command)) {
    const int port = rpc_command.get<int>("--port");
    const std::string cert_file = rpc_command.get<std::string>("--cert-file");
    const std::string key_file = rpc_command.get<std::string>("--key-file");
    const std::string ca_file = rpc_command.get<std::string>("--ca-file");
    const auto ssl =
        std::make_optional<coconut::Ssl>(cert_file, key_file, ca_file);

    apache::thrift::GlobalOutput.setOutputFunction(coconut::set_thrift_logger);

    coconut::orchid::application::rpc(
        static_cast<uint16_t>(port),
        rpc_command.get<bool>("--ssl") ? ssl : std::nullopt);
    return EXIT_SUCCESS;
  }

  if (program.is_subcommand_used(sms_worker_command)) {
    spdlog::info("launch a sms worker");
  }

  if (program.is_subcommand_used(email_worker_command)) {
    spdlog::info("launch an email worker");
  }

  spdlog::info("nothing todo, quit...");
  return EXIT_SUCCESS;
}
