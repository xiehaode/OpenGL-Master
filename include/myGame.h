//
// Created by 谢豪 on 2025/8/16.
//

#ifndef MYGAME_H
#define MYGAME_H



#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>
#include <learnopengl/camera.h>
#include <learnopengl/model.h>

#include <random>
#include <iostream>
#include <collisionDetector.h>
#include "skybox.h"
#include "mFloor.h"
#include "mModel.h"
#include "mShoot.h"
#include "mText.h"
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace Game {

    // 代表了游戏的当前状态
    enum GameState {
        GAME_ACTIVE,
        GAME_MENU,
        GAME_WIN
    };
    // settings

    class myGame {
    public:
        explicit myGame();
        ~myGame();
        void start();
        int myMainLoop();
        //返回负数表示初始化失败
        int myGlfwInit();
        static void framebuffer_size_callback(GLFWwindow* window, int width, int height);
        static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
        static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
        static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
        void processInput(GLFWwindow *window);
        static glm::vec3 getRandomXZPosition();
        
        // ImGui相关方法
        void initImGui();
        void cleanupImGui();
        void renderImGui();
        void renderMenu();


    private:
        static unsigned int SCR_WIDTH ;
        static unsigned int SCR_HEIGHT;

        // camera

        static Camera* camera;
        static float lastX ;
        static float lastY ;
        static bool firstMouse ;

        // timing
        float deltaTime = 0.0f;
        float lastFrame = 0.0f;

        GLFWwindow* window;
        skybox *sky;
        mFloor *floor;
        mModel *model;
        mText *text;
        collisionDetector *cd;
        static mShoot *mshoot;
        // 游戏状态
        int score;
        GameState  State;
        GLboolean  Keys[1024];
        GLuint     Width, Height;
        
        // ImGui相关
        bool showMenu;
        bool showText;
    };


}

#endif //MYGAME_H
