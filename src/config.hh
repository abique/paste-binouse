#ifndef CONFIG_HH
# define CONFIG_HH

# include <cstdint>
# include <string>

# include <mimosa/singleton.hh>

class Config : public mimosa::Singleton<Config>
{
public:
  Config();

  static const std::string & rootDir() { return instance().root_dir_; }
  static const std::string & dataDir() { return instance().data_dir_; }
  static const std::string & db() { return instance().db_; }
  static const std::string & tplDir() { return instance().tpl_dir_; }
  static const std::string & cssDir() { return instance().css_dir_; }
  static const std::string & jsDir() { return instance().js_dir_; }

  static uint64_t maxPasteSize() { return instance().max_paste_size_; }
  static uint64_t capacity() { return instance().capacity_; }
  static uint16_t port() { return instance().port_; }
  static uint16_t msSleep() { return instance().ms_sleep_; }
  static uint16_t maxClients() { return instance().max_clients_; }

private:
  std::string root_dir_;
  std::string tpl_dir_;
  std::string css_dir_;
  std::string js_dir_;
  std::string data_dir_;
  std::string db_;

  uint16_t port_;
  uint64_t capacity_;
  uint64_t max_paste_size_;
  uint16_t ms_sleep_;
  uint16_t max_clients_;
};

#endif /* !CONFIG_HH */
