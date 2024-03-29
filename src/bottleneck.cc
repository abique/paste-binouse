#include <mimosa/time.hh>

#include "bottleneck.hh"
#include "config.hh"

Bottleneck::Bottleneck()
{
  thread_.start([this] { this->run(); });
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

  while (true)
  {
    if (!channel_->pop(future))
      return;

    future->set(true);
    future = nullptr;

    mimosa::sleep(mimosa::millisecond * Config::msSleep());
  }
}
