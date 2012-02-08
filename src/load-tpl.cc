#include <mimosa/tpl/cache.hh>
#include <mimosa/log/log.hh>

#include "config.hh"
#include "load-tpl.hh"

mimosa::tpl::Template::ConstPtr loadTpl(const std::string & path)
{
  static mimosa::tpl::Cache cache;

  std::string real_path(Config::tplDir() + path);

  auto tpl = cache.get(real_path)->get();
  if (tpl)
    return tpl;

  mimosa::log::error("failed to load template: %s", real_path);
  return nullptr;
}
