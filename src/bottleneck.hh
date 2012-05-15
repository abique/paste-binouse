#ifndef BOTTLENECK_HH
# define BOTTLENECK_HH

# include <mimosa/thread.hh>
# include <mimosa/channel.hh>
# include <mimosa/future.hh>
# include <mimosa/singleton.hh>

class Bottleneck : public mimosa::Singleton<Bottleneck>
{
public:
  Bottleneck();
  ~Bottleneck();

  void enter();

private:
  void run();

  typedef mimosa::Future<bool> future_type;
  typedef mimosa::Channel<typename future_type::Ptr> channel_type;

  mimosa::Thread thread_;
  typename channel_type::Ptr channel_;
};

#endif /* !BOTTLENECK_HH */
