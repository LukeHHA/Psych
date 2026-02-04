#pragma once

#include "Layers/LayerStack.h"
#include "Core/Base.h"
#include "Core/Window.h"
#include "Errors/Errors.h"
#include "ge_expected"
#include "Events/EventsHandler.h"

namespace ge {
    struct GameEngineSpecification {
        std::string Name = "Application";
    };

    class GameEngine {
    public:
        GameEngine(const GameEngineSpecification& specification = GameEngineSpecification());
        virtual ~GameEngine();
        GameEngine(const GameEngine& other) = delete;
        GameEngine(GameEngine&& other) = delete;
        GameEngine& operator=(GameEngine&& other) = delete;
        GameEngine& operator=(const GameEngine& other) = delete;
        util::expected<void, errors::EngineError> Init();
        util::expected<void, errors::EngineError> Shutdown();
        void Run();
        void Stop();
        void PushLayer(std::unique_ptr<Layer> layer);
        void PushOverlay(std::unique_ptr<Layer> layer);
        Window& GetWindow() {
            CORE_ASSERT(m_Window != nullptr, "Call to: GetWindow() failed. m_Window is nullptr!");
            return *m_Window;
        }
        static GameEngine& Get();

    private:
        GameEngineSpecification m_Specification;
        bool m_Running = false;
        static GameEngine* s_Application;
        LayerStack m_LayerStack;
        Shared<Window> m_Window;
        Shared<EventHandler> m_EventHandler;
    };
    Unique<GameEngine> CreateGameEngine();
} // namespace ge
