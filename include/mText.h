//
// Created by 谢豪 on 2025/8/20.
//

#ifndef MTEXT_H
#define MTEXT_H

#pragma once
#include <learnopengl/shader_m.h>
#include <glm/glm.hpp>
#include <map>
#include <string>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

struct Character {
    GLuint TextureID;   // 字形纹理ID
    glm::ivec2 Size;    // 字形大小
    glm::ivec2 Bearing; // 从基准线到字形左上角的偏移量
    GLuint Advance;     // 水平偏移量
};

class mText {
public:
    mText(const std::string& fontPath = "../resources/fonts/Antonio-Bold.ttf");
    ~mText();

    void draw(const std::string& text, float x, float y, float scale,
              const glm::vec3& color, glm::mat4 projection);

private:
    std::map<GLchar, Character> Characters;
    GLuint VAO, VBO;
    Shader* textShader;
};

#endif //MTEXT_H