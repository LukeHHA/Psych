#include <chrono>
class Time
{
public:
  static void Update()
  {
    auto now = Clock::now();

    if (!s_Initialized) {
      s_LastTime    = now;
      s_Initialized = true;
      return;
    }

    std::chrono::duration<float> delta = now - s_LastTime;
    s_DeltaTime                        = delta.count();
    s_LastTime                         = now;
  }

  static float DeltaTime() { return s_DeltaTime; }

  static float FPS() { return s_DeltaTime > 0.0f ? 1.0f / s_DeltaTime : 0.0f; }

private:
  using Clock                      = std::chrono::steady_clock;

  inline static bool s_Initialized = false;
  inline static Clock::time_point s_LastTime;
  inline static float s_DeltaTime = 0.0f;
};
