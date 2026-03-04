//
// Created by 谢豪 on 2025/8/20.
//

#include "collisionDetector.h"
#include <algorithm>
#include <memory>
#include <limits>
#include <set>
#include <tuple>

// 工具函数
float collisionDetector::clamp_float(float value, float min, float max) {
    return std::max(min, std::min(max, value));
}

glm::vec3 collisionDetector::closestPointOnAABB(const glm::vec3& point, const glm::vec3& boxMin, const glm::vec3& boxMax) {
    return glm::vec3(
        clamp_float(point.x, boxMin.x, boxMax.x),
        clamp_float(point.y, boxMin.y, boxMax.y),
        clamp_float(point.z, boxMin.z, boxMax.z)
    );
}

float collisionDetector::distancePointToAABB(const glm::vec3& point, const glm::vec3& boxMin, const glm::vec3& boxMax) {
    glm::vec3 closest = closestPointOnAABB(point, boxMin, boxMax);
    return glm::length(point - closest);
}

// 球体与球体碰撞检测
bool collisionDetector::checkSphereSphere(const glm::vec3& center1, float radius1, const glm::vec3& center2, float radius2) {
    glm::vec3 diff = center1 - center2;
    float distanceSquared = glm::dot(diff, diff);
    float radiusSum = radius1 + radius2;
    return distanceSquared <= radiusSum * radiusSum;
}

// 球体与AABB碰撞检测
bool collisionDetector::checkSphereAABB(const glm::vec3& sphereCenter, float sphereRadius, const glm::vec3& boxMin, const glm::vec3& boxMax) {
    glm::vec3 closestPoint = closestPointOnAABB(sphereCenter, boxMin, boxMax);
    glm::vec3 diff = sphereCenter - closestPoint;
    float distanceSquared = glm::dot(diff, diff);
    return distanceSquared <= sphereRadius * sphereRadius;
}

// AABB与AABB碰撞检测
bool collisionDetector::checkAABBAABB(const glm::vec3& box1Min, const glm::vec3& box1Max, const glm::vec3& box2Min, const glm::vec3& box2Max) {
    return (box1Min.x <= box2Max.x && box1Max.x >= box2Min.x) &&
           (box1Min.y <= box2Max.y && box1Max.y >= box2Min.y) &&
           (box1Min.z <= box2Max.z && box1Max.z >= box2Min.z);
}

// 球体与胶囊体碰撞检测
bool collisionDetector::checkSphereCapsule(const glm::vec3& sphereCenter, float sphereRadius, const glm::vec3& capsuleCenter, float capsuleRadius, float capsuleHeight) {
    // 将胶囊体分解为圆柱体和两个半球
    float halfHeight = capsuleHeight * 0.5f;
    
    // 检查与圆柱体的碰撞
    glm::vec3 capsuleTop = capsuleCenter + glm::vec3(0, halfHeight, 0);
    glm::vec3 capsuleBottom = capsuleCenter - glm::vec3(0, halfHeight, 0);
    
    // 计算球心到胶囊体轴线的最近点
    glm::vec3 axis = capsuleTop - capsuleBottom;
    float axisLength = glm::length(axis);
    if (axisLength > 0.0f) {
        glm::vec3 axisNormalized = axis / axisLength;
        glm::vec3 pointToBottom = sphereCenter - capsuleBottom;
        float projection = glm::dot(pointToBottom, axisNormalized);
        projection = clamp_float(projection, 0.0f, axisLength);
        
        glm::vec3 closestPoint = capsuleBottom + axisNormalized * projection;
        float distance = glm::length(sphereCenter - closestPoint);
        return distance <= sphereRadius + capsuleRadius;
    }
    
    // 如果胶囊体高度为0，退化为球体
    return checkSphereSphere(sphereCenter, sphereRadius, capsuleCenter, capsuleRadius);
}

