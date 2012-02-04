#include <mimosa/sqlite/sqlite.hh>
#include <mimosa/log/log.hh>
#include <mimosa/http/redirect.hh>
#include <mimosa/tpl/dict.hh>

#include "db.hh"
#include "view-handler.hh"
#include "load-tpl.hh"
#include "page-header.hh"
#include "page-footer.hh"

bool
ViewHandler::handle(mimosa::http::RequestReader & request,
                    mimosa::http::ResponseWriter & response) const
{
  const auto & query = request.query();
  std::string value;

  auto it = query.find("id");
  if (it != query.end())
  {
    mimosa::sqlite::Stmt stmt;
    int err = stmt.prepare(Db::handle(),
                           "select content from paste where paste_id = ?");
    assert(err == SQLITE_OK); // must pass

    err = stmt.bind(1, it->second);
    assert(err == SQLITE_OK); // must pass
    err = stmt.step();
    if (err != SQLITE_ROW)
    {
      mimosa::log::error("not found");
      return false;
    }

    value = (const char *)sqlite3_column_text(stmt, 0);
  }

  auto tpl = loadTpl("page.html");
  if (!tpl)
    return false;

  mimosa::tpl::Dict dict;

  auto tpl_body = loadTpl("view.html");
  if (!tpl_body)
    return false;
  dict.append("body", tpl_body);
  dict.append("content", value);

  setPageHeader(dict);
  setPageFooter(dict);

  response.status_ = mimosa::http::kStatusOk;
  response.content_type_ = "text/html";
  response.sendHeader(response.writeTimeout());
  tpl->execute(&response, dict, response.writeTimeout());
  return true;
}
