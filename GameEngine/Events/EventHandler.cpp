#include "EventHandler.h"
#include "Core/Base.h"
#include "Logging/Logging.h"

namespace ge
{
EventHandler::EventHandler() { CORE_LOG_INFO("EventHandler Init"); }
EventHandler::~EventHandler() { CORE_LOG_INFO("EventHandler Shutdown"); }

void EventHandler::QueueEvent(Unique<Event> event)
{
  m_EventQueue_.QueueEvent(std::move(event));
}

bool EventHandler::TryDequeueEvent(Unique<Event>& event)
{
  return m_EventQueue_.TryDequeueEvent(event);
}
} // namespace ge
