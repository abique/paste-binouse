#include <mimosa/tpl/dict.hh>

#include "error-handler.hh"
#include "load-tpl.hh"
#include "page-header.hh"
#include "page-footer.hh"

bool errorHandler(mimosa::http::ResponseWriter & response,
                  const std::string & message)
{
  auto tpl = loadTpl("page.html");
  if (!tpl)
    return false;

  mimosa::tpl::Dict dict;

  auto tpl_body = loadTpl("error.html");
  if (!tpl_body)
    return false;
  dict.append("body", tpl_body);
  dict.append("error", message);

  setPageHeader(dict);
  setPageFooter(dict);
  response.setStatus(mimosa::http::kStatusOk);
  response.setContentType("text/html");
  response.sendHeader();
  tpl->execute(&response, dict);
  return true;
}
