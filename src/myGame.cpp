//
// Created by 谢豪 on 2025/8/16.
//
#include "myGame.h"
#include "stb_image.h"
#include <iostream>
namespace Game{

    unsigned int myGame::SCR_WIDTH = 800;
    unsigned int myGame::SCR_HEIGHT = 600;

    // camera

    Camera* myGame::camera = nullptr;
    float myGame::lastX = 0.0f;
    float myGame::lastY = 0.0f;
    bool myGame::firstMouse = true;
    mShoot *myGame::mshoot = nullptr;

     myGame::myGame() {
         myGlfwInit();
         // 初始位置设为(0, 1.3, 3)，其中1.3 = 地面高度(-0.5) + 玩家高度(1.8)
         camera = new Camera((glm::vec3(0.0f, 1.3f, 3.0f)));
         sky = new skybox();
         floor = new mFloor();
         model = new mModel();
         mshoot = new mShoot();
         text = new mText();
         cd= new collisionDetector();
         score=0;
         
         // 初始化ImGui相关状态
         showMenu = false;
         showText = true;
         initImGui();
     }

     myGame::~myGame() {
         cleanupImGui();
         delete camera;
         delete sky;
         delete floor;
         delete model;
         delete text;
         delete camera;
         delete mshoot;
         delete cd;
     }

     void myGame::start() {
        glEnable(GL_DEPTH_TEST);

        stbi_set_flip_vertically_on_load(true);



        myMainLoop();
    }

    int myGame::myMainLoop() {
        while (!glfwWindowShouldClose(window))
        {

            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;


            for (auto & bullet : mshoot->bullets) {

                BoxCollider skyCollider(glm::vec3(sky->p.x, 0.0f, sky->p.z), glm::vec3(0.5f, 1.0f, 0.5f));
                SphereCollider bulletCollider(bullet.position, 0.1f);
                
                if (collisionDetector::checkCollision(bulletCollider, skyCollider)) {
                    glm::vec3 cur = getRandomXZPosition();
                    sky->setP(cur.x,cur.z);
                    score++;
                }
                

                BoxCollider modelCollider(glm::vec3(model->p.x, 0.0f, model->p.z), glm::vec3(0.5f, 1.0f, 0.5f));
                
                if (collisionDetector::checkCollision(bulletCollider, modelCollider)) {
                    glm::vec3 cur = getRandomXZPosition();
                    model->setP(cur.x,cur.z);
                    score++;
                }
            }

            processInput(window);

            // 物理模拟：应用重力和更新位置
            verticalVelocity += gravity * deltaTime;
            camera->Position.y += verticalVelocity * deltaTime;
            
            // 地面碰撞检测
            if (camera->Position.y < groundHeight) {
                camera->Position.y = groundHeight;
                verticalVelocity = 0.0f;
                isGrounded = true;
            } else {
                isGrounded = false;
            }

            glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

            glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            //draw skybox
            sky->draw(camera->GetViewMatrix(),projection);
            //draw floor
            floor->draw(camera->GetViewMatrix(),projection);
            //draw model
            model->draw(camera->GetViewMatrix(),projection);
            //update mshoot and draw
            mshoot->update(deltaTime);
            mshoot->draw(camera->GetViewMatrix(),projection);
            
            // 只有在showText为true时才绘制文本
            if (showText) {
                glm::mat4 textProjection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT);
                //draw text
                string scoreText = "score:" + to_string(score);
                text->draw(scoreText.c_str(),0,0,1.0f,glm::vec3(0.0f,0.0f,1.0f), textProjection);
            }
            
            // 渲染ImGui
            renderImGui();

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
         glfwTerminate();
         return 0;
    }

    int myGame::myGlfwInit() {
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        #ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        #endif

        // glfw window creation
        // --------------------
        window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
        if (window == NULL)
        {
            std::cout << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
            return -1;
        }
        glfwMakeContextCurrent(window);
        // 设置窗口用户指针，指向当前游戏实例
        glfwSetWindowUserPointer(window, this);
        
        glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
        glfwSetCursorPosCallback(window, mouse_callback);
        glfwSetScrollCallback(window, scroll_callback);
         glfwSetMouseButtonCallback(window, mouse_button_callback);
        // tell GLFW to capture our mouse
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
         // 设置鼠标模式：禁用光标（隐藏并锁定在窗口内）
         //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        // glad: load all OpenGL function pointers
        // ---------------------------------------
        if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        {
            std::cout << "Failed to initialize GLAD" << std::endl;
            return -1;
        }
         // 新增：初始化成功时返回0
         return 0;
    }


