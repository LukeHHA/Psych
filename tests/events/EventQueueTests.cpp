#include "Events/EventHandler.h"
#include "Events/EventQueue.h"
#include <gtest/gtest.h>

namespace
{
TEST(EventQueueTests, DequeueFromEmptyQueueReturnsFalseAndLeavesOutputEmpty)
{
  ge::EventQueue queue;
  ge::Unique<ge::Event> event;

  EXPECT_TRUE(queue.Empty());
  EXPECT_FALSE(queue.TryDequeueEvent(event));
  EXPECT_EQ(event, nullptr);
}

TEST(EventQueueTests, QueueDequeuesEventsInFifoOrder)
{
  ge::EventQueue queue;

  queue.QueueEvent(ge::CreateUnique<ge::WindowResizeEvent>(800, 600));
  queue.QueueEvent(ge::CreateUnique<ge::WindowCloseEvent>());

  ge::Unique<ge::Event> first;
  ASSERT_TRUE(queue.TryDequeueEvent(first));
  ASSERT_NE(first, nullptr);
  EXPECT_EQ(first->GetEventType(), ge::EventType::WindowResize);
  const auto& resize = static_cast<const ge::WindowResizeEvent&>(*first);
  EXPECT_EQ(resize.GetWidth(), 800u);
  EXPECT_EQ(resize.GetHeight(), 600u);

  ge::Unique<ge::Event> second;
  ASSERT_TRUE(queue.TryDequeueEvent(second));
  ASSERT_NE(second, nullptr);
  EXPECT_EQ(second->GetEventType(), ge::EventType::WindowClose);

  ge::Unique<ge::Event> third;
  EXPECT_FALSE(queue.TryDequeueEvent(third));
  EXPECT_TRUE(queue.Empty());
}

TEST(EventQueueTests, EventHandlerForwardsQueueSemantics)
{
  ge::EventHandler handler;

  EXPECT_TRUE(handler.Empty());
  handler.QueueEvent(ge::CreateUnique<ge::FramebufferResizeEvent>(1920, 1080));
  EXPECT_FALSE(handler.Empty());

  ge::Unique<ge::Event> event;
  ASSERT_TRUE(handler.TryDequeueEvent(event));
  ASSERT_NE(event, nullptr);
  EXPECT_EQ(event->GetEventType(), ge::EventType::FramebufferResize);

  const auto& resize = static_cast<const ge::FramebufferResizeEvent&>(*event);
  EXPECT_EQ(resize.GetWidth(), 1920u);
  EXPECT_EQ(resize.GetHeight(), 1080u);
  EXPECT_TRUE(handler.Empty());
}
} // namespace
