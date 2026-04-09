#pragma once

#include <glm/glm.hpp>

class IRenderContext;  ///< 渲染上下文前向声明

/**
 * @brief 可渲染对象基类接口
 * @details 定义渲染对象的生命周期和功能
 * @details 支持更新、渲染、资源加载等
 * @details 允许对象在场景中独立演化
 * @功能描述 提供所有可渲染对象的统一接口
 * @设计目的 抽象化渲染对象的通用行为
 * @使用场景 游戏对象、UI 元素等所有可渲染实体
 * @关键实现 纯虚接口，由具体渲染对象实现
 */
class IRenderable
{
public:
  /**
   * @brief 虚析构函数
   */
  virtual ~IRenderable() = default;

  /**
   * @brief 更新逻辑
   * @param deltaTime 自上次更新以来的时间（秒）
   */
  virtual void Update(float deltaTime) = 0;

  /**
   * @brief 渲染对象
   * @param context 渲染上下文
   * @details 应该使用 context 提供的接口进行所有渲染操作
   */
  virtual void Render(IRenderContext* context) = 0;

  /**
   * @brief 清理回调
   * @details 对象被销毁时调用，释放资源
   */
  virtual void OnDestroy() {}

  /**
   * @brief 获取世界坐标系下的变换矩阵
   * @return 4x4 变换矩阵
   */
  virtual glm::mat4 GetWorldMatrix() const = 0;

  /**
   * @brief 获取可见性状态
   * @return true 表示对象应该被渲染
   */
  virtual bool IsVisible() const = 0;

  /**
   * @brief 获取渲染层级
   * @return 层级值（0=最底层，值越大越靠前）
   */
  virtual int GetRenderLayer() const = 0;

  /**
   * @brief 加载资源
   * @return true 表示成功
   * @details 在首次渲染前调用
   */
  virtual bool LoadResources() = 0;

  /**
   * @brief 卸载资源
   * @details 对象销毁前调用
   */
  virtual void UnloadResources() = 0;
};
