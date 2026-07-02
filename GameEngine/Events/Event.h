#pragma once

#include <sstream>
#include <string>

namespace ge
{

#define BIT(x) (1 << x)

enum class EventType {
  None = 0,
  WindowClose,
  WindowShouldClose,
  WindowResize,
  WindowFocus,
  WindowLostFocus,
  WindowMoved,
  FramebufferResize,
  AppTick,
  AppUpdate,
  AppRender,
  KeyPressed,
  KeyReleased,
  KeyTyped,
  MouseButtonPressed,
  MouseButtonReleased,
  MouseMoved,
  MouseScrolled
};

enum EventCategory {
  None                     = 0,
  EventCategoryApplication = BIT(0),
  EventCategoryInput       = BIT(1),
  EventCategoryKeyboard    = BIT(2),
  EventCategoryMouse       = BIT(3),
  EventCategoryMouseButton = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                                                                                                                           \
  static EventType GetStaticType() { return EventType::type; }                                                                                                           \
  virtual EventType GetEventType() const override { return GetStaticType(); }                                                                                            \
  virtual const char* GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category)                                                                                                                                   \
  virtual int GetCategoryFlags() const override { return category; }

class Event
{
public:
  virtual ~Event()                       = default;

  bool Handled                           = false;

  virtual EventType GetEventType() const = 0;
  virtual const char* GetName() const    = 0;
  virtual int GetCategoryFlags() const   = 0;
  virtual std::string ToString() const { return GetName(); }

  bool IsInCategory(EventCategory category) { return GetCategoryFlags() & category; }
};

class EventDispatcher
{
public:
  EventDispatcher(Event& event) : m_Event(event) {}

  // F will be deduced by the compiler
  template <typename T, typename F>
  bool Dispatch(const F& func)
  {
    if (m_Event.GetEventType() == T::GetStaticType()) {
      m_Event.Handled |= func(static_cast<T&>(m_Event));
      return true;
    }
    return false;
  }

private:
  Event& m_Event;
};

inline std::ostream& operator<<(std::ostream& os, const Event& e) { return os << e.ToString(); }

class WindowResizeEvent : public Event
{
public:
  WindowResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}

  unsigned int GetWidth() const { return m_Width; }
  unsigned int GetHeight() const { return m_Height; }

  std::string ToString() const override
  {
    std::stringstream ss;
    ss << "WindowResizeEvent: " << m_Width << ", " << m_Height;
    return ss.str();
  }

  EVENT_CLASS_TYPE(WindowResize)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
  unsigned int m_Width, m_Height;
};

class FramebufferResizeEvent : public Event
{
public:
  FramebufferResizeEvent(unsigned int width, unsigned int height) : m_Width(width), m_Height(height) {}

  unsigned int GetWidth() const { return m_Width; }
  unsigned int GetHeight() const { return m_Height; }

  std::string ToString() const override
  {
    std::stringstream ss;
    ss << "FramebufferResizeEvent: " << m_Width << ", " << m_Height;
    return ss.str();
  }

  EVENT_CLASS_TYPE(FramebufferResize)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
private:
  unsigned int m_Width, m_Height;
};

class WindowCloseEvent : public Event
{
public:
  WindowCloseEvent()  = default;
  ~WindowCloseEvent() = default;

  std::string ToString() const override
  {
    std::stringstream ss;
    ss << "WindowCloseEvent: ";
    return ss.str();
  }

  EVENT_CLASS_TYPE(WindowClose)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};

class WindowShouldCloseEvent : public Event
{
public:
  WindowShouldCloseEvent()  = default;
  ~WindowShouldCloseEvent() = default;

  [[nodiscard]] std::string ToString() const override
  {
    std::stringstream ss;
    ss << "WindowShouldCloseEvent: ";
    return ss.str();
  }

  EVENT_CLASS_TYPE(WindowShouldClose)
  EVENT_CLASS_CATEGORY(EventCategoryApplication)
};
} // namespace ge
