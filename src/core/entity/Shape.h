#pragma once
#include "Object.h"
#include <glm/glm.hpp>
#include <vector>
#include <memory>

#include "./../renderer/VertexBuffer.h"
#include "./../renderer/IndexBuffer.h"
#include "./../renderer/RenderManager.h"
#include "./../renderer/Camera.h"
#include "./../resources/Shader.h"
#include "./../resources/Texture.h"

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <corecrt_math_defines.h>

/**
 * @class Shape
 * @brief 几何形状基类
 * @details 所有几何形状的基类，为形状渲染提供通用属性和方法
 * @功能描述 管理形状的位置、颜色、缩放和OpenGL初始化
 * @设计目的 为所有几何形状提供统一接口，同时允许特定实现
 * @使用场景 作为各种几何形状（如矩形、圆形、三角形等）的基类
 * @关键实现 所有派生类必须实现Draw和InitOpenGL方法
 */
class Shape : public Object
{
public:
  Shape(const glm::vec2& position = glm::vec2(0.0f, 0.0f),
    const glm::vec4& color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
    float scale = 1.0f);
  virtual ~Shape();

  virtual void Draw() override = 0;

  void SetPosition(const glm::vec2& position);
  void SetColor(const glm::vec4& color);
  void SetScale(float scale);

  glm::vec2 GetPosition() const { return m_position; }
  glm::vec4 GetColor() const { return m_color; }
  float GetScale() const { return m_scale; }

protected:
  glm::vec2 m_position;
  glm::vec4 m_color;
  float m_scale;
  unsigned int m_vertexArrayObject;
  bool m_bInitialized; 

  virtual void InitOpenGL() = 0;

  void CreateVAO();
};