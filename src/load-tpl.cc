#include <mimosa/tpl/cache.hh>
#include <mimosa/log/log.hh>
#include <mimosa/options/options.hh>

#include "load-tpl.hh"

std::string & TPL_PATH = *mimosa::options::addOption<std::string>(
  "", "tpl", "the path to the tpl dir", "/srv/paste-binouse/www/tpl");

mimosa::tpl::Template::ConstPtr loadTpl(const std::string & path)
{
  static mimosa::tpl::Cache cache;

  std::string real_path(TPL_PATH + "/" + path);

  // auto tpl = cache.get(real_path)->get();
  auto tpl = mimosa::tpl::Template::parseFile(real_path);
  if (tpl)
    return tpl;

  mimosa::log::error("failed to load template: %s", real_path);
  return nullptr;
}
