#pragma once

#include "Core/Core.h"
#include "Events/Event.h"
#include <deque>
namespace psych
{
class EventQueue
{
public:
  EventQueue();
  ~EventQueue();
  EventQueue(const EventQueue&)                = delete;
  EventQueue& operator=(const EventQueue&)     = delete;
  EventQueue(EventQueue&&) noexcept            = default;
  EventQueue& operator=(EventQueue&&) noexcept = default;

  void QueueEvent(Unique<Event> event);
  bool TryDequeueEvent(Unique<Event>& event);

  [[nodiscard]] bool Empty() const { return m_EventQueue_.empty(); }

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

  [[nodiscard]] cit begin() const { return m_EventQueue_.begin(); }
  [[nodiscard]] cit end() const { return m_EventQueue_.end(); }
  [[nodiscard]] crit rbegin() const { return m_EventQueue_.rbegin(); }
  [[nodiscard]] crit rend() const { return m_EventQueue_.rend(); }

private:
  std::deque<Unique<Event>> m_EventQueue_;
};
} // namespace psych
