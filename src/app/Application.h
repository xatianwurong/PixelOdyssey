#pragma once
#include "../core/ecs/World.h"
#include "../core/ecs/Component.h"
#include "../core/event/EventBus.h"
#include "../core/command/CommandStack.h"
#include "../core/renderer/Camera.h"
#include "../tools/Tool.h"
#include "../tools/SelectTool.h"
#include "../tools/MoveTool.h"
#include "../tools/RotateTool.h"
#include "../tools/ScaleTool.h"
#include "../tools/PanTool.h"
#include "../tools/Gizmo.h"
#include "../core/utils/Grid.h"
#include "../core/utils/Raycast.h"

/**
 * @brief 应用层
 * @details PixelOdyssey 应用主类，整合所有系统
 */
class Application {
public:
    static Application& Instance() {
        static Application instance;
        return instance;
    }
    
    bool Initialize() {
        m_camera.SetPosition(glm::vec3(0, 0, 5));
        m_camera.SetTarget(glm::vec3(0, 0, 0));
        
        ToolManager::Instance().SetTool(std::make_unique<SelectTool>());
        
        GridManager::Instance().GetMainGrid().SetSize(50.0f);
        GridManager::Instance().GetMainGrid().SetDivisions(100);
        GridManager::Instance().GetMainGrid().SetVisible(true);
        
        GizmoManager::Instance().SetGizmoType(GizmoType::Move);
        
        CreateTestEntities();
        
        return true;
    }
    
    void Shutdown() {
        World::Instance().Clear();
        CommandStack::Instance().Clear();
        EventBus::Instance().Clear();
    }
    
    void Update(float deltaTime) {
        ToolManager::Instance().Update(deltaTime);
        GizmoManager::Instance().Update(&m_camera, 0, 0);
    }
    
    void Render() {
        GridManager::Instance().Render();
        GizmoManager::Instance().Render();
        ToolManager::Instance().Render();
    }
    
    Camera& GetCamera() { return m_camera; }
    
    void SetTool(ToolType toolType) {
        switch (toolType) {
            case ToolType::Select:
                ToolManager::Instance().SetTool(std::make_unique<SelectTool>());
                GizmoManager::Instance().SetGizmoType(GizmoType::None);
                break;
            case ToolType::Move:
                ToolManager::Instance().SetTool(std::make_unique<MoveTool>());
                GizmoManager::Instance().SetGizmoType(GizmoType::Move);
                break;
            case ToolType::Rotate:
                ToolManager::Instance().SetTool(std::make_unique<RotateTool>());
                GizmoManager::Instance().SetGizmoType(GizmoType::Rotate);
                break;
            case ToolType::Scale:
                ToolManager::Instance().SetTool(std::make_unique<ScaleTool>());
                GizmoManager::Instance().SetGizmoType(GizmoType::Scale);
                break;
            case ToolType::Pan:
                ToolManager::Instance().SetTool(std::make_unique<PanTool>());
                GizmoManager::Instance().SetGizmoType(GizmoType::None);
                break;
            default:
                break;
        }
    }
    
    void ToggleGrid() { GridManager::Instance().ToggleGrid(); }
    void ToggleSnap() { GridManager::Instance().ToggleSnap(); }
    void Undo() { CommandStack::Instance().Undo(); }
    void Redo() { CommandStack::Instance().Redo(); }
    void DeleteSelected() {
        auto& world = World::Instance();
        auto entities = world.GetEntities();
        for (auto& entity : entities) {
            if (entity.HasComponent<SelectionComponent>()) {
                auto& selection = entity.GetComponent<SelectionComponent>();
                if (selection.isSelected) {
                    auto cmd = std::make_unique<DeleteEntityCommand>(entity.GetId());
                    CommandStack::Instance().Execute(std::move(cmd));
                    break;
                }
            }
        }
    }

private:
    Application() = default;
    ~Application() = default;
    Application(const Application&) = delete;
    Application& operator=(const Application&) = delete;
    
    void CreateTestEntities() {
        auto& world = World::Instance();
        
        Entity rect1 = world.CreateEntity("Rectangle 1");
        auto& transform1 = rect1.AddComponent<TransformComponent>();
        transform1.position = glm::vec3(-2.0f, 0.0f, 0);
        rect1.AddComponent<RenderComponent>().isVisible = true;
        
        Entity rect2 = world.CreateEntity("Rectangle 2");
        auto& transform2 = rect2.AddComponent<TransformComponent>();
        transform2.position = glm::vec3(2.0f, 0.0f, 0);
        rect2.AddComponent<RenderComponent>().isVisible = true;
        
        Entity circle = world.CreateEntity("Circle");
        auto& transform3 = circle.AddComponent<TransformComponent>();
        transform3.position = glm::vec3(0.0f, 2.0f, 0);
        circle.AddComponent<RenderComponent>().isVisible = true;
    }
    
    Camera m_camera;
};
