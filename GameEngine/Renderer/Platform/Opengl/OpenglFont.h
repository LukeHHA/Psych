#pragma once

#include "../../Font.h"

namespace ge
{
class OpenglFont : public Font
{
public:
    OpenglFont();
    ~OpenglFont() override;

    const std::string& GetName() const override;
};
} // namespace ge
