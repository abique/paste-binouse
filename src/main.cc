#include <cerrno>

#include <mimosa/init.hh>
#include <mimosa/log/log.hh>
#include <mimosa/http/server.hh>
#include <mimosa/http/dispatch-handler.hh>
#include <mimosa/http/fs-handler.hh>
#include <mimosa/http/log-handler.hh>
#include <mimosa/runtime/thread-pool.hh>

#include "bottleneck.hh"
#include "config.hh"
#include "db.hh"
#include "purge.hh"

// handlers
#include "post-handler.hh"
#include "view-handler.hh"

int main(int argc, char ** argv)
{
  sqlite3_initialize();
  mimosa::init(argc, argv);
  Db::instance();
  Bottleneck::instance();
  Purge::instance();

  auto dispatch = new mimosa::http::DispatchHandler;
  dispatch->registerHandler("/", new PostHandler);
  dispatch->registerHandler("/view", new ViewHandler);
  dispatch->registerHandler(
    "/data/*", new mimosa::http::FsHandler(
      Config::instance().dataDir(), 1, true));
  dispatch->registerHandler(
    "/css/*", new mimosa::http::FsHandler(
      Config::instance().cssDir(), 1, true));

  auto log_handler = new mimosa::http::LogHandler;
  log_handler->setHandler(dispatch);

  mimosa::http::Server::Ptr server(new mimosa::http::Server);
  server->setHandler(log_handler);

  if (!server->listenInet4(Config::port()))
  {
    mimosa::log::fatal("failed to listen on the port %d: %s",
                       Config::port(), ::strerror(errno));
    return 1;
  }

  mimosa::runtime::ThreadPool thread_pool([server] {
      while (true)
        server->serveOne();
    });

  for (int i = 0; i < Config::maxClients(); ++i)
    thread_pool.startThread();

  thread_pool.join();
  Purge::release();
  Bottleneck::release();
  Db::release();
  mimosa::deinit();
  sqlite3_shutdown();
  return 0;
}
