#include "EditorMenuBar.h"
#include "Core/PsychEngine.h"
#include "Events/Event.h"
#include "Events/EventHandler.h"
#include "imgui.h"
#include <utility>

namespace psych::ui
{
Unique<EditorMenuBarPanel> EditorMenuBarPanel::Create() { return CreateUnique<EditorMenuBarPanel>(); }

std::string EditorMenuBarPanel::GetID() { return "EditorMenuBarPanel"; }

void EditorMenuBarPanel::OnImGuiRender() { MainMenuBar(); }

void EditorMenuBarPanel::OnUpdate() {}

void EditorMenuBarPanel::ShowExampleMenuFile()
{
  if (ImGui::MenuItem("File")) {
  }
  if (ImGui::MenuItem("Open", "Ctrl+O")) {
  }
  if (ImGui::BeginMenu("Open Recent")) {
    ImGui::MenuItem("fish_hat.c");
    ImGui::MenuItem("fish_hat.inl");
    ImGui::MenuItem("fish_hat.h");
    if (ImGui::BeginMenu("More..")) {
      ImGui::MenuItem("Hello");
      ImGui::MenuItem("Sailor");
      if (ImGui::BeginMenu("Recurse..")) {
        ShowExampleMenuFile();
        ImGui::EndMenu();
      }
      ImGui::EndMenu();
    }
    ImGui::EndMenu();
  }
  if (ImGui::MenuItem("Save", "Ctrl+S")) {
  }
  if (ImGui::MenuItem("Save As..")) {
  }

  ImGui::Separator();
  if (ImGui::BeginMenu("Options")) {
    ImGui::MenuItem("Enabled", "", &m_OptionsEnabled_);
    ImGui::BeginChild("child", ImVec2(0, 60), ImGuiChildFlags_Borders);
    for (int i = 0; i < 10; i++) {
      ImGui::Text("Scrolling Text %d", i);
    }
    ImGui::EndChild();
    ImGui::SliderFloat("Value", &m_OptionValue_, 0.0f, 1.0f);
    ImGui::InputFloat("Input", &m_OptionValue_, 0.1f);
    ImGui::Combo("Combo", &m_OptionSelection_, "Yes\0No\0Maybe\0\0");
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Colors")) {
    float sz = ImGui::GetTextLineHeight();
    for (int i = 0; i < ImGuiCol_COUNT; i++) {
      const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
      ImVec2 p         = ImGui::GetCursorScreenPos();
      ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
      ImGui::Dummy(ImVec2(sz, sz));
      ImGui::SameLine();
      ImGui::MenuItem(name);
    }
    ImGui::EndMenu();
  }

  // Here we demonstrate appending again to the "Options" menu (which we already
  // created above) Of course in this demo it is a little bit silly that this
  // function calls BeginMenu("Options") twice. In a real code-base using it
  // would make senses to use this feature from very different code locations.
  if (ImGui::BeginMenu("Options")) // <-- Append!
  {
    ImGui::Checkbox("SomeOption", &m_OptionChecked_);
    ImGui::EndMenu();
  }

  if (ImGui::BeginMenu("Disabled", false)) // Disabled
  {
    IM_ASSERT(0);
  }
  if (ImGui::MenuItem("Checked", nullptr, true)) {
  }
  ImGui::Separator();
  bool selected = false;
  if (ImGui::MenuItem("Quit", "Alt+F4", &selected)) {
    if (selected) {
      auto event = CreateUnique<WindowCloseEvent>();
      PsychEngine::Get().GetEventHandler().QueueEvent(std::move(event));
    }
  }
}

void EditorMenuBarPanel::WindowMenuBar()
{
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      ShowExampleMenuFile();
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
      ImGui::EndMenu();
    }
    // if (ImGui::MenuItem("MenuItem")) {} // You can also use MenuItem() inside
    // a menu bar!
    if (ImGui::BeginMenu("Tools")) {
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }
}

void EditorMenuBarPanel::MainMenuBar()
{
  if (ImGui::BeginMainMenuBar()) {
    if (ImGui::BeginMenu("File")) {
      ShowExampleMenuFile();
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Edit")) {
      ImGui::EndMenu();
    }
    if (ImGui::BeginMenu("Tools")) {
      ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();
  }
}
} // namespace psych::ui