// AABB与胶囊体碰撞检测
bool collisionDetector::checkAABBCapsule(const glm::vec3& boxMin, const glm::vec3& boxMax, const glm::vec3& capsuleCenter, float capsuleRadius, float capsuleHeight) {
    // 简化实现：将胶囊体近似为多个球体进行检测
    float halfHeight = capsuleHeight * 0.5f;
    int segments = 8;
    
    for (int i = 0; i <= segments; ++i) {
        float t = static_cast<float>(i) / segments;
        glm::vec3 point = capsuleCenter + glm::vec3(0, -halfHeight + t * capsuleHeight, 0);
        
        glm::vec3 closestPoint = closestPointOnAABB(point, boxMin, boxMax);
        float distance = glm::length(point - closestPoint);
        
        if (distance <= capsuleRadius) {
            return true;
        }
    }
    
    return false;
}

// 胶囊体与胶囊体碰撞检测
bool collisionDetector::checkCapsuleCapsule(const glm::vec3& center1, float radius1, float height1, const glm::vec3& center2, float radius2, float height2) {
    float halfHeight1 = height1 * 0.5f;
    float halfHeight2 = height2 * 0.5f;
    
    glm::vec3 top1 = center1 + glm::vec3(0, halfHeight1, 0);
    glm::vec3 bottom1 = center1 - glm::vec3(0, halfHeight1, 0);
    glm::vec3 top2 = center2 + glm::vec3(0, halfHeight2, 0);
    glm::vec3 bottom2 = center2 - glm::vec3(0, halfHeight2, 0);
    
    // 简化实现：检查端点球体之间的距离
    float radiusSum = radius1 + radius2;
    
    // 检查所有端点组合
    if (checkSphereSphere(top1, radius1, top2, radius2) ||
        checkSphereSphere(top1, radius1, bottom2, radius2) ||
        checkSphereSphere(bottom1, radius1, top2, radius2) ||
        checkSphereSphere(bottom1, radius1, bottom2, radius2)) {
        return true;
    }
    
    // 检查轴线间的最小距离
    glm::vec3 axis1 = top1 - bottom1;
    glm::vec3 axis2 = top2 - bottom2;
    glm::vec3 axis1Normalized = glm::normalize(axis1);
    glm::vec3 axis2Normalized = glm::normalize(axis2);
    
    // 使用简化的线段间距离计算
    glm::vec3 start1 = bottom1;
    glm::vec3 end1 = top1;
    glm::vec3 start2 = bottom2;
    glm::vec3 end2 = top2;
    
    // 这里可以使用更精确的线段间距离算法，但为了简化，使用采样方法
    int samples = 8;
    for (int i = 0; i <= samples; ++i) {
        float t1 = static_cast<float>(i) / samples;
        glm::vec3 point1 = start1 + (end1 - start1) * t1;
        
        for (int j = 0; j <= samples; ++j) {
            float t2 = static_cast<float>(j) / samples;
            glm::vec3 point2 = start2 + (end2 - start2) * t2;
            
            if (glm::length(point1 - point2) <= radiusSum) {
                return true;
            }
        }
    }
    
    return false;
}

// 通用碰撞检测函数
bool collisionDetector::checkCollision(const Collider& collider1, const Collider& collider2) {
    if (collider1.type == CollisionType::SPHERE && collider2.type == CollisionType::SPHERE) {
        const auto& sphere1 = static_cast<const SphereCollider&>(collider1);
        const auto& sphere2 = static_cast<const SphereCollider&>(collider2);
        return checkSphereSphere(sphere1.position, sphere1.radius, sphere2.position, sphere2.radius);
    }
    else if (collider1.type == CollisionType::SPHERE && collider2.type == CollisionType::BOX) {
        const auto& sphere = static_cast<const SphereCollider&>(collider1);
        const auto& box = static_cast<const BoxCollider&>(collider2);
        return checkSphereAABB(sphere.position, sphere.radius, box.getMin(), box.getMax());
    }
    else if (collider1.type == CollisionType::BOX && collider2.type == CollisionType::SPHERE) {
        const auto& box = static_cast<const BoxCollider&>(collider1);
        const auto& sphere = static_cast<const SphereCollider&>(collider2);
        return checkSphereAABB(sphere.position, sphere.radius, box.getMin(), box.getMax());
    }
    else if (collider1.type == CollisionType::BOX && collider2.type == CollisionType::BOX) {
        const auto& box1 = static_cast<const BoxCollider&>(collider1);
        const auto& box2 = static_cast<const BoxCollider&>(collider2);
        return checkAABBAABB(box1.getMin(), box1.getMax(), box2.getMin(), box2.getMax());
    }
    else if (collider1.type == CollisionType::SPHERE && collider2.type == CollisionType::CAPSULE) {
        const auto& sphere = static_cast<const SphereCollider&>(collider1);
        const auto& capsule = static_cast<const CapsuleCollider&>(collider2);
        return checkSphereCapsule(sphere.position, sphere.radius, capsule.position, capsule.radius, capsule.height);
    }
    else if (collider1.type == CollisionType::CAPSULE && collider2.type == CollisionType::SPHERE) {
        const auto& capsule = static_cast<const CapsuleCollider&>(collider1);
        const auto& sphere = static_cast<const SphereCollider&>(collider2);
        return checkSphereCapsule(sphere.position, sphere.radius, capsule.position, capsule.radius, capsule.height);
    }
    
    return false;
}

