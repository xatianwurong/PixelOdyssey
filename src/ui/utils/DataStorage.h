#pragma once

#include <string>
#include <map>

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
