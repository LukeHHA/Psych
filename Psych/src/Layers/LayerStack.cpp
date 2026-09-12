 
/**************************************************************************/
/*  LayerStack.cpp                                                        */                                                            
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

// Modified from ge’s LayerStack (Apache 2.0);
// renamed namespace hz → Core and prefixed macros with CORE_…

#include "LayerStack.h"
#include "Debug/Instrumentor.h"

namespace psych
{
LayerStack::LayerStack()
{
  CORE_PROFILE_FUNCTION();
  CORE_LOG_INFO("LayerStack Init");
}

LayerStack::~LayerStack()
{
  CORE_PROFILE_FUNCTION();
  for (const std::unique_ptr<Layer>& layer : m_Layers) {
    layer->OnDetach();
  }
  CORE_LOG_INFO("LayerStack Shutdown");
}

void LayerStack::PushLayer(std::unique_ptr<Layer> layer)
{
  CORE_PROFILE_FUNCTION();
  layer->OnAttach();
  m_Layers.insert(m_Layers.begin() + m_LayerInsertIndex, std::move(layer));
  m_LayerInsertIndex++;
}

void LayerStack::PushOverlay(std::unique_ptr<Layer> overlay)
{
  CORE_PROFILE_FUNCTION();
  overlay->OnAttach();
  m_Layers.push_back(std::move(overlay));
}

void LayerStack::PopLayer(std::unique_ptr<Layer> layer)
{
  CORE_PROFILE_FUNCTION();
  auto it = std::find(m_Layers.begin(), m_Layers.begin() + m_LayerInsertIndex, layer);
  if (it != m_Layers.begin() + m_LayerInsertIndex) {
    layer->OnDetach();
    m_Layers.erase(it);
    m_LayerInsertIndex--;
  }
}

void LayerStack::PopOverlay(std::unique_ptr<Layer> overlay)
{
  CORE_PROFILE_FUNCTION();
  auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
  if (it != m_Layers.end()) {
    overlay->OnDetach();
    m_Layers.erase(it);
  }
}
} // namespace psych
