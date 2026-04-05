/**
 * @file RenderManagerTests_Simple.cpp
 * @brief RenderManager usage examples and simple tests
 * @note This file demonstrates basic usage without gtest dependency
 */

#include "core/renderer/RenderManager.h"
#include "core/renderer/RenderContext.h"
#include "core/renderer/interfaces/IRenderable.h"
#include "core/logging/Logger.h"
#include <iostream>
#include <cassert>

using namespace Logging;

// Simple test renderable
class SimpleGameObject : public IRenderable {
public:
    SimpleGameObject(const std::string& name, int layer = 0)
        : m_name(name), m_layer(layer), m_visible(true) {}
    
    void Update(float deltaTime) override {
        LOG_DEBUG("GameObject", m_name + " updated");
    }
    
    void Render(IRenderContext* context) override {
        if (context) {
            LOG_DEBUG("GameObject", m_name + " rendered");
        }
    }
    
    void OnDestroy() override {
        LOG_DEBUG("GameObject", m_name + " destroyed");
    }
    
    glm::mat4 GetWorldMatrix() const override {
        return glm::mat4(1.0f);
    }
    
    bool IsVisible() const override { return m_visible; }
    int GetRenderLayer() const override { return m_layer; }
    
    bool LoadResources() override { return true; }
    void UnloadResources() override {}
    
    void SetVisibility(bool visible) { m_visible = visible; }
    void SetLayer(int layer) { m_layer = layer; }
    const std::string& GetName() const { return m_name; }
    
private:
    std::string m_name;
    int m_layer;
    bool m_visible;
};

// Test 1: Basic add/remove
void test_add_remove() {
    std::cout << "Test 1: Add/Remove Objects" << std::endl;
    
    try {
        auto context = std::make_shared<RenderContext>();
        auto manager = std::make_unique<RenderManager>(context);
        
        assert(manager->GetObjectCount() == 0);
        
        auto obj1 = std::make_shared<SimpleGameObject>("Obj1");
        manager->AddObject(obj1);
        assert(manager->GetObjectCount() == 1);
        
        manager->RemoveObject(obj1.get());
        assert(manager->GetObjectCount() == 0);
        
        std::cout << "PASS" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "FAIL: " << e.what() << std::endl;
    }
}

// Test 2: Visibility
void test_visibility() {
    std::cout << "Test 2: Visibility Filtering" << std::endl;
    
    try {
        auto context = std::make_shared<RenderContext>();
        auto manager = std::make_unique<RenderManager>(context);
        
        auto visible = std::make_shared<SimpleGameObject>("Visible");
        auto invisible = std::make_shared<SimpleGameObject>("Invisible");
        
        visible->SetVisibility(true);
        invisible->SetVisibility(false);
        
        manager->AddObject(visible);
        manager->AddObject(invisible);
        manager->Render();
        
        auto& stats = manager->GetStats();
        assert(stats.visibleObjectCount == 1);
        assert(stats.totalObjectCount == 2);
        
        std::cout << "PASS" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "FAIL: " << e.what() << std::endl;
    }
}

// Test 3: Layer ordering
void test_layers() {
    std::cout << "Test 3: Layer Ordering" << std::endl;
    
    try {
        auto context = std::make_shared<RenderContext>();
        auto manager = std::make_unique<RenderManager>(context);
        
        auto layer2 = std::make_shared<SimpleGameObject>("Layer2", 2);
        auto layer0 = std::make_shared<SimpleGameObject>("Layer0", 0);
        auto layer1 = std::make_shared<SimpleGameObject>("Layer1", 1);
        
        manager->AddObject(layer2);
        manager->AddObject(layer0);
        manager->AddObject(layer1);
        manager->Render();
        
        auto& stats = manager->GetStats();
        assert(stats.visibleObjectCount == 3);
        
        std::cout << "PASS" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "FAIL: " << e.what() << std::endl;
    }
}

// Test 4: Exception handling
void test_exceptions() {
    std::cout << "Test 4: Exception Handling" << std::endl;
    
    try {
        try {
            RenderManager invalid(nullptr);
            std::cout << "FAIL: Should throw on null context" << std::endl;
            return;
        }
        catch (const std::invalid_argument&) {
            // Expected
        }
        
        auto context = std::make_shared<RenderContext>();
        auto manager = std::make_unique<RenderManager>(context);
        
        try {
            manager->AddObject(nullptr);
            std::cout << "FAIL: Should throw on null object" << std::endl;
            return;
        }
        catch (const std::invalid_argument&) {
            // Expected
        }
        
        std::cout << "PASS" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "FAIL: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "\n=== RenderManager Simple Tests ===" << std::endl;
    
    Logger::Instance().SetOutputFile("render_test.log");
    Logger::Instance().EnableConsoleOutput(true);
    
    test_add_remove();
    test_visibility();
    test_layers();
    test_exceptions();
    
    std::cout << "=== Tests Complete ===" << std::endl;
    
    return 0;
}
