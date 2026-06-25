#include "Core/Core.h"
#include "Core/GameEngine.h"
#include "EditorLayer.h"
#include "EntryPoint.h"

namespace ge
{

class EditorApp : public GameEngine
{
public:
  EditorApp() : GameEngine() {}
};
} // namespace ge

ge::Expected<ge::Unique<ge::GameEngine>, ge::errors::EngineError> ge::CreateGameEngine()
{
  auto app    = CreateUnique<EditorApp>();
  auto result = app->Init();
  if (!result) {
    return Unexpected(result.error());
  }

  // If building as an editor why would you not push the overlay?
  if (app->GetConfig().GetGameEngineSpec().EnableEditorUI) {
    app->PushOverlay(CreateUnique<EditorLayer>());
  }
  return std::move(app);
}
