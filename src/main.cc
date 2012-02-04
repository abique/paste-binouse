#include <cerrno>

#include <mimosa/init.hh>
#include <mimosa/log/log.hh>
#include <mimosa/options/options.hh>
#include <mimosa/http/server.hh>
#include <mimosa/http/dispatch-handler.hh>
#include <mimosa/http/fs-handler.hh>
#include <mimosa/http/log-handler.hh>

#include "db.hh"

// handlers
#include "post-handler.hh"
#include "view-handler.hh"

uint16_t & PORT = *mimosa::options::addOption<uint16_t>("", "port", "the port to use", 11013);

int main(int argc, char ** argv)
{
  sqlite3_initialize();
  mimosa::init(argc, argv);
  Db::instance();

  auto dispatch = new mimosa::http::DispatchHandler;
  dispatch->registerHandler("/", new PostHandler);
  dispatch->registerHandler("/view", new ViewHandler);

  auto log_handler = new mimosa::http::LogHandler;
  log_handler->setHandler(dispatch);

  mimosa::http::Server::Ptr server(new mimosa::http::Server);
  server->setHandler(log_handler);

  if (!server->listenInet4(PORT))
  {
    mimosa::log::fatal("failed to listen on the port %d: %s",
                       PORT, ::strerror(errno));
    return 1;
  }

  while (true)
    server->serveOne();

  Db::release();
  mimosa::deinit();
  sqlite3_shutdown();
  return 0;
}
