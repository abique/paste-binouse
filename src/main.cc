#include <cerrno>

#include <mimosa/init.hh>
#include <mimosa/priviledge-drop.hh>
#include <mimosa/log/log.hh>
#include <mimosa/http/server.hh>
#include <mimosa/http/dispatch-handler.hh>
#include <mimosa/http/fs-handler.hh>
#include <mimosa/http/log-handler.hh>

#include "bottleneck.hh"
#include "config.hh"
#include "db.hh"
#include "purge.hh"

// handlers
#include "paste-handler.hh"
#include "view-handler.hh"

int main(int argc, char ** argv)
{
  sqlite3_initialize();
  mimosa::init(argc, argv);

  mimosa::priviledgeDrop();

  Db::instance();
  Bottleneck::instance();
  Purge::instance();

  auto dispatch = new mimosa::http::DispatchHandler;
  dispatch->registerHandler("/", new PasteHandler);
  dispatch->registerHandler("/view", new ViewHandler);
  dispatch->registerHandler(
    "/data/*", new mimosa::http::FsHandler(
      Config::instance().dataDir(), 1));
  dispatch->registerHandler(
    "/css/*", new mimosa::http::FsHandler(
      Config::instance().cssDir(), 1));
  dispatch->registerHandler(
    "/js/*", new mimosa::http::FsHandler(
      Config::instance().jsDir(), 1));
  dispatch->registerHandler(
    "/cm/*", new mimosa::http::FsHandler(
      Config::instance().cmDir(), 1));

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

  while (true)
    server->serveOne();

  Purge::release();
  Bottleneck::release();
  Db::release();
  mimosa::deinit();
  sqlite3_shutdown();
  return 0;
}
