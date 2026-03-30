#pragma once
#include "Shape.h"
#include <vector>
#include <memory>

class Shader;
class VertexBuffer;
class IndexBuffer;

/**
 * @class Heart
 * @brief 蹇冨舰瀹炰綋绫?
 * @details 琛ㄧず涓€涓彲娓叉煋鐨勫績褰㈠璞★紝缁ф壙鑷猄hape
 * @coreResponsibility 璐熻矗蹇冨舰鐨勫垵濮嬪寲鍜屾覆鏌?
 * @designIntent 閫氳繃椤剁偣缂撳啿鍖哄拰绱㈠紩缂撳啿鍖哄疄鐜板績褰㈢殑娓叉煋
 * @useCase 鐢ㄤ簬鍦∣penGL鍦烘櫙涓覆鏌撳績褰?
 * @attention 浣跨敤椤剁偣鍜岀储寮曟暟鎹疄鐜板績褰㈢殑骞虫粦娓叉煋
 */
class Heart : public Shape
{
public:
    Heart(const glm::vec2& position = glm::vec2(0.0f, 0.0f),
          const glm::vec4& color = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f),
          float size = 1.0f, int segments = 64);
    
    void Draw() override;

private:
    float m_size;
    int m_segments;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
    std::unique_ptr<IndexBuffer> m_indexBuffer;
    
    // 椤剁偣鍜岀储寮曟暟鎹?
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    
    void InitOpenGL() override;
};

