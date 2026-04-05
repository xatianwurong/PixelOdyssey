#pragma once
#include <string>
#include <memory>
#include <vector>

/**
 * @brief 命令基类
 * @details 实现命令模式，支持撤销/重做功能
 * @details 所有具体命令必须继承此类
 * @功能描述 封装操作，支持撤销和重做
 * @设计目的 实现命令模式，解耦请求发送者和接收者
 * @使用场景 编辑器操作、事务处理等需要撤销/重做的场景
 * @关键实现 纯虚函数接口，要求派生类实现具体操作
 */
class Command
{
public:
  /**
   * @brief 虚析构函数
   */
  virtual ~Command() = default;

  /**
   * @brief 执行命令
   * @details 纯虚函数，由派生类实现具体执行逻辑
   */
  virtual void Execute() = 0;

  /**
   * @brief 撤销命令
   * @details 纯虚函数，由派生类实现撤销逻辑
   */
  virtual void Undo() = 0;

  /**
   * @brief 重做命令
   * @details 默认实现与 Execute 相同
   */
  virtual void Redo()
  {
    Execute();
  }

  /**
   * @brief 获取命令名称
   * @return 命令的名称字符串
   */
  virtual std::string GetName() const = 0;

  /**
   * @brief 检查命令是否可合并
   * @param other 另一个命令指针
   * @return 如果可以合并返回 true，否则返回 false
   */
  virtual bool CanMerge(const Command* other) const
  {
    return false;
  }

  /**
   * @brief 合并命令
   * @param other 另一个命令指针
   * @return 是否合并成功
   */
  virtual bool Merge(Command* other)
  {
    return false;
  }
};

/**
 * @brief 复合命令
 * @details 可以包含多个子命令，作为一个整体执行
 * @details 支持命令的组合操作
 * @功能描述 将多个命令组合成一个命令执行
 * @设计目的 支持复杂操作的原子性执行
 * @使用场景 批量操作、事务操作等
 */
class CompositeCommand : public Command
{
public:
  /**
   * @brief 添加子命令
   * @param cmd 子命令的唯一指针
   */
  void AddCommand(std::unique_ptr<Command> cmd)
  {
    m_commands.push_back(std::move(cmd));
  }

  /**
   * @brief 执行所有子命令
   * @details 按添加顺序依次执行所有子命令
   */
  void Execute() override
  {
    for (auto& cmd : m_commands)
    {
      cmd->Execute();
    }
  }

  /**
   * @brief 撤销所有子命令
   * @details 按添加顺序的逆序依次撤销所有子命令
   */
  void Undo() override
  {
    for (auto it = m_commands.rbegin(); it != m_commands.rend(); ++it)
    {
      (*it)->Undo();
    }
  }

  /**
   * @brief 获取命令名称
   * @return 命令名称字符串
   */
  std::string GetName() const override
  {
    return "CompositeCommand";
  }

private:
  std::vector<std::unique_ptr<Command>> m_commands;  ///< 子命令列表
};
