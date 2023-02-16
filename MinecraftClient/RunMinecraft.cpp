#include "Network/ClientNetworkManager.h"
#include "Entities/Generic/CameraController.h"
#include "World/MP/MultiPlayerWorld.h"
#include "World/SP/SinglePlayerWorld.h"
#include <iostream>
#include <memory>

std::unique_ptr<SinglePlayerWorld> spWorld{};
std::unique_ptr<MultiPlayerWorld> mpWorld{};

void ErrorCallback(const int error, const char* description)
{
    std::cout << "Error: CODE: " << error << " " << description << std::endl;
}

void KeyCallback(GLFWwindow* window, const int key, int /*scancode*/, const int action, int /*mods*/)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    else if (mpWorld)
    {
        mpWorld->HandleKeyCallback(key, action);
    }
    else if (spWorld)
    {
        spWorld->HandleKeyCallback(key, action);
    }
}

void CursorPosCallback(GLFWwindow* /*window*/, const double xPos, const double yPos)
{
    if (mpWorld)
    {
        mpWorld->HandleCursorPosCallback(static_cast<float>(xPos), static_cast<float>(yPos));
    }
    else
    {
        spWorld->HandleCursorPosCallback(static_cast<float>(xPos), static_cast<float>(yPos));
    }
}

void MouseButtonCallback(GLFWwindow* /*window*/, const int button, const int action, int /*mods*/)
{
    if (mpWorld)
    {
        mpWorld->HandleMouseButtonCallback(button, action);
    }
    else
    {
        spWorld->HandleMouseButtonCallback(button, action);
    }
}

void WindowsResizeCallback(GLFWwindow* /*window*/, const int width, const int height)
{
    if (width != 0 && height != 0)
    {
        if (spWorld)
        {
            spWorld->HandleWindowResize(height, width);
        }
        else
        {
            mpWorld->HandleWindowResize(height, width);
        }
    }
}

void MainLoopMulti(GLFWwindow* window, const std::string& ip, const std::string& name)
{
    auto cam = Camera(glm::vec3(0.0F), 1920.0F / 1080.0F);
    CameraController::SetActiveCamera(cam);
    const double start = glfwGetTime();
    mpWorld = std::make_unique<MultiPlayerWorld>(window, name, ip);
    std::cout << "World creation took " << glfwGetTime() - start << " seconds" << std::endl;
    glfwPollEvents();
    glClearColor(0.5F, 0.8F, 1.0F, 1.0F);
    glClearDepthf(1.0F);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    while (glfwWindowShouldClose(window) == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        mpWorld->Run();
        glfwPollEvents();
        glfwSwapBuffers(window);
    }
}

void MainLoop(GLFWwindow* window)
{
    auto cam = Camera(glm::vec3(0.0F), 1920.0F / 1080.0F);
    CameraController::SetActiveCamera(cam);
    const double start = glfwGetTime();
    spWorld = std::make_unique<SinglePlayerWorld>(static_cast<uint16_t>(256), static_cast<uint16_t>(64), static_cast<uint16_t>(256), window);
    std::cout << "World creation took " << glfwGetTime() - start << " seconds" << std::endl;
    glfwPollEvents();
    glClearColor(0.5F, 0.8F, 1.0F, 1.0F);
    glClearDepthf(1.0F);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    while (glfwWindowShouldClose(window) == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        spWorld->Run();
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
        std::cout << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Minecraft C++ Project", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return nullptr;
    }
    glfwMakeContextCurrent(window);
    gladLoadGL();
    glfwSetErrorCallback(ErrorCallback);
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, CursorPosCallback);
    glfwSetMouseButtonCallback(window, MouseButtonCallback);
    glfwSetFramebufferSizeCallback(window, WindowsResizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);
    glfwSwapInterval(0);
    return window;
}

int main(const int argc, char* argv[])
{
    GLFWwindow* window = InitGlfw();
    if (window == nullptr)
    {
        return -1;
    }
    if (argc > 3 && std::string(argv[1]) == "Server")
    {
        MainLoopMulti(window, argv[3], argv[2]);
    }
    else
    {
        MainLoop(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
