#pragma once
#include <glm/glm.hpp>
#include <vector>

/**
 * @brief 网格类
 * @details 渲染 2D/3D 网格，用于辅助绘图和定位
 */
class Grid {
public:
    Grid() 
        : m_size(100.0f)
        , m_divisions(100)
        , m_majorLineEvery(10)
        , m_colorMajor(0.5f, 0.5f, 0.5f, 1.0f)
        , m_colorMinor(0.7f, 0.7f, 0.7f, 0.5f)
        , m_axisXColor(1.0f, 0.0f, 0.0f, 1.0f)
        , m_axisYColor(0.0f, 1.0f, 0.0f, 1.0f)
        , m_axisZColor(0.0f, 0.0f, 1.0f, 1.0f)
        , m_isVisible(true) {}
    
    /**
     * @brief 设置网格大小
     * @param size 网格半边长
     */
    void SetSize(float size) {
        m_size = size;
        GenerateGrid();
    }
    
    /**
     * @brief 设置网格划分
     * @param divisions 划分数量
     */
    void SetDivisions(int divisions) {
        m_divisions = divisions;
        GenerateGrid();
    }
    
    /**
     * @brief 设置主网格线间隔
     * @param every 每隔多少条线为主网格线
     */
    void SetMajorLineEvery(int every) {
        m_majorLineEvery = every;
    }
    
    /**
     * @brief 设置颜色
     */
    void SetColors(const glm::vec4& major, const glm::vec4& minor) {
        m_colorMajor = major;
        m_colorMinor = minor;
    }
    
    /**
     * @brief 设置可见性
     * @param visible 是否可见
     */
    void SetVisible(bool visible) {
        m_isVisible = visible;
    }
    
    /**
     * @brief 获取可见性
     */
    bool IsVisible() const {
        return m_isVisible;
    }
    
    /**
     * @brief 生成网格顶点
     */
    void GenerateGrid() {
        m_vertices.clear();
        m_colors.clear();
        
        float step = m_size * 2.0f / m_divisions;
        
        for (int i = 0; i <= m_divisions; i++) {
            float pos = -m_size + i * step;
            
            // 垂直线
            bool isMajor = (i % m_majorLineEvery == 0);
            const glm::vec4& color = isMajor ? m_colorMajor : m_colorMinor;
            
            // X 方向线
            m_vertices.push_back(glm::vec3(-m_size, pos, 0));
            m_vertices.push_back(glm::vec3(m_size, pos, 0));
            m_colors.push_back(color);
            m_colors.push_back(color);
            
            // Y 方向线
            m_vertices.push_back(glm::vec3(pos, -m_size, 0));
            m_vertices.push_back(glm::vec3(pos, m_size, 0));
            m_colors.push_back(color);
            m_colors.push_back(color);
        }
        
        // 坐标轴
        // X 轴（红色）
        m_vertices.push_back(glm::vec3(-m_size, 0, 0));
        m_vertices.push_back(glm::vec3(m_size, 0, 0));
        m_colors.push_back(m_axisXColor);
        m_colors.push_back(m_axisXColor);
        
        // Y 轴（绿色）
        m_vertices.push_back(glm::vec3(0, -m_size, 0));
        m_vertices.push_back(glm::vec3(0, m_size, 0));
        m_colors.push_back(m_axisYColor);
        m_colors.push_back(m_axisYColor);
        
        // Z 轴（蓝色）- 3D 模式
        m_vertices.push_back(glm::vec3(0, 0, -m_size));
        m_vertices.push_back(glm::vec3(0, 0, m_size));
        m_colors.push_back(m_axisZColor);
        m_colors.push_back(m_axisZColor);
    }
    
    /**
     * @brief 获取顶点数据
     */
    const std::vector<glm::vec3>& GetVertices() const {
        return m_vertices;
    }
    
    /**
     * @brief 获取颜色数据
     */
    const std::vector<glm::vec4>& GetColors() const {
        return m_colors;
    }
    
    /**
     * @brief 获取顶点数量
     */
    size_t GetVertexCount() const {
        return m_vertices.size();
    }
    
    /**
     * @brief 将点吸附到网格
     * @param point 原始点
     * @return 吸附后的点
     */
    glm::vec3 SnapToGrid(const glm::vec3& point) const {
        if (!m_snapEnabled) return point;
        
        float step = m_size * 2.0f / m_divisions;
        
        return glm::vec3(
            round(point.x / step) * step,
            round(point.y / step) * step,
            round(point.z / step) * step
        );
    }
    
    /**
     * @brief 启用/禁用吸附
     */
    void SetSnapEnabled(bool enabled) {
        m_snapEnabled = enabled;
    }
    
    /**
     * @brief 获取吸附是否启用
     */
    bool IsSnapEnabled() const {
        return m_snapEnabled;
    }

private:
    float m_size;
    int m_divisions;
    int m_majorLineEvery;
    
    glm::vec4 m_colorMajor;
    glm::vec4 m_colorMinor;
    glm::vec4 m_axisXColor;
    glm::vec4 m_axisYColor;
    glm::vec4 m_axisZColor;
    
    bool m_isVisible;
    bool m_snapEnabled = false;
    
    std::vector<glm::vec3> m_vertices;
    std::vector<glm::vec4> m_colors;
};

/**
 * @brief 网格管理器
 */
class GridManager {
public:
    static GridManager& Instance() {
        static GridManager instance;
        return instance;
    }
    
    /**
     * @brief 获取主网格
     */
    Grid& GetMainGrid() {
        return m_mainGrid;
    }
    
    /**
     * @brief 切换网格可见性
     */
    void ToggleGrid() {
        m_mainGrid.SetVisible(!m_mainGrid.IsVisible());
    }
    
    /**
     * @brief 切换吸附
     */
    void ToggleSnap() {
        m_mainGrid.SetSnapEnabled(!m_mainGrid.IsSnapEnabled());
    }
    
    /**
     * @brief 渲染网格
     */
    void Render() {
        if (!m_mainGrid.IsVisible()) return;
        
        // TODO: 实现网格渲染
        // 使用 GL_LINES 绘制
    }

private:
    GridManager() = default;
    Grid m_mainGrid;
};