    glm::vec3 myGame::getRandomXZPosition() {
         // 随机数引擎
         static std::random_device rd;
         static std::mt19937 gen(rd());
         // 定义0到10之间的均匀分布
         static std::uniform_real_distribution<float> dist(0.0f, 10.0f);

         // 生成X和Z轴随机值，Y轴设为0
         float x = dist(gen);
         float z = dist(gen);

         return glm::vec3(x, 0.0f, z); // Y轴可根据需要修改
     }

void myGame::processInput(GLFWwindow *window)
{
    static bool escPressed = false;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        if (!escPressed) {
            showMenu = !showMenu;
            showText = !showMenu;
            
            // 切换鼠标模式
            if (showMenu) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            }
        }
        escPressed = true;
    } else {
        escPressed = false;
    }

    // 只有在菜单不显示时才处理游戏控制
    if (!showMenu) {
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            camera->ProcessKeyboard(FORWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            camera->ProcessKeyboard(BACKWARD, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            camera->ProcessKeyboard(LEFT, deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            camera->ProcessKeyboard(RIGHT, deltaTime);
        
        // 跳跃控制
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && isGrounded) {
            verticalVelocity = jumpForce;
            isGrounded = false;
        }
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void myGame::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void myGame::mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

        lastX = xpos;
        lastY = ypos;


    camera->ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void myGame::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera->ProcessMouseScroll(static_cast<float>(yoffset));
}

void myGame::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
         // 获取窗口用户指针，这里存储的是myGame实例
         myGame* currentGame = static_cast<myGame*>(glfwGetWindowUserPointer(window));
         
         // 左键按下时，且菜单不显示
         if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && currentGame && !currentGame->showMenu) {
             std::cout << "左键被按下" << std::endl;
             mshoot->shoot(camera->Position,camera->Front,10,3,glm::vec3(1.0f, 0.2f, 0.2f));
             // 可在此处添加交互逻辑（如选中物体）
         }

     }

// utility function for loading a 2D texture from file
// ---------------------------------------------------

void myGame::initImGui() {
    // ImGui初始化
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    // 设置样式
    ImGui::StyleColorsDark();

    // 绑定平台和渲染器
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

void myGame::cleanupImGui() {
    // 清理ImGui
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void myGame::renderImGui() {
    // 开始新的ImGui帧
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    // 如果显示菜单，渲染菜单
    if (showMenu) {
        renderMenu();
    }

    // 渲染ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void myGame::renderMenu() {
    // 创建一个全屏窗口作为菜单背景
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImGui::GetIO().DisplaySize);
    ImGui::Begin("Game Menu", nullptr, 
        ImGuiWindowFlags_NoTitleBar | 
        ImGuiWindowFlags_NoResize | 
        ImGuiWindowFlags_NoMove | 
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoBackground |
        ImGuiWindowFlags_NoBringToFrontOnFocus);

    // 居中显示菜单内容
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 windowPos = ImGui::GetWindowPos();
    
    // 计算居中位置
    float centerX = windowPos.x + windowSize.x * 0.5f;
    float centerY = windowPos.y + windowSize.y * 0.5f;
    
    // 渲染菜单项
    ImGui::SetCursorScreenPos(ImVec2(centerX - 100, centerY - 60));
    ImGui::Text("=== 游戏菜单 ===");
    
    ImGui::SetCursorScreenPos(ImVec2(centerX - 80, centerY - 20));
    ImGui::Text("当前分数: %d", score);
    
    ImGui::SetCursorScreenPos(ImVec2(centerX - 100, centerY + 20));
    if (ImGui::Button("继续游戏", ImVec2(200, 30))) {
        showMenu = false;
        showText = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }
    
    ImGui::SetCursorScreenPos(ImVec2(centerX - 100, centerY + 60));
    if (ImGui::Button("退出游戏", ImVec2(200, 30))) {
        glfwSetWindowShouldClose(window, true);
    }
    
    ImGui::SetCursorScreenPos(ImVec2(centerX - 120, centerY + 100));
    ImGui::Text("按 ESC 键关闭菜单");
    
    ImGui::End();
}

}