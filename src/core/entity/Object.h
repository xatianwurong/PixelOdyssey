#pragma once
#include <string>

/**
 * @class Object
 * @brief 可渲染对象基类
 * @details 所有可渲染实体的基类，定义了对象的基本接口
 * @功能描述 提供对象的基本结构和唯一标识
 * @设计目的 使用面向对象设计实现不同类型实体的统一管理
 * @使用场景 作为各种可渲染对象（如3D模型、2D形状等）的基类
 * @关键实现 所有派生类必须实现Draw方法
 */
class Object
{
public:
    Object();
    virtual ~Object() = default;
    virtual void Update(float deltaTime) {}
    virtual void Draw() = 0;

    std::string GetId() const { return m_id; }

protected:
    std::string m_id;
};

