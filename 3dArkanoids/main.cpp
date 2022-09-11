#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "Renderer.h"
#include "game.h"
#include "GameInput.h"
#include <iostream>
#include "BlockInstanceRenderData.h"
#include "MockLevelLoader.h"
#include "GrpcLevelEditorServer.h"
#include <functional>
#include <thread>
#include <timeapi.h>
#include "AutoList.h"
#include <memory>

#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include "GameUiOverlay.h"
#include "GameToUiMessage.h"

#define SCR_WIDTH 800
#define SCR_HEIGHT 1200

#define TARGET_MS_PER_FRAME 30

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow* window);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);

Camera camera = Camera();
Renderer* renderer;
Game* gamePtr;

double deltaTime = 0;

static void GLAPIENTRY MessageCallback(GLenum source,
    GLenum type,
    GLuint id,
    GLenum severity,
    GLsizei length,
    const GLchar* message,
    const void* userParam)
{
    fprintf(stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
        (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
        type, severity, message);
}



int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Arkanoids 3D", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);


    // tell GLFW to capture our mouse
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // During init, enable debug output
    //glEnable(GL_DEBUG_OUTPUT);
    //glDebugMessageCallback(MessageCallback, 0);

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    
    //camera.Position.z = (((float)PLAYFIELD_DEPTH_BLOCKS / 2.0f) * (float)BLOCK_DEPTH_UNITS)*10;
    //camera.Position.x = ((float)PLAYFIELD_WIDTH_BLOCKS) * (float)BLOCK_WIDTH_UNITS * 0.5;
    //camera.Position.y = -200.0f;
    ////yaw: -90 pitch: 5
    //camera.Yaw = -90.0f;
    //camera.Pitch = 50.0f;
    //camera.updateCameraVectors();
    using namespace std;
    using namespace std::chrono;
    RendererInitialisationData d = { SCR_WIDTH, SCR_HEIGHT, "BlocksTexture.png", 6 };
    auto renderer = make_shared<Renderer>(d);

    Game game(
        renderer,
        [](ILevelEditorServerGame* g) { return std::make_unique<GrpcLevelEditorServer>(g); });

    GameUiOverlay ui(renderer);
    GameToUiMessage msg{ 420 };
    GameFramework::SendFrameworkMessage(msg);
    const auto& l = AutoList<DrawableLayerBase>::GetList();



    gamePtr = &game;
    

    GameFramework::PushLayers("Gameplay",
        GameLayerType::Draw |
        GameLayerType::Update |
        GameLayerType::Input);

    GameFramework::PushLayers("GameplayUI",
        GameLayerType::Draw);



    Assimp::Importer importer;

    
    //SaveSerializableToSingleBigBinary("level.big");
    LoadSerializableFromSingleBigBinary("level.big");
    game.Init();
    DebugPrintAllSerializableThings();

    auto prevClock = high_resolution_clock::now();

    //SaveSerializableToSingleBigBinary("Level.big");
    // render loop
    // -----------
     while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        auto nextClock = high_resolution_clock::now();
        deltaTime = (nextClock - prevClock).count() / 1e9;
        //printf(" frame time: %.2lf ms\n", deltaTime * 1e3);


        // input
        // -----
        processInput(window);

        // update
        // ------
        GameFramework::Update(deltaTime);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        GameFramework::Draw(camera);

        glfwSwapBuffers(window);
        glfwPollEvents();

        auto frameClock = high_resolution_clock::now();
        double sleepSecs = 1.0 / 60 - (frameClock - nextClock).count() / 1e9;
        if (sleepSecs > 0) {
            timeBeginPeriod(1);
            this_thread::sleep_for(nanoseconds((int64_t)(sleepSecs * 1e9)));
            timeEndPeriod(1);
        }

        prevClock = nextClock;

    }

    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    static glm::vec3 block1Pos(0.0,1.0,0.0);
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        SaveSerializableToSingleBigBinary("level.big");
        std::cout << "Saving " << std::endl;
    }

        //camera.SaveToFile("Camera.cam");
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
    gamePtr->SetScreenDims(glm::ivec2(width, height));
}

double lastX = SCR_WIDTH / 2.0f;
double lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    double xoffset = xpos - lastX;
    double yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    GameInput input = { xoffset, false };
    GameFramework::RecieveInput(input);

    lastX = xpos;
    lastY = ypos;

    //camera.ProcessMouseMovement(xoffset, yoffset);
    //std::cout << "yaw: " << camera.Yaw << " pitch: " << camera.Pitch << std::endl;
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    GameInput input = { 0.0f, false };

    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        input.Firing = true;
    }
    GameFramework::RecieveInput(input);

}