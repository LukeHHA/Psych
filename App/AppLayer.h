#pragma once

#include "Core.h"

class AppLayer : public Core::Layer
{
public:
    AppLayer();
    virtual ~AppLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;

    virtual void OnUpdate(float ts) override;
    virtual void OnRender() override;

private:
};
