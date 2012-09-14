#include <mimosa/options/options.hh>

#include "config.hh"

static uint16_t & PORT =
  *mimosa::options::addOption<uint16_t>(
    "", "port", "the port to use", 11013);

static uint16_t & MS_SLEEP =
  *mimosa::options::addOption<uint16_t>(
    "", "ms-sleep", "the paste bottleneck", 10);

static uint16_t & MAX_CLIENTS =
  *mimosa::options::addOption<uint16_t>(
    "", "max-clients", "the maximum number of client at the same time", 8);

static uint64_t & CAPACITY =
  *mimosa::options::addOption<uint64_t>(
    "", "capacity",
    "the total paste capacity in bytes",
    42 * 1042 * 1024);

static uint64_t & MAX_PASTE_SIZE =
  *mimosa::options::addOption<uint64_t>(
    "", "max-paste-size", "the maximum past size in bytes",
    256 * 1024);

static std::string & ROOT =
  *mimosa::options::addOption<std::string>(
    "", "root", "the root directory", "/srv/paste-binouse");

Config::Config()
  : root_dir_(ROOT),
    tpl_dir_(ROOT + "/www/tpl/"),
    css_dir_(ROOT + "/www/css/"),
    js_dir_(ROOT + "/www/js/"),
    cm_dir_(ROOT + "/www/CodeMirror/"),
    data_dir_(ROOT + "/www/data/"),
    db_(ROOT + "/db.sqlite"),
    port_(PORT),
    capacity_(CAPACITY),
    max_paste_size_(MAX_PASTE_SIZE),
    ms_sleep_(MS_SLEEP),
    max_clients_(MAX_CLIENTS)
{
}
