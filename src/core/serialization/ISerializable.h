#pragma once

#include <string>

/**
 * @brief 序列化接口
 * @details 所有可序列化对象必须实现此接口
 */
class ISerializable {
public:
    virtual ~ISerializable() = default;
    
    /**
     * @brief 序列化为字符串
     * @return JSON 字符串
     */
    virtual std::string Serialize() const = 0;
    
    /**
     * @brief 从字符串反序列化
     * @param data JSON 字符串
     */
    virtual void Deserialize(const std::string& data) = 0;
};
