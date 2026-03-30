#pragma once
#include "Shape.h"
#include <vector>
#include <memory>

class Shader;
class VertexBuffer;
class IndexBuffer;

/**
 * @class Pentagram
 * @brief 浜旇鏄熷疄浣撶被
 * @details 琛ㄧず涓€涓彲娓叉煋鐨勪簲瑙掓槦瀵硅薄锛岀户鎵胯嚜Shape
 * @coreResponsibility 璐熻矗浜旇鏄熺殑鍒濆鍖栧拰娓叉煋
 * @designIntent 閫氳繃椤剁偣缂撳啿鍖哄拰绱㈠紩缂撳啿鍖哄疄鐜颁簲瑙掓槦鐨勬覆鏌?
 * @useCase 鐢ㄤ簬鍦∣penGL鍦烘櫙涓覆鏌撲簲瑙掓槦
 * @attention 浣跨敤椤剁偣鍜岀储寮曟暟鎹疄鐜颁簲瑙掓槦鐨勬覆鏌?
 */
class Pentagram : public Shape
{
public:
    Pentagram(const glm::vec2& position = glm::vec2(0.0f, 0.0f),
              const glm::vec4& color = glm::vec4(1.0f, 0.5f, 0.0f, 1.0f),
              float size = 1.0f);
    
    void Draw() override;

private:
    float m_size;
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<VertexBuffer> m_vertexBuffer;
    std::unique_ptr<IndexBuffer> m_indexBuffer;
    
    // 椤剁偣鍜岀储寮曟暟鎹?
    std::vector<float> m_vertices;
    std::vector<unsigned int> m_indices;
    
    void InitOpenGL() override;
};

