//
// Created by 谢豪 on 2025/8/20.
//

#ifndef MMODEL_H
#define MMODEL_H
#include <learnopengl/shader_m.h>
#include <learnopengl/model.h>
#include "Object.h"

struct modelPosition {
    float x;
    float z;
};

class mModel : public Object {
public:
    mModel();
    // 覆盖 draw 方法
    void draw(glm::mat4 view,glm::mat4 projection) override;
    void setP(float x1,float z1);
    modelPosition p;
private:
    void initBuff();
    unsigned int loadTexture(char const * path);
    Model* ourModel;

    Shader *modelShader;
    // plane VAO
    unsigned int modelVAO, modelVBO;
};


#endif //MMODEL_H
