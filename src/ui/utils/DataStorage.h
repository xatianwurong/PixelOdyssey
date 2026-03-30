#pragma once

#include <string>
#include <map>

/**
 * @class CDataStorage
 * @brief 数据存储类
 * @details 管理应用程序的配置数据和状态信息
 * @功能描述 存储和管理相机设置、渲染设置和应用程序状态
 * @设计目的 提供一个集中的数据存储和管理接口
 * @使用场景 用于在应用程序不同组件之间共享和持久化数据
 * @关键实现 使用单例模式，提供配置文件的加载和保存功能
 */
class CDataStorage
{
public:
    static CDataStorage& Instance();

    // Camera settings
    void SetCameraPosition(float x, float y, float z);
    void GetCameraPosition(float& x, float& y, float& z) const;

    // Rendering settings
    void SetWireframe(bool bWireframe);
    bool GetWireframe() const;

    void SetSmoothShading(bool bSmoothShading);
    bool GetSmoothShading() const;

    void SetDepthTest(bool bDepthTest);
    bool GetDepthTest() const;

    // Application state
    void SetIsRunning(bool bIsRunning);
    bool GetIsRunning() const;

    // Configuration file operations
    bool LoadConfig(const std::string& strConfigFile);
    bool SaveConfig(const std::string& strConfigFile) const;

private:
    CDataStorage();
    ~CDataStorage() = default;

    // Camera settings
    float m_fCameraX;
    float m_fCameraY;
    float m_fCameraZ;

    // Rendering settings
    bool m_bWireframe;
    bool m_bSmoothShading;
    bool m_bDepthTest;

    // Application state
    bool m_bIsRunning;

    // Configuration data
    std::map<std::string, std::string> m_configData;
};
