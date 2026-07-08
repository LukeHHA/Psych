 
/**************************************************************************/
/*  EventQueue.cpp                                                        */                                                            
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

#include "EventQueue.h"
#include "Debug/Instrumentor.h"
#include "Logging/Logging.h"

namespace psych
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
} // namespace psych
