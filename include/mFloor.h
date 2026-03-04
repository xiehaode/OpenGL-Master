//
// Created by 谢豪 on 2025/8/19.
//

#ifndef FLOOR_H
#define FLOOR_H



#include <learnopengl/shader_m.h>
#include <vector>
#include "Object.h"

class mFloor : public Object {
public:
     mFloor();
    // 覆盖 draw 方法
    void draw(glm::mat4 view,glm::mat4 projection) override;
    void setP(float x1,float z1,float y1) override;
    void update(float deltaTime) override;
private:
     unsigned int initBuff() override;
     static unsigned int loadTexture(char const * path);
     unsigned int floorTexture;

    Shader *floorShader;
    // plane VAO
    unsigned int planeVAO{}, planeVBO{};



   //  float planeVertices[30] = {
   //      // positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
   //      5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
   //     -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
   //     -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
   //
   //      5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
   //     -5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
   //      5.0f, -0.5f, -5.0f,  2.0f, 2.0f
   // };
   //  float planeVertices[30] = {
   //      // positions          // texture Coords (按比例放大，保持纹理重复密度)
   //      500.0f, -0.5f,  500.0f,  200.0f, 0.0f,   // X和Z乘以100，纹理坐标U乘以100
   //     -500.0f, -0.5f,  500.0f,    0.0f, 0.0f,
   //     -500.0f, -0.5f, -500.0f,    0.0f, 200.0f, // 纹理坐标V乘以100
   //
   //      500.0f, -0.5f,  500.0f,  200.0f, 0.0f,
   //     -500.0f, -0.5f, -500.0f,    0.0f, 200.0f,
   //      500.0f, -0.5f, -500.0f,  200.0f, 200.0f
   // };
    float planeVertices[30] = {
        // positions          // texture Coords (按比例放大，保持纹理重复密度)
        500.0f, -0.0f,  500.0f,  200.0f, 0.0f,   // X和Z乘以100，纹理坐标U乘以100
       -500.0f, -0.0f,  500.0f,    0.0f, 0.0f,
       -500.0f, -0.0f, -500.0f,    0.0f, 200.0f, // 纹理坐标V乘以100

        500.0f, -0.0f,  500.0f,  200.0f, 0.0f,
       -500.0f, -0.0f, -500.0f,    0.0f, 200.0f,
        500.0f, -0.0f, -500.0f,  200.0f, 200.0f
   };
};



#endif //FLOOR_H
