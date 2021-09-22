#include <mimosa/http/redirect.hh>
#include <mimosa/http/server-channel.hh>
#include <mimosa/log/log.hh>
#include <mimosa/net/print.hh>
#include <mimosa/sqlite/sqlite.hh>
#include <mimosa/tpl/dict.hh>
#include <mimosa/stream/lzma-encoder.hh>
#include <mimosa/stream/string-stream.hh>
#include <mimosa/uri/parse-query.hh>

#include "bottleneck.hh"
#include "config.hh"
#include "db.hh"
#include "encoding.hh"
#include "error-handler.hh"
#include "load-tpl.hh"
#include "page-footer.hh"
#include "page-header.hh"
#include "paste-handler.hh"
#include "purge.hh"

static void
encode(const std::string & input,
       std::string * output,
       Encoding * encoding)
{
  if (input.size() < 512)
  {
    *output = input;
    *encoding = kIdentity;
    return;
  }

  *encoding = kLzma;
  mimosa::stream::StringStream::Ptr str = new mimosa::stream::StringStream;
  mimosa::stream::LzmaEncoder::Ptr lzma = new mimosa::stream::LzmaEncoder(str.get());
  lzma->loopWrite(input.data(), input.size());
  lzma->flush();
  *output = str->str();
}

bool
PasteHandler::handle(mimosa::http::RequestReader & request,
                     mimosa::http::ResponseWriter & response) const
{
  auto contentLength = request.contentLength();
  if (contentLength > Config::maxPasteSize())
    return errorHandler(response, "paste size exceed limit");

  mimosa::stream::Buffer buffer(contentLength);
  int64_t rbytes = request.loopRead(buffer.data(), request.contentLength());
  if (rbytes != contentLength)
    return errorHandler(response, "i/o error");

  mimosa::kvs form;
  mimosa::uri::parseQuery(buffer.data(), contentLength, &form);

  std::string content_type;
  auto it = form.find("content-type");
  if (it != form.end()) {
      content_type = it->second;
  }

  it = form.find("content");
  if (it != form.end())
  {
    if (it->second.size() > Config::maxPasteSize())
      return errorHandler(response, "paste size exceed limit");

    Encoding encoding;
    std::string content;

    encode(it->second, &content, &encoding);

    mimosa::sqlite::Stmt stmt;
    stmt.prepare(Db::handle(),
                 "insert or fail into paste (content, ip, encoding, content_type)"
                 " values (?, ?, ?, ?)")
      .bind((void*)content.data(), (int)content.size(),
            mimosa::net::print(request.channel().remoteAddr(),
                               request.channel().remoteAddrLen()),
            encoding, content_type)
      .exec();

    int64_t row_id = sqlite3_last_insert_rowid(Db::handle());
    Purge::instance().newPaste(it->second.size());
    return redirect(response, mimosa::format::str("/view?id=%d", row_id));
  }

  auto tpl = loadTpl("page.html");
  if (!tpl)
    return false;

  mimosa::tpl::Dict dict;

  auto tpl_body = loadTpl("paste.html");
  if (!tpl_body)
    return false;
  dict.append("body", tpl_body);

  setPageHeader(dict);
  setPageFooter(dict);

  response.setStatus(mimosa::http::kStatusOk);
  response.setContentType("text/html");
  response.sendHeader();
  tpl->execute(&response, dict);
  return true;
}
