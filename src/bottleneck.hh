#ifndef BOTTLENECK_HH
# define BOTTLENECK_HH

# include <mimosa/runtime/thread.hh>
# include <mimosa/sync/channel.hh>
# include <mimosa/sync/future.hh>
# include <mimosa/container/singleton.hh>

class Bottleneck : public mimosa::container::Singleton<Bottleneck>
{
public:
  Bottleneck();
  ~Bottleneck();

  void enter();

private:
  void run();

  typedef mimosa::sync::Future<bool> future_type;
  typedef mimosa::sync::Channel<typename future_type::Ptr> channel_type;

  mimosa::runtime::Thread thread_;
  typename channel_type::Ptr channel_;
};

#endif /* !BOTTLENECK_HH */
