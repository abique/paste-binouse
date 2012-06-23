#include <mimosa/sqlite/sqlite.hh>
#include <mimosa/log/log.hh>
#include <mimosa/http/redirect.hh>
#include <mimosa/tpl/dict.hh>
#include <mimosa/stream/string-stream.hh>
#include <mimosa/stream/lzma-decoder.hh>

#include "config.hh"
#include "db.hh"
#include "view-handler.hh"
#include "load-tpl.hh"
#include "page-header.hh"
#include "page-footer.hh"
#include "error-handler.hh"
#include "encoding.hh"

bool
ViewHandler::handle(mimosa::http::RequestReader & request,
                    mimosa::http::ResponseWriter & response) const
{
  std::string  content;
  int          encoding;
  const auto & query = request.query();

  auto it = query.find("id");
  if (it != query.end())
  {
    mimosa::sqlite::Stmt stmt;
    stmt.prepare(Db::handle(),
                 "select content, encoding from paste where paste_id = ?");
    stmt.bind(it->second);
    if (!stmt.fetch(&content, &encoding))
    {
      mimosa::log::error("not found");
      return errorHandler(response, "Paste not found, take an other binouse!");
    }

    if (encoding == kLzma)
    {
      mimosa::stream::StringStream::Ptr str = new mimosa::stream::StringStream();
      mimosa::stream::LzmaDecoder::Ptr lzma = new mimosa::stream::LzmaDecoder(str);
      lzma->loopWrite(content.data(), content.size());
      lzma->flush();
      content = str->str();
    }
  }

  auto tpl = loadTpl("page.html");
  if (!tpl)
    return false;

  mimosa::tpl::Dict dict;

  auto tpl_body = loadTpl("view.html");
  if (!tpl_body)
    return false;
  dict.append("body", tpl_body);
  dict.append("content", content);

  setPageHeader(dict);
  setPageFooter(dict);

  response.status_ = mimosa::http::kStatusOk;
  response.content_type_ = "text/html";
  response.sendHeader();
  tpl->execute(&response, dict);
  return true;
}
