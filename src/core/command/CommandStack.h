#pragma once
#include "Command.h"
#include <vector>
#include <memory>
#include <functional>

/**
 * @brief 命令栈类
 * @details 管理命令的执行、撤销和重做
 * @details 使用单例模式，通过 Instance() 方法获取全局唯一实例
 * @details 支持命令合并（连续相同操作可合并为一个命令）
 * @功能描述 管理命令历史，提供撤销/重做功能
 * @设计目的 实现命令模式的历史管理
 * @使用场景 编辑器操作历史管理
 * @关键实现 使用两个栈分别管理撤销和重做命令
 */
class CommandStack
{
public:
  /**
   * @brief 获取命令栈单例实例
   * @return 命令栈实例引用
   */
  static CommandStack& Instance()
  {
    static CommandStack instance;
    return instance;
  }

  /**
   * @brief 执行命令并压入栈
   * @param command 要执行的命令唯一指针
   * @details 检查是否可以与上一个命令合并，若可合并则合并
   */
  void Execute(std::unique_ptr<Command> command)
  {
    command->Execute();

    // 检查是否可以与上一个命令合并
    if (!m_undoStack.empty() && m_undoStack.back()->CanMerge(command.get()))
    {
      m_undoStack.back()->Merge(command.get());
    }
    else
    {
      m_undoStack.push_back(std::move(command));
    }

    // 清空重做栈
    m_redoStack.clear();

    // 通知回调
    if (m_onCommandExecuted)
    {
      m_onCommandExecuted();
    }
  }

  /**
   * @brief 撤销上一个命令
   * @details 将命令从撤销栈移到重做栈
   */
  void Undo()
  {
    if (m_undoStack.empty())
    {
      return;
    }

    auto& command = m_undoStack.back();
    command->Undo();

    m_redoStack.push_back(std::move(command));
    m_undoStack.pop_back();

    if (m_onCommandUndone)
    {
      m_onCommandUndone();
    }
  }

  /**
   * @brief 重做上一个撤销的命令
   * @details 将命令从重做栈移回撤销栈
   */
  void Redo()
  {
    if (m_redoStack.empty())
    {
      return;
    }

    auto& command = m_redoStack.back();
    command->Redo();

    m_undoStack.push_back(std::move(command));
    m_redoStack.pop_back();

    if (m_onCommandRedone)
    {
      m_onCommandRedone();
    }
  }

  /**
   * @brief 清空命令栈
   * @details 清空撤销栈和重做栈
   */
  void Clear()
  {
    m_undoStack.clear();
    m_redoStack.clear();
  }

  /**
   * @brief 检查是否可以撤销
   * @return 如果撤销栈非空返回 true
   */
  bool CanUndo() const
  {
    return !m_undoStack.empty();
  }

  /**
   * @brief 检查是否可以重做
   * @return 如果重做栈非空返回 true
   */
  bool CanRedo() const
  {
    return !m_redoStack.empty();
  }

  /**
   * @brief 获取撤销栈大小
   * @return 撤销栈中的命令数量
   */
  size_t GetUndoStackSize() const
  {
    return m_undoStack.size();
  }

  /**
   * @brief 获取重做栈大小
   * @return 重做栈中的命令数量
   */
  size_t GetRedoStackSize() const
  {
    return m_redoStack.size();
  }

  /**
   * @brief 设置命令执行回调
   * @param callback 回调函数
   */
  void SetOnCommandExecuted(std::function<void()> callback)
  {
    m_onCommandExecuted = callback;
  }

  /**
   * @brief 设置命令撤销回调
   * @param callback 回调函数
   */
  void SetOnCommandUndone(std::function<void()> callback)
  {
    m_onCommandUndone = callback;
  }

  /**
   * @brief 设置命令重做回调
   * @param callback 回调函数
   */
  void SetOnCommandRedone(std::function<void()> callback)
  {
    m_onCommandRedone = callback;
  }

private:
  /**
   * @brief 私有构造函数
   */
  CommandStack() = default;

  /**
   * @brief 私有析构函数
   */
  ~CommandStack() = default;

  /**
   * @brief 禁用拷贝构造函数
   */
  CommandStack(const CommandStack&) = delete;

  /**
   * @brief 禁用赋值运算符
   */
  CommandStack& operator=(const CommandStack&) = delete;

  std::vector<std::unique_ptr<Command>> m_undoStack;   ///< 撤销栈
  std::vector<std::unique_ptr<Command>> m_redoStack;   ///< 重做栈

  std::function<void()> m_onCommandExecuted;  ///< 命令执行回调
  std::function<void()> m_onCommandUndone;    ///< 命令撤销回调
  std::function<void()> m_onCommandRedone;    ///< 命令重做回调
};
    CommandStack(const CommandStack&) = delete;
    CommandStack& operator=(const CommandStack&) = delete;
    
    std::vector<std::unique_ptr<Command>> m_undoStack;
    std::vector<std::unique_ptr<Command>> m_redoStack;
    
    std::function<void()> m_onCommandExecuted;
    std::function<void()> m_onCommandUndone;
    std::function<void()> m_onCommandRedone;
};
