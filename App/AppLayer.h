#pragma once

#include "Core.h"

class AppLayer : public ge::Layer
{
public:
  AppLayer();
  virtual ~AppLayer();

  virtual void OnAttach() override;
  virtual void OnDetach() override;
  virtual void OnEvent(ge::Event& event) override;
  virtual void OnUpdate(float ts = 1) override;
  virtual void OnRender() override;

private:
};
