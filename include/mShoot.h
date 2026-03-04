//
// Created by 谢豪 on 2025/8/20.
//

#ifndef MSHOOT_H
#define MSHOOT_H



#include <learnopengl/shader_m.h>
#include <glm/glm.hpp>
#include <vector>
#include "Object.h"

// 子弹结构体
struct Bullet {
    glm::vec3 position;    // 位置
    glm::vec3 direction;   // 飞行方向
    float speed;           // 飞行速度
    float lifetime;        // 剩余生命周期
    float maxLifetime;     // 最大生命周期（超时销毁）
    glm::vec3 color;       // 子弹颜色
};

class mShoot : public Object {
public:
    mShoot();
    // 发射子弹（参数：发射位置、方向、速度、生命周期、颜色）
    void shoot(glm::vec3 pos, glm::vec3 dir, float speed = 10.0f,
               float lifetime = 3.0f, glm::vec3 color = glm::vec3(1.0f, 0.2f, 0.2f));
    // 更新所有子弹位置（deltaTime：帧间隔时间）
    void update(float deltaTime);
    // 覆盖 draw 方法
    void draw(glm::mat4 view, glm::mat4 projection) override;
    std::vector<Bullet> bullets;
private:
    // 初始化子弹渲染所需的缓冲对象
    void initBuffers();
    // 子弹数据

    // 渲染相关
    Shader* bulletShader;
    unsigned int bulletVAO, bulletVBO;  // 子弹模型（使用简单立方体）

    // 子弹顶点数据（中心在原点的小立方体，仅包含位置信息）
    float bulletVertices[108] = {
        // 正面
        -0.05f, -0.05f,  0.05f,
         0.05f, -0.05f,  0.05f,
         0.05f,  0.05f,  0.05f,
         0.05f,  0.05f,  0.05f,
        -0.05f,  0.05f,  0.05f,
        -0.05f, -0.05f,  0.05f,
        // 背面
        -0.05f, -0.05f, -0.05f,
        -0.05f,  0.05f, -0.05f,
         0.05f,  0.05f, -0.05f,
         0.05f,  0.05f, -0.05f,
         0.05f, -0.05f, -0.05f,
        -0.05f, -0.05f, -0.05f,
        // 左面
        -0.05f,  0.05f,  0.05f,
        -0.05f,  0.05f, -0.05f,
        -0.05f, -0.05f, -0.05f,
        -0.05f, -0.05f, -0.05f,
        -0.05f, -0.05f,  0.05f,
        -0.05f,  0.05f,  0.05f,
        // 右面
         0.05f,  0.05f,  0.05f,
         0.05f, -0.05f,  0.05f,
         0.05f, -0.05f, -0.05f,
         0.05f, -0.05f, -0.05f,
         0.05f,  0.05f, -0.05f,
         0.05f,  0.05f,  0.05f,
        // 顶面
        -0.05f,  0.05f,  0.05f,
         0.05f,  0.05f,  0.05f,
         0.05f,  0.05f, -0.05f,
         0.05f,  0.05f, -0.05f,
        -0.05f,  0.05f, -0.05f,
        -0.05f,  0.05f,  0.05f,
        // 底面
        -0.05f, -0.05f,  0.05f,
        -0.05f, -0.05f, -0.05f,
         0.05f, -0.05f, -0.05f,
         0.05f, -0.05f, -0.05f,
         0.05f, -0.05f,  0.05f,
        -0.05f, -0.05f,  0.05f
    };
};




#endif //MSHOOT_H
