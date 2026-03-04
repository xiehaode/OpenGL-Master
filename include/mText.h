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


    void draw(const std::string& text, float x, float y, float scale,
              const glm::vec3& color, glm::mat4 projection) {

    }

private:

};

#endif //MTEXT_H