#include "EventsHandler.h"

namespace ge {
    EventHandler::EventHandler() {}
    EventHandler::~EventHandler() {}

    Exp<void, errors::EventErrors> EventHandler::HandleEvent(Event& e) {
        auto it = m_EventHandlers.find(e.GetEventType());
        if (it == m_EventHandlers.end() || it->second.empty())
            return util::unexpected(errors::EventErrors{});

        for (auto& fn : it->second)
            fn(e);

        return {};
    }
} // namespace ge