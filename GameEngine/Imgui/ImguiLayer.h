#pragma once

#include "Layers/Layer.h"
#include <cstdint>
namespace ge
{
class IImGuiLayer : public Layer
{
public:
  using Layer::Layer;
  virtual ~IImGuiLayer()                     = default;

  virtual void Begin()                       = 0;
  virtual void End()                         = 0;
  virtual void BlockEvents(bool block)       = 0;
  virtual void SetDarkThemeColors()          = 0;
  virtual uint32_t GetActiveWidgetID() const = 0;
};

class ImGuiLayer : public IImGuiLayer
{
public:
  ImGuiLayer();
  ~ImGuiLayer() = default;

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnEvent(Event& e) override;
  virtual void OnUpdate(float ts = 1) override {}
  virtual void OnRender() override {}

  void Begin() override;
  void End() override;

  void OnImGuiRender() override;

  void BlockEvents(bool block) override { m_BlockEvents = block; }

  void SetDarkThemeColors() override;

  uint32_t GetActiveWidgetID() const override;

private:
  bool m_BlockEvents = true;
};

class NullImguiLayer : public IImGuiLayer
{

public:
  NullImguiLayer()  = default;
  ~NullImguiLayer() = default;

  virtual void OnAttach() override {}
  virtual void OnDetach() override {}
  virtual void OnEvent(Event& e) override {}
  virtual void OnUpdate(float ts = 1) override {}
  virtual void OnRender() override {}

  void Begin() override {}
  void End() override {}

  void OnImGuiRender() override {}

  void BlockEvents(bool block) override { m_BlockEvents = block; }

  void SetDarkThemeColors() override {}

  uint32_t GetActiveWidgetID() const override { return 1; }

private:
  bool m_BlockEvents = true;
};
} // namespace ge
