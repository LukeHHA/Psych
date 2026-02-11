#include "EventQueue.h"

namespace ge
{
EventQueue::EventQueue() {}
EventQueue::~EventQueue() {};

void EventQueue::QueueEvent(Unique<Event> event)
{
  m_EventQueue_.push_back(std::move(event));
}

bool EventQueue::TryDequeueEvent(Unique<Event>& event)
{
  if (m_EventQueue_.empty()) {
    return false;
  }
  event = std::move(m_EventQueue_.front());
  m_EventQueue_.pop_front();
  return true;
}
} // namespace ge
