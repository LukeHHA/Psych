/*  EXAMPLE: Google Tests
    ASSERT_EQ(x.size(), y.size()) << "Vectors x and y are of unequal length";

    for (int i = 0; i < x.size(); ++i) {
        EXPECT_EQ(x[i], y[i]) << "Vectors x and y differ at index " << i;
    }

    TEST() MACRO Signature
    TEST(SUIT, TEST NAME)

    All assertion macros support streaming a custom failure message into them with the << operator, for example:
    EXPECT_TRUE(my_condition) << "My condition is not true";
*/

#include <gtest/gtest.h>
#define GE_ENTRYPOINT_IMPL
#include <memory>

#include "Core.h"

class AppLayer : public ge::Layer {
public:
    ~AppLayer() override = default;

    void OnAttach() override {}
    void OnDetach() override {}
    void OnUpdate(float) override {}
    void OnRender() override {}
};

class App : public ge::GameEngine {
public:
    App() {
        CORE_PROFILE_FUNCTION();
        CORE_LOG_INFO("Initializing App");
        PushLayer(std::move(std::make_unique<AppLayer>()));
    }

    ~App() {
        CORE_PROFILE_FUNCTION();
        CORE_LOG_INFO("App Shutdown successfully");
    }
};

std::unique_ptr<ge::GameEngine> ge::CreateGameEngine() {
    CORE_PROFILE_FUNCTION();
    return std::make_unique<App>();
}

// If CORE_ASSERT aborts/terminates, death tests can validate it.
// If your assert throws exceptions instead, swap ASSERT_DEATH -> ASSERT_THROW accordingly.

// 1) CreateGameEngine must return a valid unique_ptr
TEST(EngineLifecycle, CreateReturnsNonNull) {
    ge::Log::Init();
    auto app = ge::CreateGameEngine();
    ASSERT_TRUE(app) << "CreateGameEngine() returned nullptr";
}

// 2) GameEngine::Get() must return the same instance that was created
TEST(EngineSingleton, GetReturnsSameInstance) {
    ge::Log::Init();
    auto app = ge::CreateGameEngine();
    ge::GameEngine& singleton = ge::GameEngine::Get();
    ASSERT_EQ(&singleton, app.get()) << "GameEngine::Get() did not return the active instance";
}

// 3) Destroying the engine must reset singleton so a new one can be created
TEST(EngineLifecycle, DestructionResetsSingleton) {
    ge::Log::Init();
    ge::GameEngine* firstPtr = nullptr;

    {
        auto app = ge::CreateGameEngine();
        firstPtr = app.get();
        ASSERT_EQ(&ge::GameEngine::Get(), firstPtr);
    } // app destroyed here, should clear s_Application

    auto app2 = ge::CreateGameEngine();
    ASSERT_TRUE(app2);
    ASSERT_NE(app2.get(), firstPtr) << "New engine instance reused same address (possible but unlikely)";

    ASSERT_EQ(&ge::GameEngine::Get(), app2.get())
        << "Singleton did not point at the new instance after recreating";
}

// 4) Calling Get() before any engine exists should assert (death test)
TEST(EngineSingleton, GetWithoutInstanceDies) {
    ge::Log::Init();
    // Only valid if your CORE_ASSERT causes process termination.
    ASSERT_DEATH(
        {
            (void)ge::GameEngine::Get();
        },
        "");
}

// 5) Creating a second engine while one exists should assert (death test)
TEST(EngineSingleton, SecondInstanceDies) {
    ge::Log::Init();
    auto app = ge::CreateGameEngine();
    ASSERT_TRUE(app);

    ASSERT_DEATH(
        {
            auto app2 = ge::CreateGameEngine();
            (void)app2;
        },
        "");
}
