 
/**************************************************************************/
/*  RendererCommandBuffer.h                                               */                                                            
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

#pragma once

#include "Core/Core.h"
#include "Renderer/RenderCommandQueue.h"
namespace psych
{

struct CommandBufferSpec {
  CommandBufferSpec(int buff_size = 2) : buffer_size(buff_size) {}
  int buffer_size;
};

class RendererCommandBuffer
{
public:
  RendererCommandBuffer(const CommandBufferSpec& spec = CommandBufferSpec()) : m_CommandBufferSpec_(spec)
  {
    m_CommandQueues_.reserve(spec.buffer_size);
    for (int i = 0; i < spec.buffer_size; i++) {
      m_CommandQueues_.emplace_back();
    }
    m_ActiveBuffer_ = &m_CommandQueues_.at(m_CurrentBufferIndex_);
  }

  ~RendererCommandBuffer() = default;

  void Begin() { SwapBuffers(); }
  void End()
  {
    Flush();
    m_ActiveBuffer_->clear();
  }
  void Push(auto&& func) { m_ActiveBuffer_->Push(std::forward<decltype(func)>(func)); }

  void Flush()
  {
    for (const auto& cmd : *m_ActiveBuffer_) {
      cmd();
    }
  }

  uint32_t GetCurrentIndex() const { return m_CurrentBufferIndex_; }

  void Submit(auto&& func) { m_ActiveBuffer_->Push(std::forward<decltype(func)>(func)); }

private:
  void SwapBuffers()
  {
    m_CurrentBufferIndex_ = (m_CurrentBufferIndex_ + 1) % m_CommandBufferSpec_.buffer_size;
    m_ActiveBuffer_       = &m_CommandQueues_.at(m_CurrentBufferIndex_);
  }

private:
  RenderCommandQueue* m_ActiveBuffer_ = nullptr;
  std::vector<RenderCommandQueue> m_CommandQueues_;
  uint32_t m_CurrentBufferIndex_ = 0;
  CommandBufferSpec m_CommandBufferSpec_;
};
} // namespace psych
