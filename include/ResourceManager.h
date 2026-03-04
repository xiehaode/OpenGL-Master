#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H

#include <string>
#include <iostream>

#include <direct.h> // for _getcwd on Windows


class ResourceManager {
public:
    static std::string getShaderPath(const std::string& filename) {
        return "../shader/" + filename;
    }
    
    static std::string getTexturePath(const std::string& filename) {
        return "../resources/textures/" + filename;
    }
    
    static std::string getModelPath(const std::string& filename) {
        return "../resources/objects/" + filename;
    }
    
    static std::string getFontPath(const std::string& filename) {
        return "../resources/fonts/" + filename;
    }
    
    static std::string getSkyboxPath(const std::string& filename) {
        return "../resources/textures/skybox/" + filename;
    }
    
    static void printCurrentWorkingDirectory() {
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != nullptr) {
            std::cout << "Current working directory: " << cwd << std::endl;
        } else {
            std::cout << "Unable to get current working directory" << std::endl;
        }
    }
};

#endif // RESOURCE_MANAGER_H