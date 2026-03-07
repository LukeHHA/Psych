#include "EventHandler.h"
#include "Core/Base.h"
#include "Debug/Instrumentor.h"
#include "Logging/Logging.h"

namespace ge
{
EventHandler::EventHandler() { CORE_LOG_INFO("EventHandler Init"); }
EventHandler::~EventHandler() { CORE_LOG_INFO("EventHandler Shutdown"); }

void EventHandler::QueueEvent(Unique<Event> event)
{
  CORE_PROFILE_FUNCTION();
  m_EventQueue_.QueueEvent(std::move(event));
}

bool EventHandler::TryDequeueEvent(Unique<Event>& event)
{
  CORE_PROFILE_FUNCTION();
  return m_EventQueue_.TryDequeueEvent(event);
}
} // namespace ge
