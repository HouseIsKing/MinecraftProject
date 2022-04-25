#include <iostream>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include "World/SinglePlayerWorld.h"
#include "Shaders/Shader.h"
#include "Entities/Player/PlayerController.h"

using std::cout;
using std::endl;

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: CODE: %d %s\n", error,description);
}

SinglePlayerWorld* helper;

void handleMouseInput(GLFWwindow* window, double mouseX, double mouseY)
{
    helper->handleMouseMovementInput(mouseX, mouseY);
}

void mainLoop(GLFWwindow* window)
{
    Camera cam = Camera(glm::vec3(0,0,0),1280/720.0f);
    CameraController::setActiveCamera(cam);
    double start = glfwGetTime();
    helper = new SinglePlayerWorld(256,64, 256);
    double end = glfwGetTime();
    cout << "World creation took " << end-start << " seconds" << endl;
    glfwSetCursorPosCallback(window, handleMouseInput);
    double counter = 0;
    int framesCompleted = 0;
    while (!glfwWindowShouldClose(window))
    {
        start = glfwGetTime();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        helper->tick();
        helper->drawWorld();
        glfwSwapBuffers(window);
        end = glfwGetTime();
        counter += end - start;
        framesCompleted++;
        if (counter > 1)
        {
            counter -= 1;
            cout << "FPS: " << framesCompleted << std::endl;
            framesCompleted = 0;
        }
        glfwPollEvents();
    }
    delete helper;
}
void GLAPIENTRY MessageCallback( GLenum source,GLenum type,
                 GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userParam ) {
    if(severity != GL_DEBUG_SEVERITY_NOTIFICATION)
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
            (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
            type, severity, message);
}

GLFWwindow* initGLFW()
{
    if(!glfwInit()) {
        cout << "Failed to initialize GLFW" << endl;
        return nullptr;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Minecraft C++ Project", nullptr, nullptr);
    if(!window) {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL(glfwGetProcAddress);
    glfwSetErrorCallback(error_callback);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback,nullptr);
    return window;
}

int main() {
    GLFWwindow* window = initGLFW();
    if(!window) {
        return -1;
    }
    mainLoop(window);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
