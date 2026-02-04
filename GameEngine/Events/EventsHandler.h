#pragma once

#include "Event.h"
#include "Errors/Errors.h"
#include "ge_expected"
#include <unordered_map>
#include <functional>
#include <vector>

namespace ge {
    template <typename E, typename U>
    using Exp = util::expected<E, U>;
    using HandlerFn = std::function<void(Event&)>;

    class EventHandler {
    public:
        EventHandler();
        ~EventHandler();

        Exp<void, errors::EventErrors> HandleEvent(Event& e);
        template <class TEvent>
        static Exp<void, errors::EventErrors> SubmitCallbackFn(std::function<void(TEvent&)> fn) {
            m_EventHandlers[TEvent::GetEventType()].push_back(
                [fn = std::move(fn)](Event& e) { fn(static_cast<TEvent&>(e)); });
            return {};
        }

    private:
        static inline std::unordered_map<EventType, std::vector<HandlerFn>> m_EventHandlers;
    };
} // namespace ge