#include <mimosa/tpl/include.hh>
#include <mimosa/tpl/value.hh>

#include "page-header.hh"
#include "load-tpl.hh"

bool setPageHeader(mimosa::tpl::Dict & dict)
{
  auto tpl = loadTpl("header.html");
  if (!tpl)
    return false;

  dict.append("header", tpl);

  return true;
}
