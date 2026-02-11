#include "EventHandler.h"
#include "Core/Base.h"

namespace ge
{
EventHandler::EventHandler() {}
EventHandler::~EventHandler() {}

void EventHandler::QueueEvent(Unique<Event> event)
{
  m_EventQueue_.QueueEvent(std::move(event));
}

bool EventHandler::TryDequeueEvent(Unique<Event>& event)
{
  return m_EventQueue_.TryDequeueEvent(event);
}
} // namespace ge
