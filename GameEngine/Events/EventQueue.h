#pragma once

#include "Core/Core.h"
#include "Events/Event.h"
#include <deque>
namespace ge
{
class EventQueue
{
public:
  EventQueue();
  ~EventQueue();

  void QueueEvent(Unique<Event> event);
  bool TryDequeueEvent(Unique<Event>& event);

  bool Empty() const { return m_EventQueue_.empty(); }

  // Iterators
  using container = std::deque<Unique<Event>>;
  using it        = container::iterator;
  using rit       = container::reverse_iterator;
  using cit       = container::const_iterator;
  using crit      = container::const_reverse_iterator;

  it begin() { return m_EventQueue_.begin(); }
  it end() { return m_EventQueue_.end(); }
  rit rbegin() { return m_EventQueue_.rbegin(); }
  rit rend() { return m_EventQueue_.rend(); }

  cit begin() const { return m_EventQueue_.begin(); }
  cit end() const { return m_EventQueue_.end(); }
  crit rbegin() const { return m_EventQueue_.rbegin(); }
  crit rend() const { return m_EventQueue_.rend(); }

private:
  std::deque<Unique<Event>> m_EventQueue_;
};
} // namespace ge
