#ifndef PURGE_HH
# define PURGE_HH

# include <deque>

# include <mimosa/sync/channel.hh>
# include <mimosa/runtime/thread.hh>
# include <mimosa/container/singleton.hh>

/**
 * init:
 * load the N oldest pastes
 * load the total size of the pastes
 *
 * run:
 * call newPost() for each new posts,
 * when total_size > Config::maxPasteSize(), purge old pastes
 * when oldest_pastes_ is empty, reload oldest pastes
 */
class Purge : public mimosa::container::Singleton<Purge>
{
public:
  Purge();
  ~Purge();

  void newPaste(uint32_t size);

private:
  void computeTotalSize();
  void loadOldestPastes();
  void cleanup();
  void run();

  struct Paste
  {
    inline Paste(int64_t id, int64_t size) : id_(id), size_(size) {}

    uint64_t id_;
    uint64_t size_;
  };

  typedef mimosa::sync::Channel<uint32_t> channel_type;

  mimosa::runtime::Thread thread_;
  channel_type::Ptr       channel_;
  uint64_t                total_size_;
  std::deque<Paste>       oldest_pastes_;
};

#endif /* !PURGE_HH */
