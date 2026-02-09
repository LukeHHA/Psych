#pragma once

namespace ge {

    enum class EventType {
        WindowResizeEvent,
    };

    class Event {
    public:
        Event() = default;
        virtual ~Event() = default;

        virtual EventType GetEventType() = 0;
    };

    class WindowResizeEvent : public Event {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
            : m_Width(width), m_Height(height) {}

        virtual EventType GetEventType() override {
            return EventType::WindowResizeEvent;
        }

        unsigned int GetWidth() const { return m_Width; }
        unsigned int GetHeight() const { return m_Height; }

    private:
        unsigned int m_Width, m_Height;
    };

} // namespace ge