#include <mimosa/tpl/include.hh>

#include "page-footer.hh"
#include "load-tpl.hh"

bool setPageFooter(mimosa::tpl::Dict & dict)
{
  auto tpl = loadTpl("footer.html");
  if (!tpl)
    return false;

  dict.append("footer", tpl);
  return true;
}
