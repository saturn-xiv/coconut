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
                                  std::optional<coconut::Ssl> ssl) {}
