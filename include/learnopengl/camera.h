#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

// Default camera values
const float YAW         = -90.0f;
const float PITCH       =  0.0f;
const float SPEED       =  2.5f;
const float SENSITIVITY =  0.1f;
const float ZOOM        =  45.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:

    /*变量名
    Position	摄像机位置：摄像机在世界坐标系中的三维坐标（x, y, z），是观察点的空间位置。
    Front	摄像机指向方向：摄像机镜头朝向的单位向量（指向被观察的方向），例如初始值为 (0,0,-1) 表示默认看向 Z 轴负方向。
    Up	摄像机局部上轴：摄像机自身坐标系的 “上方向” 单位向量（相对于摄像机当前朝向，非世界坐标系的上方向）。
    Right	摄像机局部右轴：摄像机自身坐标系的 “右方向” 单位向量（与 Front、Up 向量垂直，构成摄像机局部坐标系）。
    WorldUp	世界坐标系上轴：固定的世界坐标系 “上方向” 向量（通常为 (0,1,0)，即 Y 轴正方向，用于辅助计算摄像机局部*/
    // camera Attributes
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    // euler Angles
    /*变量名	含义
    Yaw	偏航角：绕 WorldUp 轴（通常是世界 Y 轴）旋转的角度，控制摄像机左右转动（水平方向视角）。初始值 - 90° 表示默认看向 Z 轴负方向（与 OpenGL 坐标系一致）。
    Pitch	俯仰角：绕摄像机 Right 轴旋转的角度，控制摄像机上下转动（垂直方向视角）。范围被限制在 ±89°（避免超过 90° 导致画面翻转）。*/
    float Yaw;
    float Pitch;
    // camera options
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;

    // constructor with vectors
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = position;
        WorldUp = up;
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }
    // constructor with scalar values
    Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
    {
        Position = glm::vec3(posX, posY, posZ);
        WorldUp = glm::vec3(upX, upY, upZ);
        Yaw = yaw;
        Pitch = pitch;
        updateCameraVectors();
    }

    // returns the view matrix calculated using Euler Angles and the LookAt Matrix
    glm::mat4 GetViewMatrix()
    {
        //cameraDirection(摄像机方向) = cameraPos(摄像机位置) - cameraTarget(摄像机指向场景)
        return glm::lookAt(Position, Position + Front, Up);
    }

    // processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
    void ProcessKeyboard(Camera_Movement direction, float deltaTime)
    {
        float velocity = MovementSpeed * deltaTime;

        // 生成水平方向的前向向量（移除Y分量，确保在XY平面移动）
        glm::vec3 horizontalFront = glm::vec3(Front.x, 0.0f, Front.z);
        // 归一化处理，避免斜向移动时速度变快
        horizontalFront = glm::normalize(horizontalFront);

        // 右向量本身通常在水平面上，若有倾斜可同样移除Y分量
        glm::vec3 horizontalRight = glm::vec3(Right.x, 0.0f, Right.z);
        horizontalRight = glm::normalize(horizontalRight);

        if (direction == FORWARD)
            Position += horizontalFront * velocity;
        if (direction == BACKWARD)
            Position -= horizontalFront * velocity;
        if (direction == LEFT)
            Position -= horizontalRight * velocity;
        if (direction == RIGHT)
            Position += horizontalRight * velocity;

        // 可选：强制锁定Y坐标（防止因浮点误差导致的微小垂直偏移）
        Position.y = 0.0f; // 或设置为固定高度，如摄像机初始Y值
    }

    // processes input received from a mouse input system. Expects the offset value in both the x and y direction.
    void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
    {
        xoffset *= MouseSensitivity;
        yoffset *= MouseSensitivity;

        Yaw   += xoffset*0.1f;
        Pitch += yoffset*0.1f;

        // make sure that when pitch is out of bounds, screen doesn't get flipped
        if (constrainPitch)
        {
            if (Pitch > 89.0f)
                Pitch = 89.0f;
            if (Pitch < -89.0f)
                Pitch = -89.0f;
        }

        // update Front, Right and Up Vectors using the updated Euler angles
        updateCameraVectors();
    }

    // processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
    void ProcessMouseScroll(float yoffset)
    {
        Zoom -= (float)yoffset;
        if (Zoom < 1.0f)
            Zoom = 1.0f;
        if (Zoom > 45.0f)
            Zoom = 45.0f;
    }

private:
    // calculates the front vector from the Camera's (updated) Euler Angles
    void updateCameraVectors()
    {
        // calculate the new Front vector
        glm::vec3 front;
        front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        front.y = sin(glm::radians(Pitch));
        front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
        Front = glm::normalize(front);
        // also re-calculate the Right and Up vector
        Right = glm::normalize(glm::cross(Front, WorldUp));  // normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
        Up    = glm::normalize(glm::cross(Right, Front));
    }
};
#endif
