 
/**************************************************************************/
/*  OpenglFramebuffer.cpp                                                 */                                                            
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

#include "OpenglFramebuffer.h"
#include "Core/PsychEngine.h"
#include "Core/glad_glfw_incl.h"
#include "Debug/Instrumentor.h"

namespace psych
{
OpenglFramebuffer::OpenglFramebuffer(uint32_t width, uint32_t height) : m_Width(width), m_Height(height)
{
  CORE_PROFILE_FUNCTION();
  Invalidate();
}

OpenglFramebuffer::~OpenglFramebuffer()
{
  glDeleteFramebuffers(1, &m_RendererID);
  glDeleteTextures(1, &m_ColorAttachmentID);
  glDeleteRenderbuffers(1, &m_DepthAttachmentID);
}

void OpenglFramebuffer::Bind() const
{
  CORE_PROFILE_FUNCTION();
  glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);
  glViewport(0, 0, m_Width, m_Height);
}

void OpenglFramebuffer::Unbind() const
{
  CORE_PROFILE_FUNCTION();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, PsychEngine::Get().GetWindow().GetWidth(), PsychEngine::Get().GetWindow().GetHeight());
}

void OpenglFramebuffer::Invalidate()
{
  if (m_RendererID) {
    glDeleteFramebuffers(1, &m_RendererID);
    glDeleteTextures(1, &m_ColorAttachmentID);
    glDeleteRenderbuffers(1, &m_DepthAttachmentID);

    m_RendererID        = 0;
    m_ColorAttachmentID = 0;
    m_DepthAttachmentID = 0;
  }

  glGenFramebuffers(1, &m_RendererID);
  glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);

  glGenTextures(1, &m_ColorAttachmentID);
  glBindTexture(GL_TEXTURE_2D, m_ColorAttachmentID);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachmentID, 0);

  glGenRenderbuffers(1, &m_DepthAttachmentID);
  glBindRenderbuffer(GL_RENDERBUFFER, m_DepthAttachmentID);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, m_Width, m_Height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_DepthAttachmentID);

  CheckFramebufferComplete();
  glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenglFramebuffer::Resize(uint32_t width, uint32_t height)
{
  if (width == 0 || height == 0)
    return;

  m_Width  = width;
  m_Height = height;

  Invalidate();
}

uint32_t OpenglFramebuffer::GetColorAttachmentID(uint32_t index) const { return m_ColorAttachmentID; }
} // namespace psych
