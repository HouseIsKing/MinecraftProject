#include "World/SinglePlayerWorld.h"
#include "Entities/Player/PlayerController.h"
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <iostream>
#include <memory>


using std::cout;
using std::endl;

unique_ptr<SinglePlayerWorld> helper{};
constexpr float TICK_RATE = 0.01666667F;

void ErrorCallback(const int error, const char* description)
{
    std::cout << "Error: CODE: " << error << " " << description << endl;
}

void KeyCallback(GLFWwindow* window, const int key, int /*scancode*/, const int action, int /*mods*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

void WindowsResizeCallback(GLFWwindow* /*window*/, const int width, const int height)
{
	helper->HandleWindowResize(height, width);
}

void MainLoop(GLFWwindow* window)
{
    auto cam = Camera(vec3(0.0F, 0.0F, 0.0F), 1280 / 720.0F);
    CameraController::SetActiveCamera(cam);
    double start = glfwGetTime();
    helper = std::make_unique<SinglePlayerWorld>(static_cast<uint16_t>(256), static_cast<uint16_t>(64), static_cast<uint16_t>(256), window);
    double end = glfwGetTime();
    cout << "World creation took " << end - start << " seconds" << endl;
    double counter = 0;
    float ticksTimer = 0;
    int framesCompleted = 0;
    glClearColor(0.5F, 0.8F, 1.0F, 1.0F);
    glClearDepthf(1.0F);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    bool firstTick = true;
    start = glfwGetTime();
    while (glfwWindowShouldClose(window) == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        int i;
        for (i = 0; static_cast<float>(i) < ticksTimer / TICK_RATE; i++)
        {
            helper->Tick();
        }
        ticksTimer -= static_cast<float>(i) * TICK_RATE;
        helper->DrawWorld();
        end = glfwGetTime();
        if (!firstTick)
        {
            counter += end - start;
            ticksTimer += static_cast<float>(end - start);
            framesCompleted++;
            if (counter > 1)
            {
                counter -= 1;
                cout << "FPS: " << framesCompleted << std::endl;
                framesCompleted = 0;
            }
        }
        firstTick = false;
        start = end;
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void GLAPIENTRY MessageCallback(GLenum /*source*/, const GLenum type,
                                GLuint /*id*/, const GLenum severity, GLsizei /*length*/, const GLchar* message, const void* /*userParam*/)
{
    if (severity != GL_DEBUG_SEVERITY_NOTIFICATION)
    {
        std::cerr << "GL CALLBACK: " << (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "") << " type = " << type << ", severity = " << severity << ", message = " << message << std::endl;
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
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Minecraft C++ Project", nullptr, nullptr);
    if (window == nullptr)
    {
        cout << "Failed to create GLFW window" << endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSetErrorCallback(ErrorCallback);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetFramebufferSizeCallback(window, WindowsResizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);
    glfwSwapInterval(0);
    return window;
}

int main()
{
    GLFWwindow* window = InitGlfw();
    if (window == nullptr)
    {
        return -1;
    }
    MainLoop(window);
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
