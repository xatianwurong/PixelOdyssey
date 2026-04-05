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
 * @brief 应用主类
 * @details PixelOdyssey 应用核心类，整合所有系统模块
 * @details 负责初始化、更新、渲染等核心功能
 * @details 使用单例模式，通过 Instance() 方法获取全局唯一实例
 * @功能描述 管理应用生命周期，协调各系统协同工作
 * @设计目的 提供统一的应用层接口，简化系统间交互
 * @使用场景 应用程序启动后自动创建，贯穿整个应用生命周期
 * @关键实现 单例模式，禁止拷贝和赋值
 */
class Application
{
public:
  /**
   * @brief 获取应用单例实例
   * @return 应用实例引用
   */
  static Application& Instance()
  {
    static Application instance;
    return instance;
  }

  /**
   * @brief 初始化应用
   * @details 设置相机位置、初始化默认工具、配置网格、创建测试实体
   * @return 初始化成功返回 true
   */
  bool Initialize()
  {
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

  /**
   * @brief 关闭应用
   * @details 清理世界、命令栈和事件总线
   */
  void Shutdown()
  {
    World::Instance().Clear();
    CommandStack::Instance().Clear();
    EventBus::Instance().Clear();
  }

  /**
   * @brief 更新应用逻辑
   * @param deltaTime 帧间隔时间（秒）
   */
  void Update(float deltaTime)
  {
    ToolManager::Instance().Update(deltaTime);
    GizmoManager::Instance().Update(&m_camera, 0, 0);
  }

  /**
   * @brief 渲染应用
   * @details 渲染网格、Gizmo 和工具 UI
   */
  void Render()
  {
    GridManager::Instance().Render();
    GizmoManager::Instance().Render();
    ToolManager::Instance().Render();
  }

  /**
   * @brief 获取相机对象
   * @return 相机引用
   */
  Camera& GetCamera() { return m_camera; }

  /**
   * @brief 设置当前工具
   * @param toolType 工具类型枚举
   */
  void SetTool(ToolType toolType)
  {
    switch (toolType)
    {
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

  /**
   * @brief 切换网格显示
   */
  void ToggleGrid() { GridManager::Instance().ToggleGrid(); }

  /**
   * @brief 切换网格吸附
   */
  void ToggleSnap() { GridManager::Instance().ToggleSnap(); }

  /**
   * @brief 撤销操作
   */
  void Undo() { CommandStack::Instance().Undo(); }

  /**
   * @brief 重做操作
   */
  void Redo() { CommandStack::Instance().Redo(); }

  /**
   * @brief 删除选中的实体
   * @details 查找所有实体，删除第一个被选中的实体
   */
  void DeleteSelected()
  {
    auto& world = World::Instance();
    auto entities = world.GetEntities();
    for (auto& entity : entities)
    {
      if (entity.HasComponent<SelectionComponent>())
      {
        auto& selection = entity.GetComponent<SelectionComponent>();
        if (selection.isSelected)
        {
          auto cmd = std::make_unique<DeleteEntityCommand>(entity.GetId());
          CommandStack::Instance().Execute(std::move(cmd));
          break;
        }
      }
    }
  }

private:
  /**
   * @brief 私有构造函数
   */
  Application() = default;

  /**
   * @brief 私有析构函数
   */
  ~Application() = default;

  /**
   * @brief 禁用拷贝构造函数
   */
  Application(const Application&) = delete;

  /**
   * @brief 禁用赋值运算符
   */
  Application& operator=(const Application&) = delete;

  /**
   * @brief 创建测试实体
   * @details 创建三个测试实体：两个矩形和一个圆形
   */
  void CreateTestEntities()
  {
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

  Camera m_camera;  ///< 相机对象
};
