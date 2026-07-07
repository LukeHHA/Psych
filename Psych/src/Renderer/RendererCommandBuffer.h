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
