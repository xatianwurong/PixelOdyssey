#pragma once
#include "../ecs/Entity.h"
#include "../ecs/Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <optional>

/**
 * @brief 射线类
 * @details 表示 3D 空间中的一条射线
 */
class Ray {
public:
    glm::vec3 origin;    // 射线起点
    glm::vec3 direction; // 射线方向（归一化）
    
    Ray() : origin(0), direction(0, 0, -1) {}
    Ray(const glm::vec3& origin, const glm::vec3& direction) 
        : origin(origin), direction(glm::normalize(direction)) {}
    
    /**
     * @brief 获取射线上的点
     * @param t 距离参数
     * @return 射线上的点
     */
    glm::vec3 GetPoint(float t) const {
        return origin + direction * t;
    }
};

/**
 * @brief 射线投射结果
 */
struct RaycastHit {
    Entity entity;
    glm::vec3 point;      // 击中点
    glm::vec3 normal;     // 击中点法线
    float distance;       // 距离
    bool hasHit;          // 是否击中
    
    RaycastHit() : distance(0), hasHit(false) {}
};

/**
 * @brief 射线投射工具类
 * @details 提供射线与几何体的相交检测
 */
class Raycast {
public:
    /**
     * @brief 从屏幕坐标创建射线
     * @param mouseX 鼠标 X 坐标（归一化设备坐标 -1 到 1）
     * @param mouseY 鼠标 Y 坐标（归一化设备坐标 -1 到 1）
     * @param viewMatrix 视图矩阵
     * @param projMatrix 投影矩阵
     * @return 射线对象
     */
    static Ray CreateRayFromScreen(float mouseX, float mouseY,
                                   const glm::mat4& viewMatrix,
                                   const glm::mat4& projMatrix) {
        // 将屏幕坐标转换为 NDC
        float ndcX = mouseX;
        float ndcY = mouseY;
        
        // 将 NDC 转换为裁剪空间
        glm::vec4 clipCoords(ndcX, ndcY, -1.0f, 1.0f);
        
        // 转换为眼空间
        glm::mat4 invProj = glm::inverse(projMatrix);
        glm::vec4 eyeCoords = invProj * clipCoords;
        eyeCoords.z = -1.0f;
        eyeCoords.w = 0.0f;
        
        // 转换为世界空间
        glm::mat4 invView = glm::inverse(viewMatrix);
        glm::vec4 worldCoords = invView * eyeCoords;
        
        glm::vec3 rayStart(0, 0, 0); // 相机位置
        glm::vec3 rayDirection = glm::normalize(glm::vec3(worldCoords));
        
        return Ray(rayStart, rayDirection);
    }
    
    /**
     * @brief 射线与 AABB 相交检测
     * @param ray 射线
     * @param min AABB 最小点
     * @param max AABB 最大点
     * @return 如果相交返回 true
     */
    static bool RaycastAABB(const Ray& ray, const glm::vec3& min, const glm::vec3& max) {
        float tmin = (min.x - ray.origin.x) / ray.direction.x;
        float tmax = (max.x - ray.origin.x) / ray.direction.x;
        
        if (tmin > tmax) std::swap(tmin, tmax);
        
        float tymin = (min.y - ray.origin.y) / ray.direction.y;
        float tymax = (max.y - ray.origin.y) / ray.direction.y;
        
        if (tymin > tymax) std::swap(tymin, tymax);
        
        if ((tmin > tymax) || (tymin > tmax))
            return false;
        
        if (tymin > tmin) tmin = tymin;
        if (tymax < tmax) tmax = tymax;
        
        float tzmin = (min.z - ray.origin.z) / ray.direction.z;
        float tzmax = (max.z - ray.origin.z) / ray.direction.z;
        
        if (tzmin > tzmax) std::swap(tzmin, tzmax);
        
        if ((tmin > tzmax) || (tzmin > tmax))
            return false;
        
        return true;
    }
    
    /**
     * @brief 射线与球体相交检测
     * @param ray 射线
     * @param center 球心
     * @param radius 半径
     * @return 如果相交返回 true
     */
    static bool RaycastSphere(const Ray& ray, const glm::vec3& center, float radius) {
        glm::vec3 L = center - ray.origin;
        float tca = glm::dot(L, ray.direction);
        
        if (tca < 0) return false;
        
        float d2 = glm::dot(L, L) - tca * tca;
        float radius2 = radius * radius;
        
        if (d2 > radius2) return false;
        
        return true;
    }
    
    /**
     * @brief 射线与平面相交检测
     * @param ray 射线
     * @param planePoint 平面上一点
     * @param planeNormal 平面法线
     * @return 交点距离，如果不相交返回 -1
     */
    static float RaycastPlane(const Ray& ray, const glm::vec3& planePoint, 
                              const glm::vec3& planeNormal) {
        float denom = glm::dot(planeNormal, ray.direction);
        
        if (std::abs(denom) < 1e-6f) return -1.0f;
        
        float t = glm::dot(planePoint - ray.origin, planeNormal) / denom;
        
        if (t < 0) return -1.0f;
        
        return t;
    }
    
    /**
     * @brief 射线与 2D 平面（Z=0）相交检测
     * @param ray 射线
     * @return 交点（2D），如果不相交返回空
     */
    static std::optional<glm::vec2> RaycastToGroundPlane(const Ray& ray) {
        glm::vec3 planePoint(0, 0, 0);
        glm::vec3 planeNormal(0, 0, 1);
        
        float t = RaycastPlane(ray, planePoint, planeNormal);
        
        if (t < 0) return std::nullopt;
        
        glm::vec3 hitPoint = ray.GetPoint(t);
        return glm::vec2(hitPoint.x, hitPoint.y);
    }
    
    /**
     * @brief 对场景中的所有实体进行射线投射
     * @param ray 射线
     * @param world 世界对象
     * @return 最近的击中结果
     */
    static RaycastHit RaycastAll(const Ray& ray, World& world) {
        RaycastHit closestHit;
        closestHit.distance = std::numeric_limits<float>::max();
        
        auto entities = world.GetEntities();
        
        for (auto& entity : entities) {
            if (!entity.HasComponent<TransformComponent>() ||
                !entity.HasComponent<RenderComponent>()) {
                continue;
            }
            
            auto& transform = entity.GetComponent<TransformComponent>();
            auto& render = entity.GetComponent<RenderComponent>();
            
            if (!render.isVisible) continue;
            
            // 简单的球体包围盒检测
            glm::vec3 position = transform.GetModelMatrix() * glm::vec4(0, 0, 0, 1);
            float radius = 0.5f; // 简化处理
            
            if (RaycastSphere(ray, position, radius)) {
                float distance = glm::length(position - ray.origin);
                
                if (distance < closestHit.distance) {
                    closestHit.entity = entity;
                    closestHit.point = position;
                    closestHit.normal = glm::normalize(position - ray.origin);
                    closestHit.distance = distance;
                    closestHit.hasHit = true;
                }
            }
        }
        
        return closestHit;
    }
};
