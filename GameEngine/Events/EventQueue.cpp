#include "EventQueue.h"
#include "Debug/Instrumentor.h"
#include "Logging/Logging.h"

namespace ge
{
EventQueue::EventQueue() { CORE_LOG_INFO("EventQueue Init"); }
EventQueue::~EventQueue() { CORE_LOG_INFO("EventQueue Shutdown"); }

void EventQueue::QueueEvent(Unique<Event> event)
{
  CORE_PROFILE_FUNCTION();
  m_EventQueue_.push_back(std::move(event));
}

bool EventQueue::TryDequeueEvent(Unique<Event>& event)
{
  CORE_PROFILE_FUNCTION();
  if (m_EventQueue_.empty()) {
    return false;
  }
  event = std::move(m_EventQueue_.front());
  m_EventQueue_.pop_front();
  return true;
}
} // namespace ge
