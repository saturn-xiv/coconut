#include "coconut/application.hpp"
#include "coconut/auth.hpp"
#include "coconut/controllers.hpp"
#include "coconut/google.hpp"
#include "coconut/monitor.hpp"
#include "coconut/version.hpp"
#include "coconut/wechat.hpp"

#include <thrift/concurrency/ThreadFactory.h>
#include <thrift/concurrency/ThreadManager.h>
#include <thrift/processor/TMultiplexedProcessor.h>
#include <thrift/protocol/TBinaryProtocol.h>
#include <thrift/server/TNonblockingServer.h>
#include <thrift/server/TThreadPoolServer.h>
#include <thrift/server/TThreadedServer.h>
#include <thrift/transport/TNonblockingSSLServerSocket.h>
#include <thrift/transport/TNonblockingServerSocket.h>
#include <thrift/transport/TSSLServerSocket.h>
#include <thrift/transport/TSSLSocket.h>
#include <thrift/transport/TServerSocket.h>
#include <thrift/transport/TSocket.h>
#include <thrift/transport/TTransportUtils.h>

void coconut::application::launch(const uint16_t port,
                                  std::optional<coconut::Ssl> ssl) {
  std::shared_ptr<coconut::google::Oauth2Handler> googleOauth2Handler =
      std::make_shared<coconut::google::Oauth2Handler>();
  std::shared_ptr<v1::GoogleOauth2Processor> googleOauth2Processor =
      std::make_shared<v1::GoogleOauth2Processor>(googleOauth2Handler);

  std::shared_ptr<coconut::wechat::Oauth2Handler> wechatOauth2Handler =
      std::make_shared<coconut::wechat::Oauth2Handler>();
  std::shared_ptr<v1::WechatOauth2Processor> wechatOauth2Processor =
      std::make_shared<v1::WechatOauth2Processor>(wechatOauth2Handler);

  std::shared_ptr<coconut::wechat::MiniProgramHandler>
      wechatMiniProgramHandler =
          std::make_shared<coconut::wechat::MiniProgramHandler>();
  std::shared_ptr<v1::WechatMiniProgramProcessor> wechatMiniProgramProcessor =
      std::make_shared<v1::WechatMiniProgramProcessor>(
          wechatMiniProgramHandler);

  std::shared_ptr<monitor::HealthHandler> healthHandler =
      std::make_shared<monitor::HealthHandler>();
  std::shared_ptr<v1::HealthProcessor> healthProcessor =
      std::make_shared<v1::HealthProcessor>(healthHandler);

  std::shared_ptr<apache::thrift::TMultiplexedProcessor> multiplexedProcessor =
      std::make_shared<apache::thrift::TMultiplexedProcessor>();

  {
    const auto name = typeid(v1::GoogleOauth2If).name();
    spdlog::info("register google.oauth2 service {}", name);
    multiplexedProcessor->registerProcessor(name, googleOauth2Processor);
  }
  {
    const auto name = typeid(v1::WechatOauth2If).name();
    spdlog::info("register wechat.oauth2 service {}", name);
    multiplexedProcessor->registerProcessor(name, wechatOauth2Processor);
  }
  {
    const auto name = typeid(v1::WechatMiniProgramIf).name();
    spdlog::info("register wechat.mini-program service {}", name);
    multiplexedProcessor->registerProcessor(name, wechatMiniProgramProcessor);
  }
  {
    const auto name = typeid(v1::HealthIf).name();
    spdlog::info("register health service {}", name);
    multiplexedProcessor->registerProcessor(name, healthProcessor);
  }

  std::shared_ptr<apache::thrift::TProcessor> processor =
      std::dynamic_pointer_cast<apache::thrift::TProcessor>(
          multiplexedProcessor);

  auto threads_count = std::thread::hardware_concurrency();

  std::shared_ptr<apache::thrift::concurrency::ThreadFactory> threadFactory =
      std::make_shared<apache::thrift::concurrency::ThreadFactory>();
  std::shared_ptr<apache::thrift::concurrency::ThreadManager> threadManager =
      apache::thrift::concurrency::ThreadManager::newSimpleThreadManager(
          threads_count * 4 + 1);
  threadManager->threadFactory(threadFactory);
  threadManager->start();

  std::shared_ptr<apache::thrift::protocol::TBinaryProtocolFactoryT<
      apache::thrift::transport::TFramedTransport>>
      protocolFactory =
          std::make_shared<apache::thrift::protocol::TBinaryProtocolFactoryT<
              apache::thrift::transport::TFramedTransport>>();

  std::shared_ptr<apache::thrift::server::TNonblockingServer> server;

  if (ssl) {
    std::shared_ptr<apache::thrift::transport::TSSLSocketFactory>
        sslSocketFactory =
            std::make_shared<apache::thrift::transport::TSSLSocketFactory>();
    {
      spdlog::info("listening on tcps://0.0.0.0:{}", port);
      spdlog::debug("load cert from {}, key from {}, ca from {}",
                    ssl->cert_file, ssl->key_file, ssl->ca_file);
      sslSocketFactory->loadCertificate(ssl->cert_file.c_str());
      sslSocketFactory->loadPrivateKey(ssl->key_file.c_str());
      sslSocketFactory->loadTrustedCertificates(ssl->ca_file.c_str());

      sslSocketFactory->ciphers("ALL:!ADH:!LOW:!EXP:!MD5:@STRENGTH");
    }

    std::shared_ptr<apache::thrift::transport::TNonblockingSSLServerSocket>
        serverSocket = std::make_shared<
            apache::thrift::transport::TNonblockingSSLServerSocket>(
            port, sslSocketFactory);

    server = std::make_shared<apache::thrift::server::TNonblockingServer>(
        multiplexedProcessor, protocolFactory, serverSocket, threadManager);
  } else {
    spdlog::info("listening on tcp://0.0.0.0:{}", port);
    std::shared_ptr<apache::thrift::transport::TNonblockingServerSocket>
        serverSocket = std::make_shared<
            apache::thrift::transport::TNonblockingServerSocket>(port);
    server = std::make_shared<apache::thrift::server::TNonblockingServer>(
        multiplexedProcessor, protocolFactory, serverSocket, threadManager);
  }

  server->setNumIOThreads(threads_count * 2 + 1);
  server->serve();
}
