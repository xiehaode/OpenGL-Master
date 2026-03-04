//
// Created by 谢豪 on 2025/8/20.
//

#ifndef MTEXT_H
#define MTEXT_H

#include <learnopengl/shader_m.h>
#include <glm/glm.hpp>
#include <map>
#include <string>

class mText {
public:
    mText(const std::string& fontPath = "../resources/fonts/Antonio-Bold.ttf");
    ~mText();

    // 渲染文本 - 简化版本，只在控制台输出
    void draw(const std::string& text, float x, float y, float scale,
              const glm::vec3& color, glm::mat4 projection) {
        // 简化实现，只在控制台输出分数
        std::cout << "Score: " << text << std::endl;
    }

private:
    // 简化实现，不需要实际的文本渲染
};

#endif //MTEXT_H