#include "Core/Core.h"
#include "Core/PsychEngine.h"
#include "EditorLayer.h"
#include "EntryPoint.h"

namespace psych
{

class EditorApp : public PsychEngine
{
public:
  EditorApp() = default;
};
} // namespace psych

psych::Expected<psych::Unique<psych::PsychEngine>, psych::errors::EngineError> psych::CreatePsychEngine()
{
  auto app    = psych::CreateUnique<psych::EditorApp>();
  auto result = app->Init();
  if (!result) {
    return psych::Unexpected(result.error());
  }

  // If building as an editor why would you not push the overlay?
  if (app->GetConfig().GetPsychEngineSpec().EnableEditorUI) {
    app->PushOverlay(psych::CreateUnique<psych::EditorLayer>());
  }
  return std::move(app);
}
