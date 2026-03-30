#pragma once
#include "../entity/Object.h"
#include <unordered_map>
#include <memory>
#include <string>
#include <vector>

class Scene
{
public:
    Scene(const std::string& name);
    ~Scene() = default;

    void AddObject(Object* object);
    void RemoveObject(const std::string& id);
    Object* GetObject(const std::string& id);
    void Clear();

    const std::string& GetName() const { return m_name; }

    void Update(float deltaTime);
    void Draw();

private:
    std::string m_name;
    std::unordered_map<std::string, std::unique_ptr<Object>> m_objects;
    std::vector<Object*> m_updateOrder;
    std::vector<Object*> m_drawOrder;
};