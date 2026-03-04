//
// Created by 谢豪 on 2025/8/20.
//

#ifndef COLLISIONDETECTOR_H
#define COLLISIONDETECTOR_H

#include <glm/glm.hpp>
#include <vector>
#include <memory>

// 3D碰撞体类型枚举
enum class CollisionType {
    SPHERE,
    BOX,
    CAPSULE
};

// 碰撞体基类
struct Collider {
    glm::vec3 position;
    CollisionType type;
    
    Collider(const glm::vec3& pos, CollisionType t) : position(pos), type(t) {}
    virtual ~Collider() = default;
};

// 球体碰撞体
struct SphereCollider : public Collider {
    float radius;
    
    SphereCollider(const glm::vec3& pos, float r) : Collider(pos, CollisionType::SPHERE), radius(r) {}
};

// 轴对齐包围盒（AABB）
struct BoxCollider : public Collider {
    glm::vec3 halfExtents; // 各轴的半长度
    
    BoxCollider(const glm::vec3& pos, const glm::vec3& extents) : Collider(pos, CollisionType::BOX), halfExtents(extents) {}
    
    // 获取边界
    glm::vec3 getMin() const { return position - halfExtents; }
    glm::vec3 getMax() const { return position + halfExtents; }
};

struct CapsuleCollider : public Collider {
    float radius;
    float height; // 圆柱部分高度
    
    CapsuleCollider(const glm::vec3& pos, float r, float h) : Collider(pos, CollisionType::CAPSULE), radius(r), height(h) {}
};

// 3D碰撞检测类
class collisionDetector {
public:
    // 基本碰撞检测函数
    
    // 球体与球体碰撞检测
    static bool checkSphereSphere(const glm::vec3& center1, float radius1, const glm::vec3& center2, float radius2);
    
    // 球体与AABB碰撞检测
    static bool checkSphereAABB(const glm::vec3& sphereCenter, float sphereRadius, const glm::vec3& boxMin, const glm::vec3& boxMax);
    
    // AABB与AABB碰撞检测
    static bool checkAABBAABB(const glm::vec3& box1Min, const glm::vec3& box1Max, const glm::vec3& box2Min, const glm::vec3& box2Max);
    
    // 球体与胶囊体碰撞检测
    static bool checkSphereCapsule(const glm::vec3& sphereCenter, float sphereRadius, const glm::vec3& capsuleCenter, float capsuleRadius, float capsuleHeight);
    
    // AABB与胶囊体碰撞检测
    static bool checkAABBCapsule(const glm::vec3& boxMin, const glm::vec3& boxMax, const glm::vec3& capsuleCenter, float capsuleRadius, float capsuleHeight);
    
    // 胶囊体与胶囊体碰撞检测
    static bool checkCapsuleCapsule(const glm::vec3& center1, float radius1, float height1, const glm::vec3& center2, float radius2, float height2);
    
    // 通用碰撞检测函数（基于碰撞体类型）
    static bool checkCollision(const Collider& collider1, const Collider& collider2);
    
    // 射线与球体碰撞检测
    static bool raySphereIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& sphereCenter, float sphereRadius, float& t);
    
    // 射线与AABB碰撞检测
    static bool rayAABBIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& boxMin, const glm::vec3& boxMax, float& tMin, float& tMax);
    
    // 兼容性函数（保持向后兼容）
    static bool checkCollision(const glm::vec3& bulletPos, float bulletRadius, const glm::vec3& boxPos, const glm::vec2& boxHalfExtents);
    static bool checkCollision(const glm::vec3& bulletPos, float bulletRadius, float boxMinX, float boxMaxX, float boxMinZ, float boxMaxZ);
    
    // 空间分区优化
    static bool checkCollisionWithSpatialPartitioning(const std::vector<std::shared_ptr<Collider>>& colliders, const Collider& testCollider);
    
    // 工具函数
    static float clamp_float(float value, float min, float max);
    static glm::vec3 closestPointOnAABB(const glm::vec3& point, const glm::vec3& boxMin, const glm::vec3& boxMax);
    static float distancePointToAABB(const glm::vec3& point, const glm::vec3& boxMin, const glm::vec3& boxMax);
};




#endif //COLLISIONDETECTOR_H
