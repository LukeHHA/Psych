#include "Core/Base.h"
#include "Core/GameEngine.h"
#include "EditorLayer.h"
#include "EntryPoint.h"

namespace ge
{
class EditorApp : public GameEngine
{
public:
  EditorApp() { PushOverlay(std::move(CreateUnique<EditorLayer>())); }
};
} // namespace ge

ge::Unique<ge::GameEngine>
ge::CreateGameEngine(ge::GameEngineSpecification& spec)
{
  return CreateUnique<EditorApp>();
}
