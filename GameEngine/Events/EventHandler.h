#pragma once

#include "Events/Event.h"
#include "Events/EventQueue.h"

namespace ge
{
class EventHandler
{
public:
  EventHandler();
  ~EventHandler();

  void QueueEvent(Unique<Event> event);
  bool TryDequeueEvent(Unique<Event>& event);

  bool Empty() const { return m_EventQueue_.Empty(); }

private:
  EventQueue m_EventQueue_;
};
} // namespace ge