// 射线与球体碰撞检测
bool collisionDetector::raySphereIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& sphereCenter, float sphereRadius, float& t) {
    glm::vec3 oc = rayOrigin - sphereCenter;
    float a = glm::dot(rayDir, rayDir);
    float b = 2.0f * glm::dot(oc, rayDir);
    float c = glm::dot(oc, oc) - sphereRadius * sphereRadius;
    
    float discriminant = b * b - 4 * a * c;
    if (discriminant < 0.0f) {
        return false;
    }
    
    float sqrtDiscriminant = sqrt(discriminant);
    t = (-b - sqrtDiscriminant) / (2.0f * a);
    
    if (t < 0.0f) {
        t = (-b + sqrtDiscriminant) / (2.0f * a);
        if (t < 0.0f) {
            return false;
        }
    }
    
    return true;
}

// 射线与AABB碰撞检测
bool collisionDetector::rayAABBIntersection(const glm::vec3& rayOrigin, const glm::vec3& rayDir, const glm::vec3& boxMin, const glm::vec3& boxMax, float& tMin, float& tMax) {
    glm::vec3 invDir = 1.0f / rayDir;
    glm::vec3 t1 = (boxMin - rayOrigin) * invDir;
    glm::vec3 t2 = (boxMax - rayOrigin) * invDir;
    
    glm::vec3 tMin3 = glm::min(t1, t2);
    glm::vec3 tMax3 = glm::max(t1, t2);
    
    tMin = glm::max(glm::max(tMin3.x, tMin3.y), tMin3.z);
    tMax = glm::min(glm::min(tMax3.x, tMax3.y), tMax3.z);
    
    return tMax >= tMin && tMax >= 0.0f;
}

// 空间分区优化（简化版本）
bool collisionDetector::checkCollisionWithSpatialPartitioning(const std::vector<std::shared_ptr<Collider>>& colliders, const Collider& testCollider) {
    // 简化实现：遍历所有碰撞体
    // 在实际项目中可以实现八叉树、BVH等空间分区数据结构
    for (const auto& collider : colliders) {
        if (checkCollision(testCollider, *collider)) {
            return true;
        }
    }
    return false;
}

// 兼容性函数（保持向后兼容）
bool collisionDetector::checkCollision(const glm::vec3& bulletPos, float bulletRadius, const glm::vec3& boxPos, const glm::vec2& boxHalfExtents) {
    // 转换为3D检测，假设Y轴高度为1单位
    glm::vec3 boxMin = boxPos - glm::vec3(boxHalfExtents.x, 0.5f, boxHalfExtents.y);
    glm::vec3 boxMax = boxPos + glm::vec3(boxHalfExtents.x, 0.5f, boxHalfExtents.y);
    return checkSphereAABB(bulletPos, bulletRadius, boxMin, boxMax);
}

bool collisionDetector::checkCollision(const glm::vec3& bulletPos, float bulletRadius, float boxMinX, float boxMaxX, float boxMinZ, float boxMaxZ) {
    // 转换为3D检测，假设Y轴范围为[-0.5, 0.5]
    glm::vec3 boxMin(boxMinX, -0.5f, boxMinZ);
    glm::vec3 boxMax(boxMaxX, 0.5f, boxMaxZ);
    return checkSphereAABB(bulletPos, bulletRadius, boxMin, boxMax);
}