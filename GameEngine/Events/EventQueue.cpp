#include "EventQueue.h"
#include "Logging/Logging.h"

namespace ge
{
EventQueue::EventQueue() { CORE_LOG_INFO("EventQueue Init"); }
EventQueue::~EventQueue() { CORE_LOG_INFO("EventQueue Shutdown"); }

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
