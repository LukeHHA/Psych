#pragma once

namespace psych
{
class Event;

class Panel
{
public:
  Panel()                        = default;
  Panel(const Panel&)            = default;
  Panel(Panel&&)                 = delete;
  Panel& operator=(const Panel&) = default;
  Panel& operator=(Panel&&)      = delete;
  virtual ~Panel()               = default;

  virtual void OnImGuiRender()   = 0;
  virtual void OnUpdate()        = 0;
  virtual void OnEvent(Event*) {}
};
} // namespace psych
