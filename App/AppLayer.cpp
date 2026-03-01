#include "AppLayer.h"
#include "Debug/Instrumentor.h"
#include "Renderer/Renderer.h"

AppLayer::AppLayer() : ge::Layer("AppLayer") { CORE_PROFILE_FUNCTION(); }

AppLayer::~AppLayer() { CORE_PROFILE_FUNCTION(); }

void AppLayer::OnUpdate(float ts)
{
  CORE_PROFILE_FUNCTION();
  ge::Renderer::SetClearColour({0.9f, 0.9f, 0.9f});
}
void AppLayer::OnEvent(ge::Event& event) { CORE_PROFILE_FUNCTION(); }

void AppLayer::OnRender() { CORE_PROFILE_FUNCTION(); }

void AppLayer::OnAttach() { CORE_PROFILE_FUNCTION(); }

void AppLayer::OnDetach() { CORE_PROFILE_FUNCTION(); }

void AppLayer::OnImGuiRender() {}
