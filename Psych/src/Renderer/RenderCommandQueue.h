 
/**************************************************************************/
/*  RenderCommandQueue.h                                                  */                                                            
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

#include <functional>
#include <vector>
namespace psych
{
class RenderCommandQueue
{
public:
  using container_type         = std::vector<std::function<void()>>;
  using value_type             = typename container_type::value_type;
  using size_type              = typename container_type::size_type;
  using difference_type        = typename container_type::difference_type;

  using reference              = typename container_type::reference;
  using const_reference        = typename container_type::const_reference;

  using iterator               = typename container_type::iterator;
  using const_iterator         = typename container_type::const_iterator;

  using reverse_iterator       = typename container_type::reverse_iterator;
  using const_reverse_iterator = typename container_type::const_reverse_iterator;

public:
  void Push(auto&& func) { m_CommandQueue_.push_back(std::forward<decltype(func)>(func)); }

  // Iterators
  iterator begin() noexcept { return m_CommandQueue_.begin(); }
  const_iterator begin() const noexcept { return m_CommandQueue_.begin(); }
  const_iterator cbegin() const noexcept { return m_CommandQueue_.cbegin(); }

  iterator end() noexcept { return m_CommandQueue_.end(); }
  const_iterator end() const noexcept { return m_CommandQueue_.end(); }
  const_iterator cend() const noexcept { return m_CommandQueue_.cend(); }

  reverse_iterator rbegin() noexcept { return m_CommandQueue_.rbegin(); }
  const_reverse_iterator rbegin() const noexcept { return m_CommandQueue_.rbegin(); }
  const_reverse_iterator crbegin() const noexcept { return m_CommandQueue_.crbegin(); }

  reverse_iterator rend() noexcept { return m_CommandQueue_.rend(); }
  const_reverse_iterator rend() const noexcept { return m_CommandQueue_.rend(); }
  const_reverse_iterator crend() const noexcept { return m_CommandQueue_.crend(); }

  bool empty() const noexcept { return m_CommandQueue_.empty(); }
  size_type size() const noexcept { return m_CommandQueue_.size(); }

  void clear() noexcept { m_CommandQueue_.clear(); }

private:
  container_type m_CommandQueue_;
};
} // namespace psych
