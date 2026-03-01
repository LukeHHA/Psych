#include "EditorLayer.h"
#include "imgui.h"

namespace ge
{
EditorLayer::EditorLayer() : Layer("EditorLayer") {}
void EditorLayer::OnAttach() {}
void EditorLayer::OnUpdate(float ts) {}
void EditorLayer::OnDetach() {}
void EditorLayer::OnEvent(Event& event) {}
void EditorLayer::OnRender() {}
void EditorLayer::OnImGuiRender()
{
  bool open    = true;
  auto* p_open = &open;

  struct ImGuiDemoDockspaceArgs {
    bool IsFullscreen = true;
    bool KeepWindowPadding =
        false; // Keep WindowPadding to help understand that DockSpace() is a
               // widget inside the window.
    ImGuiDockNodeFlags DockSpaceFlags = ImGuiDockNodeFlags_None;
  };
  static int opt_demo_mode          = 0;
  static bool opt_demo_mode_changed = false;
  static ImGuiDemoDockspaceArgs args;

  ImGuiDockNodeFlags dockspace_flags = args.DockSpaceFlags;

  // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window
  // not dockable into, because it would be confusing to have two docking
  // targets within each others.
  ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
  if (args.IsFullscreen) {
    // Fullscreen dockspace: practically the same as calling
    // DockSpaceOverViewport();
    const ImGuiViewport* viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                    ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |=
        ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    window_flags |= ImGuiWindowFlags_NoBackground;
  } else {
    // Floating dockspace
    dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
  }

  // Important: note that we proceed even if Begin() returns false (aka window
  // is collapsed). This is because we want to keep our DockSpace() active. If a
  // DockSpace() is inactive, all active windows docked into it will lose their
  // parent and become undocked. We cannot preserve the docking relationship
  // between an active window and an inactive docking, otherwise any change of
  // dockspace/settings would lead to windows being stuck in limbo and never
  // being visible.
  if (!args.KeepWindowPadding)
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  ImGui::Begin("Window with a DockSpace", p_open, window_flags);
  if (!args.KeepWindowPadding)
    ImGui::PopStyleVar();

  if (args.IsFullscreen)
    ImGui::PopStyleVar(2);

  // Submit the DockSpace widget inside our window
  // - Note that the id here is different from the one used by
  // DockSpaceOverViewport(), so docking state won't get transfered between
  // "Basic" and "Advanced" demos.
  // - If we made the ShowExampleAppDockSpaceBasic() calculate its own ID and
  // pass it to DockSpaceOverViewport() the ID could easily match.
  ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

  ImGui::End();

  // Refocus our window to minimize perceived loss of focus when changing mode
  // (caused by the fact that each use a different window, which would not
  // happen in a real app)
  if (opt_demo_mode_changed)
    ImGui::SetNextWindowFocus();
  ImGui::Begin("Examples: Dockspace", p_open, ImGuiWindowFlags_MenuBar);
  opt_demo_mode_changed = false;
  opt_demo_mode_changed |=
      ImGui::RadioButton("Basic demo mode", &opt_demo_mode, 0);
  opt_demo_mode_changed |=
      ImGui::RadioButton("Advanced demo mode", &opt_demo_mode, 1);

  ImGui::SeparatorText("Options");

  ImGui::Checkbox("Fullscreen", &args.IsFullscreen);
  ImGui::Checkbox("Keep Window Padding", &args.KeepWindowPadding);
  ImGui::SameLine();

  ImGui::BeginDisabled(args.IsFullscreen == false);
  ImGui::CheckboxFlags("Flag: PassthruCentralNode", &args.DockSpaceFlags,
                       ImGuiDockNodeFlags_PassthruCentralNode);
  ImGui::EndDisabled();
  ImGui::CheckboxFlags("Flag: NoDockingOverCentralNode", &args.DockSpaceFlags,
                       ImGuiDockNodeFlags_NoDockingOverCentralNode);
  ImGui::CheckboxFlags("Flag: NoDockingSplit", &args.DockSpaceFlags,
                       ImGuiDockNodeFlags_NoDockingSplit);
  ImGui::CheckboxFlags("Flag: NoUndocking", &args.DockSpaceFlags,
                       ImGuiDockNodeFlags_NoUndocking);
  ImGui::CheckboxFlags("Flag: NoResize", &args.DockSpaceFlags,
                       ImGuiDockNodeFlags_NoResize);
  ImGui::CheckboxFlags("Flag: AutoHideTabBar", &args.DockSpaceFlags,
                       ImGuiDockNodeFlags_AutoHideTabBar);

  // Show demo options and help
  if (ImGui::BeginMenuBar()) {
    if (ImGui::BeginMenu("Help")) {
      ImGui::TextUnformatted(
          "This demonstrates the use of ImGui::DockSpace() which allows you to "
          "manually\ncreate a docking node _within_ another window."
          "\n"
          "The \"Basic\" version uses the ImGui::DockSpaceOverViewport() "
          "helper. Most applications can probably use this.");
      ImGui::Separator();
      ImGui::TextUnformatted(
          "When docking is enabled, you can ALWAYS dock MOST window into "
          "another! Try it now!"
          "\n"
          "- Drag from window title bar or their tab to dock/undock."
          "\n"
          "- Drag from window menu button (upper-left button) to undock an "
          "entire node (all windows)."
          "\n"
          "- Hold SHIFT to disable docking (if io.ConfigDockingWithShift == "
          "false, default)"
          "\n"
          "- Hold SHIFT to enable docking (if io.ConfigDockingWithShift == "
          "true)");
      ImGui::Separator();
      ImGui::TextUnformatted("More details:");
      ImGui::Bullet();
      ImGui::SameLine();
      ImGui::TextLinkOpenURL("Docking Wiki page",
                             "https://github.com/ocornut/imgui/wiki/Docking");
      ImGui::BulletText("Read comments in ShowExampleAppDockSpace()");
      ImGui::EndMenu();
    }
    ImGui::EndMenuBar();
  }

  ImGui::End();
}
} // namespace ge
