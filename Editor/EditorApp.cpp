#include "Core/Core.h"
#include "Core/GameEngine.h"
#include "EditorLayer.h"
#include "EntryPoint.h"

namespace ge
{
class EditorApp : public GameEngine
{
public:
  EditorApp(const GameEngineSpecification& spec) : GameEngine(spec)
  {
    PushOverlay(CreateUnique<EditorLayer>());
  }
};
} // namespace ge

ge::Unique<ge::GameEngine>
ge::CreateGameEngine(ge::GameEngineSpecification& spec)
{
  spec.EnableEditorUI = true;
  spec.EditorUI.FontPath =
      "Editor/Assets/Fonts/JetBrainsMonoNerdFont-Regular.ttf";
  spec.EditorUI.FontSize = 18.0f;
  return CreateUnique<EditorApp>(spec);
}
