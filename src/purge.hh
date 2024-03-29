#ifndef PURGE_HH
# define PURGE_HH

# include <deque>

# include <mimosa/channel.hh>
# include <mimosa/thread.hh>
# include <mimosa/singleton.hh>

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
class Purge : public mimosa::Singleton<Purge>
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

    int64_t id_;
    int64_t size_;
  };

  typedef mimosa::Channel<uint32_t> channel_type;

  mimosa::Thread    thread_;
  channel_type::Ptr channel_ = new channel_type;
  uint64_t          total_size_ = 0;
  std::deque<Paste> oldest_pastes_;
};

#endif /* !PURGE_HH */
