#ifndef MMODEL_H
#define MMODEL_H
#include <learnopengl/shader_m.h>
#include <learnopengl/model.h>
#include <string>
#include "Object.h"

struct modelPosition {
    float x;
    float z;
    float y; //the depth
};

class mModel : public Object {
public:
    // 支持传入模型路径
    mModel(const std::string& modelPath = "nanosuit/nanosuit.obj");
    
    void draw(glm::mat4 view,glm::mat4 projection) override;
    void setP(float x1,float z1,float y1=1.5) override;
    void setScale(glm::vec3 s);
    // 实现 update 方法
    void update(float deltaTime) override;

    modelPosition p;
    glm::vec3 scaleVal;
    float yaw = 0.0f; // 旋转角度
private:
    unsigned int initBuff() override;
    unsigned int loadTexture(char const * path);
    Model* ourModel;
    Shader *modelShader;
};

#endif //MMODEL_H
