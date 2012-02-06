#include <mimosa/sqlite/sqlite.hh>
#include <mimosa/log/log.hh>
#include <mimosa/http/redirect.hh>
#include <mimosa/tpl/dict.hh>

#include "db.hh"
#include "post-handler.hh"
#include "load-tpl.hh"
#include "page-header.hh"
#include "page-footer.hh"

bool
PostHandler::handle(mimosa::http::RequestReader & request,
                    mimosa::http::ResponseWriter & response) const
{
  const auto & form = request.form();

  auto it = form.find("content");
  if (it != form.end())
  {
    mimosa::sqlite::Stmt stmt;
    int err = stmt.prepare(Db::handle(),
                           "insert or fail into paste (content, content_size)"
                           " values (?, ?)");
    assert(err == SQLITE_OK); // must pass

    err = stmt.bindBlob(1, it->second.data(), it->second.size());
    assert(err == SQLITE_OK); // must pass

    err = stmt.bind(2, it->second.size());
    assert(err == SQLITE_OK); // must pass

    err = stmt.step();
    if (err != SQLITE_DONE)
    {
      mimosa::log::error("failed to post: %s", sqlite3_errmsg(Db::handle()));
      return false;
    }

    int64_t row_id = sqlite3_last_insert_rowid(Db::handle());
    return redirect(response, mimosa::format::str("/view?id=%d", row_id));
  }

  auto tpl = loadTpl("page.html");
  if (!tpl)
    return false;

  mimosa::tpl::Dict dict;

  auto tpl_body = loadTpl("post.html");
  if (!tpl_body)
    return false;
  dict.append("body", tpl_body);

  setPageHeader(dict);
  setPageFooter(dict);

  response.status_ = mimosa::http::kStatusOk;
  response.content_type_ = "text/html";
  response.sendHeader(response.writeTimeout());
  tpl->execute(&response, dict, response.writeTimeout());
  return true;
}
