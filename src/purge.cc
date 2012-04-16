#include <mimosa/sqlite/sqlite.hh>

#include "config.hh"
#include "db.hh"
#include "purge.hh"

Purge::Purge()
  : thread_([this] { run(); }),
    channel_(new channel_type),
    total_size_(0),
    oldest_pastes_()
{
  computeTotalSize();
  loadOldestPastes();
  thread_.start();
}

Purge::~Purge()
{
  channel_->close();
  thread_.join();
}

void
Purge::computeTotalSize()
{
  mimosa::sqlite::Stmt stmt;
  stmt.prepare(Db::handle(), "select sum(length(content)) from paste")
    .fetch(&total_size_);
}

void
Purge::loadOldestPastes()
{
  mimosa::sqlite::Stmt stmt;
  stmt.prepare(Db::handle(),
               "select paste_id, length(content) from paste"
               " order by paste_id"
               " limit 100");

  int64_t paste_id, length;
  while (stmt.fetch(&paste_id, &length))
    oldest_pastes_.push_back(Paste(paste_id, length));
}

void
Purge::cleanup()
{
  Paste paste(0, 0);

  while (total_size_ > Config::maxPasteSize())
  {
    if (oldest_pastes_.empty())
      loadOldestPastes();
    assert(!oldest_pastes_.empty());

    paste = oldest_pastes_.front();
    total_size_ -= paste.size_;
    if (total_size_ <= Config::maxPasteSize())
      break;
    oldest_pastes_.pop_front();
  }

  mimosa::sqlite::Stmt stmt;
  stmt.prepare(Db::handle(), "delete from paste where paste_id <= ?")
    .bind(paste.id_).exec();
}

void
Purge::run()
{
  uint32_t size;

  while (true)
  {
    if (!channel_->pop(size))
      return;

    do {
      total_size_ += size;
    } while (channel_->tryPop(size));

    if (total_size_ > Config::maxPasteSize())
      cleanup();
  }
}

void
Purge::newPaste(uint32_t size)
{
  channel_->push(size);
}
