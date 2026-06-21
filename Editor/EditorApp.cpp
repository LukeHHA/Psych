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
  spec.EnableImGui = true;
  return CreateUnique<EditorApp>(spec);
}
