#pragma once
#include <string>
#include <memory>

/**
 * @brief 命令基类
 * @details 实现命令模式，支持撤销/重做功能
 * @details 所有具体命令必须继承此类
 */
class Command {
public:
    virtual ~Command() = default;
    
    /**
     * @brief 执行命令
     */
    virtual void Execute() = 0;
    
    /**
     * @brief 撤销命令
     */
    virtual void Undo() = 0;
    
    /**
     * @brief 重做命令
     * @details 默认实现与 Execute 相同
     */
    virtual void Redo() {
        Execute();
    }
    
    /**
     * @brief 获取命令名称
     * @return 命令名称
     */
    virtual std::string GetName() const = 0;
    
    /**
     * @brief 检查命令是否可合并
     * @param other 另一个命令
     * @return 如果可以合并返回 true
     */
    virtual bool CanMerge(const Command* other) const {
        return false;
    }
    
    /**
     * @brief 合并命令
     * @param other 另一个命令
     * @return 是否合并成功
     */
    virtual bool Merge(Command* other) {
        return false;
    }
};

/**
 * @brief 复合命令
 * @details 可以包含多个子命令，作为一个整体执行
 */
class CompositeCommand : public Command {
public:
    /**
     * @brief 添加子命令
     * @param cmd 子命令
     */
    void AddCommand(std::unique_ptr<Command> cmd) {
        m_commands.push_back(std::move(cmd));
    }
    
    /**
     * @brief 执行所有子命令
     */
    void Execute() override {
        for (auto& cmd : m_commands) {
            cmd->Execute();
        }
    }
    
    /**
     * @brief 撤销所有子命令（逆序）
     */
    void Undo() override {
        for (auto it = m_commands.rbegin(); it != m_commands.rend(); ++it) {
            (*it)->Undo();
        }
    }
    
    /**
     * @brief 获取命令名称
     */
    std::string GetName() const override {
        return "CompositeCommand";
    }
    
private:
    std::vector<std::unique_ptr<Command>> m_commands;
};
