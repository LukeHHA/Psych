#include "Events/EventHandler.h"
#include "Events/EventQueue.h"
#include <gtest/gtest.h>

namespace
{
TEST(EventQueueTests, DequeueFromEmptyQueueReturnsFalseAndLeavesOutputEmpty)
{
  psych::EventQueue queue;
  psych::Unique<psych::Event> event;

  EXPECT_TRUE(queue.Empty());
  EXPECT_FALSE(queue.TryDequeueEvent(event));
  EXPECT_EQ(event, nullptr);
}

TEST(EventQueueTests, QueueDequeuesEventsInFifoOrder)
{
  psych::EventQueue queue;

  queue.QueueEvent(psych::CreateUnique<psych::WindowResizeEvent>(800, 600));
  queue.QueueEvent(psych::CreateUnique<psych::WindowCloseEvent>());

  psych::Unique<psych::Event> first;
  ASSERT_TRUE(queue.TryDequeueEvent(first));
  ASSERT_NE(first, nullptr);
  EXPECT_EQ(first->GetEventType(), psych::EventType::WindowResize);
  const auto& resize = static_cast<const psych::WindowResizeEvent&>(*first);
  EXPECT_EQ(resize.GetWidth(), 800u);
  EXPECT_EQ(resize.GetHeight(), 600u);

  psych::Unique<psych::Event> second;
  ASSERT_TRUE(queue.TryDequeueEvent(second));
  ASSERT_NE(second, nullptr);
  EXPECT_EQ(second->GetEventType(), psych::EventType::WindowClose);

  psych::Unique<psych::Event> third;
  EXPECT_FALSE(queue.TryDequeueEvent(third));
  EXPECT_TRUE(queue.Empty());
}

TEST(EventQueueTests, EventHandlerForwardsQueueSemantics)
{
  psych::EventHandler handler;

  EXPECT_TRUE(handler.Empty());
  handler.QueueEvent(psych::CreateUnique<psych::FramebufferResizeEvent>(1920, 1080));
  EXPECT_FALSE(handler.Empty());

  psych::Unique<psych::Event> event;
  ASSERT_TRUE(handler.TryDequeueEvent(event));
  ASSERT_NE(event, nullptr);
  EXPECT_EQ(event->GetEventType(), psych::EventType::FramebufferResize);

  const auto& resize = static_cast<const psych::FramebufferResizeEvent&>(*event);
  EXPECT_EQ(resize.GetWidth(), 1920u);
  EXPECT_EQ(resize.GetHeight(), 1080u);
  EXPECT_TRUE(handler.Empty());
}
} // namespace
