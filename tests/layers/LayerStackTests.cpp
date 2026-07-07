#include "Layers/LayerStack.h"
#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace
{
struct LayerEvents {
  std::vector<std::string> attached;
  std::vector<std::string> detached;
};

class TestLayer : public psych::Layer
{
public:
  TestLayer(std::string name, LayerEvents& events) : psych::Layer(name), m_Name_(std::move(name)), m_Events_(events) {}

  const std::string& Name() const { return m_Name_; }

  void OnAttach() override { m_Events_.attached.push_back(m_Name_); }
  void OnDetach() override { m_Events_.detached.push_back(m_Name_); }
  void OnEvent(psych::Event&) override {}
  void OnUpdate(float) override {}
  void OnRender() override {}
  void OnImGuiRender() override {}

private:
  std::string m_Name_;
  LayerEvents& m_Events_;
};

TEST(LayerStackTests, PushLayerInsertsBeforeOverlays)
{
  LayerEvents events;
  psych::LayerStack stack;

  stack.PushOverlay(std::make_unique<TestLayer>("overlay-a", events));
  stack.PushLayer(std::make_unique<TestLayer>("layer-a", events));
  stack.PushLayer(std::make_unique<TestLayer>("layer-b", events));
  stack.PushOverlay(std::make_unique<TestLayer>("overlay-b", events));

  std::vector<std::string> order;
  for (const auto& layer : stack) {
    order.push_back(static_cast<TestLayer*>(layer.get())->Name());
  }

  EXPECT_EQ(order, (std::vector<std::string>{"layer-a", "layer-b", "overlay-a", "overlay-b"}));
  EXPECT_EQ(events.attached, (std::vector<std::string>{"overlay-a", "layer-a", "layer-b", "overlay-b"}));
}

TEST(LayerStackTests, DestructorDetachesOwnedLayersInIterationOrder)
{
  LayerEvents events;

  {
    psych::LayerStack stack;
    stack.PushLayer(std::make_unique<TestLayer>("layer-a", events));
    stack.PushOverlay(std::make_unique<TestLayer>("overlay-a", events));

    EXPECT_EQ(events.attached, (std::vector<std::string>{"layer-a", "overlay-a"}));
    EXPECT_TRUE(events.detached.empty());
  }

  EXPECT_EQ(events.detached, (std::vector<std::string>{"layer-a", "overlay-a"}));
}
} // namespace
