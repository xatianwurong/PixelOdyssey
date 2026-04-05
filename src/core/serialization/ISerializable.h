#pragma once

#include <string>

/**
 * @brief 序列化接口
 * @details 所有可序列化对象必须实现此接口
 * @details 支持 JSON 格式的序列化和反序列化
 * @功能描述 提供对象的持久化存储能力
 * @设计目的 统一序列化接口，支持场景保存/加载
 * @使用场景 场景保存、配置导出等
 * @关键实现 纯虚接口，由具体需要序列化的类实现
 */
class ISerializable
{
public:
  /**
   * @brief 虚析构函数
   */
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
