#ifndef OBJECT_H
#define OBJECT_H

#include <glm/glm.hpp>

// 抽象基类 Object
class Object {
public:
    virtual ~Object() = default;
    
    // 纯虚函数 draw，所有子类必须实现
    // view: 视图矩阵
    // projection: 投影矩阵
    virtual void draw(glm::mat4 view, glm::mat4 projection) = 0;
};

#endif //OBJECT_H
