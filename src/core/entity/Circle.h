#pragma once
#include "Shape.h"
#include "../renderer/VertexBuffer.h"
#include "../renderer/IndexBuffer.h"
#include "../resources/Shader.h"

/**
 * @class Circle
 * @brief 鍦嗗舰瀹炰綋绫?
 * @details 琛ㄧず涓€涓彲娓叉煋鐨勫渾褰㈠璞★紝缁ф壙鑷猄hape
 * @coreResponsibility 璐熻矗鍦嗗舰鐨勫垵濮嬪寲鍜屾覆鏌?
 * @designIntent 閫氳繃椤剁偣缂撳啿鍖哄拰绱㈠紩缂撳啿鍖哄疄鐜板渾褰㈢殑娓叉煋
 * @useCase 鐢ㄤ簬鍦∣penGL鍦烘櫙涓覆鏌撳渾褰?
 * @attention 浣跨敤椤剁偣鍜岀储寮曟暟鎹疄鐜板渾褰㈢殑骞虫粦娓叉煋
 */
class Circle : public Shape
{
public:
    Circle(const glm::vec2& position = glm::vec2(0.0f, 0.0f),
           const glm::vec4& color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
           float radius = 0.5f, int segments = 32);
    
    void Draw() override;

private:
    float m_radius;
    int m_segments;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
    std::unique_ptr<IndexBuffer> m_indexBuffer;
    
    // 椤剁偣鍜岀储寮曟暟鎹?
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    
    void InitOpenGL() override;
};

