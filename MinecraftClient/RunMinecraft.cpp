#include "Network/ClientNetworkManager.h"
#include "World/SP/SinglePlayerWorld.h"
#include <iostream>
#include <memory>
#include "Entities/Generic/CameraController.h"
#include "World/MP/MultiPlayerWorld.h"

std::unique_ptr<World<SinglePlayerWorld, PlayerController>> spWorld{};
std::unique_ptr<World<MultiPlayerWorld, PlayerController>> mpWorld{};
constexpr float TICK_RATE = 1.0F / 20.0F;

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
}

void WindowsResizeCallback(GLFWwindow* /*window*/, const int width, const int height)
{
    if (width != 0 && height != 0)
    {
        if (spWorld)
        {
            spWorld->HandleWindowResize(width, height);
        }
        else
        {
            mpWorld->HandleWindowResize(width, height);
        }
    }
}

void MainLoop(GLFWwindow* window)
{
    auto cam = Camera(glm::vec3(0.0F), 1920.0F / 1080.0F);
    CameraController::SetActiveCamera(cam);
    const double start = glfwGetTime();
    spWorld = std::make_unique<SinglePlayerWorld>(static_cast<uint16_t>(256), static_cast<uint16_t>(64), static_cast<uint16_t>(256), window);
    auto* world = dynamic_cast<SinglePlayerWorld*>(spWorld.get());
    std::cout << "World creation took " << glfwGetTime() - start << " seconds" << std::endl;
    glClearColor(0.5F, 0.8F, 1.0F, 1.0F);
    glClearDepthf(1.0F);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    while (glfwWindowShouldClose(window) == 0)
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        world->Run();
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
    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Minecraft C++ Project", glfwGetPrimaryMonitor(), nullptr);
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
    glfwSetFramebufferSizeCallback(window, WindowsResizeCallback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(MessageCallback, nullptr);
    glfwSwapInterval(0);
    return window;
}

int main(const int argc, char* argv[])
{
    if (argc > 2 && std::string(argv[1]) == "Server")
    {
        ClientNetworkManager manager;
        manager.Start("127.0.0.1", argv[2]);
    }
    else
    {
        GLFWwindow* window = InitGlfw();
        if (window == nullptr)
        {
            return -1;
        }
        MainLoop(window);
        glfwDestroyWindow(window);
        glfwTerminate();
    }
    return 0;
}
