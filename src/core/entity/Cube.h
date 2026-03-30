#pragma once
#include "Object.h"
#include "../renderer/VertexBuffer.h"
#include "../renderer/IndexBuffer.h"
#include "../renderer/Camera.h"
#include "../resources/Shader.h"
#include <memory>
#include <vector>
#include <glm/glm.hpp>

/**
 * @class Cube
 * @brief 绔嬫柟浣撳疄浣撶被
 * @details 琛ㄧず涓€涓彲娓叉煋鐨勭珛鏂逛綋瀵硅薄锛岀户鎵胯嚜Object
 * @coreResponsibility 璐熻矗绔嬫柟浣撶殑鍒濆鍖栥€佹覆鏌撳拰璧勬簮绠＄悊
 * @designIntent 閫氳繃缁勫悎妯″紡闆嗘垚鐫€鑹插櫒鍜岄《鐐圭紦鍐插尯锛屽疄鐜扮珛鏂逛綋鐨勬覆鏌?
 * @useCase 鐢ㄤ簬鍦∣penGL鍦烘櫙涓覆鏌撶珛鏂逛綋
 * @attention 鍐呴儴绠＄悊鐫€鑹插櫒鍜岄《鐐圭紦鍐插尯璧勬簮锛岀‘淇濇纭噴鏀?
 */
class Cube : public Object
{
public:
    Cube();
    ~Cube();
    
    void Draw() override;

private:
    std::unique_ptr<Shader> m_shader;
    std::unique_ptr<VertexBuffer> m_positionBuffer;
    std::unique_ptr<VertexBuffer> m_colorBuffer;
    std::unique_ptr<IndexBuffer> m_indexBuffer;
    std::unique_ptr<Camera> m_camera;
    unsigned int m_vertexArrayObject; // Vertex Array Object

    // Vertex data (stored for deferred initialization)
    std::vector<float> m_vertexPositionBufferData;
    std::vector<unsigned int> m_indexBufferData;
    std::vector<float> m_colorBufferData;
    bool m_bInitialized;

    // Initialize OpenGL resources
    void InitOpenGL();
};

