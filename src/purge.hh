#ifndef PURGE_HH
# define PURGE_HH

# include <mimosa/container/purge.hh>

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
  void newPaste(uint32_t size);

private:
  struct Paste
  {
    uint32_t id_;
    uint32_t size_;
  };

  uint64_t          total_size_;
  std::deque<Paste> odest_pastes_;
};

#endif /* !PURGE_HH */
