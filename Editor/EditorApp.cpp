#include "Core/Core.h"
#include "Core/GameEngine.h"
#include "EditorLayer.h"
#include "EntryPoint.h"

namespace ge
{
namespace
{
GameEngineConfig CreateEditorConfig()
{
  GameEngineSpecification spec;
  spec.EnableEditorUI    = true;
  spec.EditorUI.FontPath = "engine://Editor/Assets/Fonts/JetBrainsMonoNerdFont-Regular.ttf";
  spec.EditorUI.FontSize = 18.0f;
  return GameEngineConfig(spec);
}
} // namespace

class EditorApp : public GameEngine
{
public:
  EditorApp() : GameEngine(CreateEditorConfig()) {}
};
} // namespace ge

ge::Expected<ge::Unique<ge::GameEngine>, ge::errors::EngineError>
ge::CreateGameEngine()
{
  auto app    = CreateUnique<EditorApp>();
  auto result = app->Init();
  if (!result) {
    return Unexpected(result.error());
  }

  if (app->GetConfig().GetGameEngineSpec().EnableEditorUI) {
    app->PushOverlay(CreateUnique<EditorLayer>());
  }
  return std::move(app);
}
