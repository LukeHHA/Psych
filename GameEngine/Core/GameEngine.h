#pragma once

#include "Layers/LayerStack.h"
#include "Core/Base.h"
#include "Core/Window.h"
#include "Errors/Errors.h"
#include "ge_expected"

namespace ge {
    struct GameEngineSpecification {
        std::string Name = "Application";
    };

    class GameEngine {
    public:
        GameEngine(
            const GameEngineSpecification& specification = GameEngineSpecification());
        ~GameEngine();
        util::expected<void, errors::EngineError> Init();
        util::expected<void, errors::EngineError> Shutdown();

        void Run();
        void Stop();

        void PushLayer(std::unique_ptr<Layer> layer);
        void PushOverlay(std::unique_ptr<Layer> layer);

        static GameEngine& Get();

    private:
        GameEngineSpecification m_Specification;
        bool m_Running = false;
        static GameEngine* s_Application;
        LayerStack m_LayerStack;
        Shared<Window> m_Window;
    };

    Unique<GameEngine> CreateGameEngine();
} // namespace ge
