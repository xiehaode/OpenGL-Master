cmake_minimum_required(VERSION 3.28)
project(OpenGLGame2)

set(CMAKE_CXX_STANDARD 11)



set(3RD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rd)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)


set(GLFW_INC ${3RD_DIR}/glfw3/include)
set(GLFW_LIB ${3RD_DIR}/glfw3/lib/glfw3dll.lib)

set(ASSIMP_INC ${3RD_DIR}/assimp/include)
set(ASSIMP_LIB ${3RD_DIR}/assimp/lib/assimp-vc143-mt.lib)

set(FREETYPE_INC ${3RD_DIR}/freetype/include)
set(FREETYPE_LIB ${3RD_DIR}/freetype/lib/freetype.lib)

set(IMGUI_INC ${3RD_DIR}/imgui/include)
set(IMGUI_LIB ${3RD_DIR}/imgui/lib/imgui.lib)


set(SOURCES
        src/main.cpp
        src/glad.c
        src/myGame.cpp
        src/model.cpp
        src/skybox.cpp
        src/mFloor.cpp
        src/mModel.cpp
        src/mShoot.cpp
        src/mText.cpp
        src/collisionDetector.cpp
        src/imgui_impl_glfw.cpp
        src/imgui_impl_opengl3.cpp
        include/imgui_impl_opengl3_loader.h
        include/imgui_impl_glfw.h
        include/imgui_impl_opengl3.h
        include/skybox.h
        include/mFloor.h
        include/mModel.h
        include/mShoot.h
        include/mText.h
        include/collisionDetector.h
        include/ResourceManager.h
)

add_executable(OpenGLGame2 ${SOURCES})

set_target_properties(OpenGLGame2 PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin
        RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_SOURCE_DIR}/bin
        RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_SOURCE_DIR}/bin
)

target_include_directories(OpenGLGame2 PRIVATE
        ${INC_DIR}
        ${SRC_DIR}
        ${GLFW_INC}
        ${ASSIMP_INC}
        ${FREETYPE_INC}
        ${IMGUI_INC}
)


target_link_libraries(OpenGLGame2 PRIVATE
        opengl32
        ${GLFW_LIB}
        ${ASSIMP_LIB}
        ${FREETYPE_LIB}
        ${IMGUI_LIB}
)


