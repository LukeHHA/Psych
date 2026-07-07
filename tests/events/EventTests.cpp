#include "Events/Event.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvents.h"
#include <gtest/gtest.h>

namespace
{
TEST(EventTests, WindowResizeStoresDimensionsAndCategory)
{
  psych::WindowResizeEvent event(1280, 720);

  EXPECT_EQ(event.GetEventType(), psych::EventType::WindowResize);
  EXPECT_STREQ(event.GetName(), "WindowResize");
  EXPECT_EQ(event.GetWidth(), 1280u);
  EXPECT_EQ(event.GetHeight(), 720u);
  EXPECT_TRUE(event.IsInCategory(psych::EventCategoryApplication));
  EXPECT_FALSE(event.IsInCategory(psych::EventCategoryInput));
  EXPECT_EQ(event.ToString(), "WindowResizeEvent: 1280, 720");
}

TEST(EventTests, KeyPressedReportsKeyRepeatAndCategories)
{
  psych::KeyPressedEvent event(psych::Key::A, true);

  EXPECT_EQ(event.GetEventType(), psych::EventType::KeyPressed);
  EXPECT_EQ(event.GetKeyCode(), psych::Key::A);
  EXPECT_TRUE(event.IsRepeat());
  EXPECT_TRUE(event.IsInCategory(psych::EventCategoryKeyboard));
  EXPECT_TRUE(event.IsInCategory(psych::EventCategoryInput));
  EXPECT_FALSE(event.IsInCategory(psych::EventCategoryMouse));
  EXPECT_EQ(event.ToString(), "KeyPressedEvent: 65 (repeat = 1)");
}

TEST(EventTests, MouseButtonPressedReportsButtonAndCategories)
{
  psych::MouseButtonPressedEvent event(psych::Mouse::ButtonRight);

  EXPECT_EQ(event.GetEventType(), psych::EventType::MouseButtonPressed);
  EXPECT_EQ(event.GetMouseButton(), psych::Mouse::ButtonRight);
  EXPECT_TRUE(event.IsInCategory(psych::EventCategoryMouse));
  EXPECT_TRUE(event.IsInCategory(psych::EventCategoryInput));
  EXPECT_TRUE(event.IsInCategory(psych::EventCategoryMouseButton));
  EXPECT_FALSE(event.IsInCategory(psych::EventCategoryKeyboard));
}

TEST(EventTests, DispatcherInvokesOnlyMatchingEventType)
{
  psych::WindowResizeEvent event(640, 480);
  psych::EventDispatcher dispatcher(event);

  bool keyHandlerCalled    = false;
  bool resizeHandlerCalled = false;

  EXPECT_FALSE(dispatcher.Dispatch<psych::KeyPressedEvent>([&](psych::KeyPressedEvent&) {
    keyHandlerCalled = true;
    return true;
  }));

  EXPECT_TRUE(dispatcher.Dispatch<psych::WindowResizeEvent>([&](psych::WindowResizeEvent& resizeEvent) {
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
  psych::WindowCloseEvent event;
  psych::EventDispatcher dispatcher(event);

  EXPECT_TRUE(dispatcher.Dispatch<psych::WindowCloseEvent>([](psych::WindowCloseEvent&) { return false; }));
  EXPECT_FALSE(event.Handled);
}
} // namespace
