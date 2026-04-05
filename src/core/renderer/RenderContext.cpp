#include "RenderContext.h"
#include "../logging/Logger.h"
#include "interfaces/IBuffer.h"
#include "interfaces/IShader.h"
#include <glm/gtc/matrix_transform.hpp>

using namespace Logging;

RenderContext::RenderContext()
    : m_viewMatrix(1.0f), m_projectionMatrix(1.0f), m_worldMatrix(1.0f) {
    LOG_DEBUG("RenderContext", "RenderContext created");
}

RenderContext::~RenderContext() {
    LOG_DEBUG("RenderContext", "RenderContext destroyed");
}

void RenderContext::SetViewMatrix(const glm::mat4& view) {
    m_viewMatrix = view;
}

void RenderContext::SetProjectionMatrix(const glm::mat4& proj) {
    m_projectionMatrix = proj;
}

void RenderContext::SetWorldMatrix(const glm::mat4& world) {
    m_worldMatrix = world;
}

glm::mat4 RenderContext::GetViewProjectionMatrix() const {
    return m_projectionMatrix * m_viewMatrix;
}

glm::mat4 RenderContext::GetWorldViewProjectionMatrix() const {
    return m_projectionMatrix * m_viewMatrix * m_worldMatrix;
}

void RenderContext::BindVertexBuffer(IBuffer* vb) {
    if (m_currentVB != vb) {
        if (vb) {
            vb->Bind();
            m_currentVB = vb;
        } else {
            LOG_WARNING("RenderContext", "Attempting to bind null vertex buffer");
        }
    }
}

void RenderContext::BindIndexBuffer(IBuffer* ib) {
    if (m_currentIB != ib) {
        if (ib) {
            ib->Bind();
            m_currentIB = ib;
        } else {
            LOG_WARNING("RenderContext", "Attempting to bind null index buffer");
        }
    }
}

void RenderContext::BindShader(IShader* shader) {
    if (m_currentShader != shader) {
        if (shader) {
            shader->Bind();
            m_currentShader = shader;
        } else {
            LOG_WARNING("RenderContext", "Attempting to bind null shader");
        }
    }
}

void RenderContext::DrawIndexed(uint32_t indexCount, uint32_t offset) {
    if (!m_currentVB || !m_currentIB) {
        LOG_WARNING("RenderContext", "DrawIndexed called without binding VB or IB");
        return;
    }
    
    if (indexCount == 0) {
        LOG_WARNING("RenderContext", "DrawIndexed called with zero index count");
        return;
    }
    
    // 这里应该调用OpenGL的绘制函数
    // glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_INT, (void*)(offset * sizeof(GLuint)));
    LOG_DEBUG("RenderContext", "DrawIndexed: count=" + std::to_string(indexCount) + 
              ", offset=" + std::to_string(offset));
}

void RenderContext::DrawArrays(uint32_t vertexCount, uint32_t offset) {
    if (!m_currentVB) {
        LOG_WARNING("RenderContext", "DrawArrays called without binding VB");
        return;
    }
    
    if (vertexCount == 0) {
        LOG_WARNING("RenderContext", "DrawArrays called with zero vertex count");
        return;
    }
    
    // 这里应该调用OpenGL的绘制函数
    // glDrawArrays(GL_TRIANGLES, offset, vertexCount);
    LOG_DEBUG("RenderContext", "DrawArrays: count=" + std::to_string(vertexCount) + 
              ", offset=" + std::to_string(offset));
}

void RenderContext::Clear(float r, float g, float b, float a) {
    m_clearColor = {r, g, b, a};
    // 这里应该调用OpenGL的清除函数
    // glClearColor(r, g, b, a);
    // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    LOG_DEBUG("RenderContext", "Clear: color=(" + std::to_string(r) + "," + 
              std::to_string(g) + "," + std::to_string(b) + "," + std::to_string(a) + ")");
}

void RenderContext::Present() {
    // 这里应该交换缓冲区
    // SwapBuffers(...)
    LOG_DEBUG("RenderContext", "Present");
}
