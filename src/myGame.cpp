//
// Created by 谢豪 on 2025/8/16.
//
#include "myGame.h"
#include "stb_image.h"
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
         camera = new Camera((glm::vec3(0.0f, 0.0f, 3.0f)));
         sky = new skybox();
         floor = new mFloor();
         model = new mModel();
         mshoot = new mShoot();
         text = new mText();
         cd= new collisionDetector();
         score=0;
     }

     myGame::~myGame() {
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
            glm::mat4 textProjection = glm::ortho(0.0f, (float)SCR_WIDTH, 0.0f, (float)SCR_HEIGHT);
            //draw text
            string scoreText = "score:" + to_string(score);
            text->draw(scoreText.c_str(),0,0,1.0f,glm::vec3(0.0f,0.0f,1.0f), textProjection);

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
         // 随机数引擎（仅初始化一次）
         static std::random_device rd;
         static std::mt19937 gen(rd());
         // 定义0到10之间的均匀分布（不包含10）
         static std::uniform_real_distribution<float> dist(0.0f, 10.0f);

         // 生成X和Z轴随机值，Y轴设为0（或根据需求调整）
         float x = dist(gen);
         float z = dist(gen);

         return glm::vec3(x, 0.0f, z); // Y轴可根据需要修改
     }

void myGame::processInput(GLFWwindow *window)
{

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(RIGHT, deltaTime);
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
         // 左键按下时
         if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
             std::cout << "左键被按下" << std::endl;
             mshoot->shoot(camera->Position,camera->Front,10,3,glm::vec3(1.0f, 0.2f, 0.2f));
             // 可在此处添加交互逻辑（如选中物体）
         }

     }

// utility function for loading a 2D texture from file
// ---------------------------------------------------

}