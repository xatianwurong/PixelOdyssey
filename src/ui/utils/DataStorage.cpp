#include "DataStorage.h"
#include <fstream>
#include <sstream>

CDataStorage& CDataStorage::Instance()
{
    static CDataStorage instance;
    return instance;
}

CDataStorage::CDataStorage()
    : m_fCameraX(0.0f),
    m_fCameraY(0.0f),
    m_fCameraZ(5.0f),
    m_bWireframe(false),
    m_bSmoothShading(true),
    m_bDepthTest(true),
    m_bIsRunning(false)
{
}

void CDataStorage::SetCameraPosition(float x, float y, float z)
{
    m_fCameraX = x;
    m_fCameraY = y;
    m_fCameraZ = z;
}

void CDataStorage::GetCameraPosition(float& x, float& y, float& z) const
{
    x = m_fCameraX;
    y = m_fCameraY;
    z = m_fCameraZ;
}

void CDataStorage::SetWireframe(bool bWireframe)
{
    m_bWireframe = bWireframe;
}

bool CDataStorage::GetWireframe() const
{
    return m_bWireframe;
}

void CDataStorage::SetSmoothShading(bool bSmoothShading)
{
    m_bSmoothShading = bSmoothShading;
}

bool CDataStorage::GetSmoothShading() const
{
    return m_bSmoothShading;
}

void CDataStorage::SetDepthTest(bool bDepthTest)
{
    m_bDepthTest = bDepthTest;
}

bool CDataStorage::GetDepthTest() const
{
    return m_bDepthTest;
}

void CDataStorage::SetIsRunning(bool bIsRunning)
{
    m_bIsRunning = bIsRunning;
}

bool CDataStorage::GetIsRunning() const
{
    return m_bIsRunning;
}

bool CDataStorage::LoadConfig(const std::string& strConfigFile)
{
    std::ifstream file(strConfigFile);
    if (!file.is_open())
    {
        return false;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value))
        {
            m_configData[key] = value;
        }
    }

    file.close();
    return true;
}

bool CDataStorage::SaveConfig(const std::string& strConfigFile) const
{
    std::ofstream file(strConfigFile);
    if (!file.is_open())
    {
        return false;
    }

    for (const auto& pair : m_configData)
    {
        file << pair.first << "=" << pair.second << std::endl;
    }

    file.close();
    return true;
}
