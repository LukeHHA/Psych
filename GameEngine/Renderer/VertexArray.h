#pragma once

namespace ge
{
class VertexArray
{
public:
    virtual ~VertexArray();

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;
};
} // namespace ge
