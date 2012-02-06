#include <mimosa/options/options.hh>

#include "config.hh"

static uint16_t & PORT =
  *mimosa::options::addOption<uint16_t>("", "port", "the port to use", 11013);

static uint64_t & CAPACITY =
  *mimosa::options::addOption<uint64_t>("", "capacity", "the capacity in bytes", 42 * 1042 * 1024);

static std::string & ROOT =
  *mimosa::options::addOption<std::string>("", "root", "the root directory", "/srv/paste-binouse");

Config::Config()
  : root_dir_(ROOT),
    tpl_dir_(ROOT + "/www/tpl/"),
    css_dir_(ROOT + "/www/css/"),
    data_dir_(ROOT + "/www/data/"),
    db_(ROOT + "/db.sqlite"),
    port_(PORT),
    capacity_(CAPACITY)
{
}
