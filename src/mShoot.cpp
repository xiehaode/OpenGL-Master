//
// Created by 谢豪 on 2025/8/20.
//

#include "../include/mShoot.h"
#include <../include/glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <iostream>
#include "ResourceManager.h"

mShoot::mShoot() {
    // 使用简单的纯色着色器
    bulletShader = new Shader(ResourceManager::getShaderPath("bullet.vs").c_str(), ResourceManager::getShaderPath("bullet.fs").c_str());
    initBuffers();

    // 初始化着色器（设置默认颜色，实际会被每个子弹的颜色覆盖）
    bulletShader->use();
}

void mShoot::shoot(glm::vec3 pos, glm::vec3 dir, float speed, float lifetime, glm::vec3 color) {
    Bullet newBullet;
    newBullet.position = pos;
    newBullet.direction = glm::normalize(dir); // 确保方向向量标准化
    newBullet.speed = speed;
    newBullet.lifetime = lifetime;
    newBullet.maxLifetime = lifetime;
    newBullet.color = color;
    bullets.push_back(newBullet);
}

void mShoot::update(float deltaTime) {
    // 遍历所有子弹并更新位置
    for (size_t i = 0; i < bullets.size(); ) {
        bullets[i].position += bullets[i].direction * bullets[i].speed * deltaTime;
        bullets[i].lifetime -= deltaTime;

        // 移除生命周期结束的子弹
        if (bullets[i].lifetime <= 0.0f) {
            // 与最后一个元素交换并删除最后一个，避免迭代器失效
            bullets[i] = bullets.back();
            bullets.pop_back();
        } else {
            i++;
        }
    }
}

void mShoot::draw(glm::mat4 view, glm::mat4 projection) {
    glEnable(GL_DEPTH_TEST);
    bulletShader->use();
    bulletShader->setMat4("view", view);
    bulletShader->setMat4("projection", projection);

    // 绑定子弹的VAO
    glBindVertexArray(bulletVAO);

    // 渲染每个子弹
    for (const auto& bullet : bullets) {
        // 计算子弹的模型矩阵
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, bullet.position);

        // 设置模型矩阵和颜色
        bulletShader->setMat4("model", model);
        bulletShader->setVec3("bulletColor", bullet.color);

        // 绘制子弹（立方体有36个顶点）
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }

    glBindVertexArray(0);
}

void mShoot::initBuffers() {
    // 生成并绑定VAO和VBO
    glGenVertexArrays(1, &bulletVAO);
    glGenBuffers(1, &bulletVBO);
    glBindVertexArray(bulletVAO);

    glBindBuffer(GL_ARRAY_BUFFER, bulletVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(bulletVertices), bulletVertices, GL_STATIC_DRAW);

    // 只需要位置属性（不再需要纹理坐标）
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    glBindVertexArray(0);
}
