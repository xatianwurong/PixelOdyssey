#pragma once
#include "../entity/Object.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

/**
 * @brief 场景类
 * @details 管理场景中的对象，包括对象的添加、移除、更新和绘制
 * @details 维护对象的更新顺序和绘制顺序
 * @功能描述 提供场景中对象的管理和操作功能
 * @设计目的 为场景管理提供统一的接口
 * @使用场景 游戏或应用中的不同场景管理
 * @关键实现 使用哈希表存储对象，使用向量维护顺序
 */
class Scene
{
public:
  /**
   * @brief 构造函数
   * @param name 场景名称
   */
  Scene(const std::string& name);

  /**
   * @brief 析构函数
   */
  ~Scene() = default;

  /**
   * @brief 添加对象
   * @param object 对象指针
   */
  void AddObject(Object* object);

  /**
   * @brief 移除对象
   * @param id 对象 ID
   */
  void RemoveObject(const std::string& id);

  /**
   * @brief 获取对象
   * @param id 对象 ID
   * @return 对象指针，如果不存在返回 nullptr
   */
  Object* GetObject(const std::string& id);

  /**
   * @brief 清空场景
   * @details 移除所有对象
   */
  void Clear();

  /**
   * @brief 获取场景名称
   * @return 场景名称常量引用
   */
  const std::string& GetName() const { return m_name; }

  /**
   * @brief 更新场景
   * @param deltaTime 帧间隔时间（秒）
   * @details 按更新顺序更新所有对象
   */
  void Update(float deltaTime);

  /**
   * @brief 绘制场景
   * @details 按绘制顺序绘制所有对象
   */
  void Draw();

private:
  std::string m_name;                                                  ///< 场景名称
  std::unordered_map<std::string, std::unique_ptr<Object>> m_objects;  ///< 对象存储
  std::vector<Object*> m_updateOrder;                                  ///< 更新顺序列表
  std::vector<Object*> m_drawOrder;                                    ///< 绘制顺序列表
};