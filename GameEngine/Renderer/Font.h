#pragma once

#include <string>

namespace ge
{
class Font
{
public:
    virtual ~Font();

    virtual const std::string& GetName() const = 0;
};
} // namespace ge
