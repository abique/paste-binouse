#include <mimosa/runtime/time.hh>

#include "bottleneck.hh"
#include "config.hh"

Bottleneck::Bottleneck()
  : thread_([this] { this->run(); }),
    channel_(new channel_type)
{
  thread_.start();
}

Bottleneck::~Bottleneck()
{
  channel_->close();
  thread_.join();
}

void
Bottleneck::enter()
{
  future_type::Ptr future = new future_type;

  channel_->push(future);
  future->wait();
}

void
Bottleneck::run()
{
  future_type::Ptr future;
  bool             succeed;

  while (true)
  {
    if (!channel_->pop(future))
      return;

    future->set(true);
    future = nullptr;

    mimosa::runtime::sleep(mimosa::runtime::millisecond * Config::msSleep());
  }
}
