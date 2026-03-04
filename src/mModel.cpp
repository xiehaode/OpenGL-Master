#include "mModel.h"
#include "ResourceManager.h"

mModel::mModel(const std::string& modelPath) {
    p.x = 0.0f;
    p.z = 0.0f;
    p.y = 0.0f;
    scaleVal = glm::vec3(0.005f);
    try {
        modelShader = new Shader(ResourceManager::getShaderPath("model_loading.vert").c_str(), ResourceManager::getShaderPath("model_loading.frag").c_str());
        ourModel = new Model(ResourceManager::getModelPath(modelPath).c_str());
    } catch (...) {
        std::cout << "Failed to load model: " << modelPath << std::endl;
        ourModel = nullptr;
        modelShader = nullptr;
    }
}

void mModel::draw(glm::mat4 view, glm::mat4 projection) {
    if (!ourModel || !modelShader) return;
    
    modelShader->use();
    //create model
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::translate(model, glm::vec3(p.x, p.y, p.z)); // 使用 p.y
    model = glm::rotate(model, glm::radians(yaw), glm::vec3(0.0f, 1.0f, 0.0f)); // 应用旋转
    // reduce scale for spirit
    model = glm::scale(model, scaleVal); 

    modelShader->setMat4("projection", projection);
    modelShader->setMat4("view", view);
    modelShader->setMat4("model", model);
    ourModel->Draw(*modelShader);
}

void mModel::setP(float x1, float z1, float y1) {
    p.x = x1;
    p.z = z1;
    p.y = y1;
}

void mModel::setScale(glm::vec3 s) {
    scaleVal = s;
}

void mModel::update(float deltaTime) {
    // 默认空实现
}

unsigned int mModel::initBuff() {
    return 1;
}

unsigned int mModel::loadTexture(char const *path) {
    return 0;
}
