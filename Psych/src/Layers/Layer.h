#pragma once

#include "Events/Event.h"

namespace psych
{

class Layer
{
public:
  Layer(const std::string& name = "Layer");
  virtual ~Layer()                    = default;

  virtual void OnAttach()             = 0;
  virtual void OnDetach()             = 0;
  virtual void OnEvent(Event& event)  = 0;
  virtual void OnUpdate(float ts = 1) = 0;
  virtual void OnRender()             = 0;
  virtual void OnImGuiRender()        = 0;

private:
  const std::string m_DebugName_;
};

} // namespace psych
