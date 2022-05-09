#include <iostream>
#include <glad/glad.h>
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

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    helper->handleKeyboardPlayerInput(key, action);
}

void windowsResizeCallback(GLFWwindow* window, int width, int height)
{
    helper->handleWindowResize(height, width);
}

const float tickRate = 0.01666667f;

void mainLoop(GLFWwindow* window)
{
    Camera cam = Camera(glm::vec3(0,0,0),1280/720.0f);
    CameraController::setActiveCamera(cam);
    double start = glfwGetTime();
    helper = new SinglePlayerWorld(256, 64, 256);
    double end = glfwGetTime();
    cout << "World creation took " << end-start << " seconds" << endl;
    glfwSetCursorPosCallback(window, handleMouseInput);
    double counter = 0;
    double ticksTimer = 0;
    int framesCompleted = 0;
    start = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int i;
        for (i = 0; i < ticksTimer / static_cast<double>(tickRate); i++)
        {
            helper->tick();
        }
        ticksTimer -= i * tickRate;
        helper->drawWorld();
        end = glfwGetTime();
        counter += end - start;
        ticksTimer += end - start;
        framesCompleted++;
        if (counter > 1)
        {
            counter -= 1;
            cout << "FPS: " << framesCompleted << std::endl;
            framesCompleted = 0;
        }
        start = end;
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
    delete helper;
}
void GLAPIENTRY MessageCallback( GLenum source,GLenum type,
                 GLuint id,GLenum severity,GLsizei length,const GLchar* message,const void* userParam ) {
    if(severity != GL_DEBUG_SEVERITY_NOTIFICATION)
    {
        fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
                (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
                type, severity, message);
    }
}

GLFWwindow* InitGlfw()
{
    if (glfwInit() == 0)
    {
        cout << "Failed to initialize GLFW" << endl;
        return nullptr;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Minecraft C++ Project", nullptr, nullptr);
    if (!window)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSetErrorCallback(error_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetFramebufferSizeCallback(window, windowsResizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback,nullptr);
    return window;
}

int main()
{
    GLFWwindow* window = InitGlfw();
    if (window == nullptr)
    {
        return -1;
    }
    mainLoop(window);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
