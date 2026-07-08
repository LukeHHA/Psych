 
/**************************************************************************/
/*  EventQueue.h                                                          */                                                            
/**************************************************************************/
/*                         This file is part of:                          */
/*                             PSYCH ENGINE                               */
/**************************************************************************/
/* Copyright (c)  Luke Howe                                               */                                                  
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

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
