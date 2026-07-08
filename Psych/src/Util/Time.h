 
/**************************************************************************/
/*  Time.h                                                                */                                                            
/**************************************************************************/
/*                         This file is part of:                          */
/*                             PSYCH ENGINE                               */
/**************************************************************************/
/* Copyright (c)  Luke Howe                                               */                                                  
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

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
