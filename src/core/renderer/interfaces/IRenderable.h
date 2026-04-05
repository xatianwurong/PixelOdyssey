#pragma once

#include <glm/glm.hpp>

/**
 * @interface IRenderable
 * @brief 所有可渲染对象的基类接口
 * @details
 *   - 定义渲染对象的生命周期和功能
 *   - 支持更新、渲染、资源加载等
 *   - 允许对象在场景中独立演化
 * 
 * @usage
 *   class MyGameObject : public IRenderable {
 *       void Update(float deltaTime) override { ... }
 *       void Render(IRenderContext* context) override { ... }
 *   };
 */
class IRenderContext;  // Forward declaration

class IRenderable {
public:
    virtual ~IRenderable() = default;
    
    /**
     * @brief 更新逻辑
     * @param deltaTime 自上次更新以来的时间（秒）
     */
    virtual void Update(float deltaTime) = 0;
    
    /**
     * @brief 渲染对象
     * @param context 渲染上下文
     * @note 应该使用context提供的接口进行所有渲染操作
     */
    virtual void Render(IRenderContext* context) = 0;
    
    /**
     * @brief 清理回调
     * @note 对象被销毁时调用，释放资源
     */
    virtual void OnDestroy() {}
    
    /**
     * @brief 获取世界坐标系下的变换矩阵
     * @return 4x4变换矩阵
     */
    virtual glm::mat4 GetWorldMatrix() const = 0;
    
    /**
     * @brief 获取可见性状态
     * @return true if object should be rendered
     */
    virtual bool IsVisible() const = 0;
    
    /**
     * @brief 获取渲染层级
     * @return 层级值（0=最底层，值越大越靠前）
     */
    virtual int GetRenderLayer() const = 0;
    
    /**
     * @brief 加载资源
     * @return true if successful
     * @note 在首次渲染前调用
     */
    virtual bool LoadResources() = 0;
    
    /**
     * @brief 卸载资源
     * @note 对象销毁前调用
     */
    virtual void UnloadResources() = 0;
};
