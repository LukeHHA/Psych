#include "Core/Core.h"
#include "Core/GameEngine.h"
#include "EditorLayer.h"
#include "EntryPoint.h"

#include <utility>

namespace ge
{
class EditorApp : public GameEngine
{
public:
  explicit EditorApp(GameEngineConfig config) : GameEngine(std::move(config)) {}
};
} // namespace ge

ge::Expected<ge::Unique<ge::GameEngine>, ge::errors::EngineError>
ge::CreateGameEngine(ge::GameEngineSpecification& spec)
{
  spec.EnableEditorUI    = true;
  spec.EditorUI.FontPath =
      "Editor/Assets/Fonts/JetBrainsMonoNerdFont-Regular.ttf";
  spec.EditorUI.FontSize = 18.0f;

  auto app               = CreateUnique<EditorApp>(GameEngineConfig(spec));
  auto result            = app->Init();
  if (!result) {
    return Unexpected(result.error());
  }

  if (app->GetConfig().GetGameEngineSpec().EnableEditorUI) {
    app->PushOverlay(CreateUnique<EditorLayer>());
  }
  return std::move(app);
}
