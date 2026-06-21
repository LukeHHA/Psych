#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvents.h"
#include <gtest/gtest.h>

namespace
{
TEST(EventTests, WindowResizeStoresDimensionsAndCategory)
{
  ge::WindowResizeEvent event(1280, 720);

  EXPECT_EQ(event.GetEventType(), ge::EventType::WindowResize);
  EXPECT_STREQ(event.GetName(), "WindowResize");
  EXPECT_EQ(event.GetWidth(), 1280u);
  EXPECT_EQ(event.GetHeight(), 720u);
  EXPECT_TRUE(event.IsInCategory(ge::EventCategoryApplication));
  EXPECT_FALSE(event.IsInCategory(ge::EventCategoryInput));
  EXPECT_EQ(event.ToString(), "WindowResizeEvent: 1280, 720");
}

TEST(EventTests, KeyPressedReportsKeyRepeatAndCategories)
{
  ge::KeyPressedEvent event(ge::Key::A, true);

  EXPECT_EQ(event.GetEventType(), ge::EventType::KeyPressed);
  EXPECT_EQ(event.GetKeyCode(), ge::Key::A);
  EXPECT_TRUE(event.IsRepeat());
  EXPECT_TRUE(event.IsInCategory(ge::EventCategoryKeyboard));
  EXPECT_TRUE(event.IsInCategory(ge::EventCategoryInput));
  EXPECT_FALSE(event.IsInCategory(ge::EventCategoryMouse));
  EXPECT_EQ(event.ToString(), "KeyPressedEvent: 65 (repeat = 1)");
}

TEST(EventTests, MouseButtonPressedReportsButtonAndCategories)
{
  ge::MouseButtonPressedEvent event(ge::Mouse::ButtonRight);

  EXPECT_EQ(event.GetEventType(), ge::EventType::MouseButtonPressed);
  EXPECT_EQ(event.GetMouseButton(), ge::Mouse::ButtonRight);
  EXPECT_TRUE(event.IsInCategory(ge::EventCategoryMouse));
  EXPECT_TRUE(event.IsInCategory(ge::EventCategoryInput));
  EXPECT_TRUE(event.IsInCategory(ge::EventCategoryMouseButton));
  EXPECT_FALSE(event.IsInCategory(ge::EventCategoryKeyboard));
}

TEST(EventTests, DispatcherInvokesOnlyMatchingEventType)
{
  ge::WindowResizeEvent event(640, 480);
  ge::EventDispatcher dispatcher(event);

  bool keyHandlerCalled    = false;
  bool resizeHandlerCalled = false;

  EXPECT_FALSE(dispatcher.Dispatch<ge::KeyPressedEvent>(
      [&](ge::KeyPressedEvent&) {
        keyHandlerCalled = true;
        return true;
      }));

  EXPECT_TRUE(dispatcher.Dispatch<ge::WindowResizeEvent>(
      [&](ge::WindowResizeEvent& resizeEvent) {
        resizeHandlerCalled = true;
        EXPECT_EQ(resizeEvent.GetWidth(), 640u);
        EXPECT_EQ(resizeEvent.GetHeight(), 480u);
        return true;
      }));

  EXPECT_FALSE(keyHandlerCalled);
  EXPECT_TRUE(resizeHandlerCalled);
  EXPECT_TRUE(event.Handled);
}

TEST(EventTests, DispatcherPreservesUnhandledResult)
{
  ge::WindowCloseEvent event;
  ge::EventDispatcher dispatcher(event);

  EXPECT_TRUE(dispatcher.Dispatch<ge::WindowCloseEvent>(
      [](ge::WindowCloseEvent&) { return false; }));
  EXPECT_FALSE(event.Handled);
}
} // namespace
