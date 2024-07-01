/**
 * Application.cpp
 * Contributors:
 *      * CS (author)
 * Licence:
 *      * MIT
 */

#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "Texture.hpp"
#include "asset.hpp"
#include "glError.hpp"

using namespace std;

struct GLFWwindow;

/// Application class:
/// * init OpenGL
/// * provide:
///   * getWidth()
///   * getHeight()
///   * getFrameDeltaTime()
///   * getWindowRatio()
///   * windowDimensionChanged()
/// * let the user define the "loop" function.
class Application {
   public:
    Application() {
        cout << "[Info] GLFW initialisation" << endl;
        // initialize the GLFW library
        if (!glfwInit()) {
            throw std::runtime_error("Couldn't init GLFW");
        }
        // setting the opengl version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
        // create the window
        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (!window) {
            glfwTerminate();
            throw std::runtime_error("Couldn't create a window");
        }
        glfwMakeContextCurrent(window);
        // setting window position in the center
        GLFWmonitor* primaryMonitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(primaryMonitor);
        screenWidth = mode->width;
        screenHeight = mode->height;
        glfwSetWindowPos(window, (screenWidth - width) / 2, (screenHeight - height) / 2);
        glewExperimental = GL_TRUE;
        // initialize GLEW
        GLenum err = glewInit();
        if (err != GLEW_OK) {
            glfwTerminate();
            throw std::runtime_error(string("Could initialize GLEW, error = ") + (const char*)glewGetErrorString(err));
        }
        // get version info
        const auto* renderer = glGetString(GL_RENDERER);
        const auto* version = glGetString(GL_VERSION);
        cout << "Renderer: " << renderer << endl;
        cout << "OpenGL version supported " << version << endl;
        // opengl configuration
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        // vsync
        // glfwSwapInterval(false);
        vertices = {
            -1.0, -1.0, 0.0, 0.0, 1.0,
            1.0, -1.0, 0.0, 1.0, 1.0,
            1.0, 1.0, 0.0, 1.0, 0.0,
            -1.0, -1.0, 0.0, 0.0, 1.0,
            1.0, 1.0, 0.0, 1.0, 0.0,
            -1.0, 1.0, 0.0, 0.0, 0.0};
        genVaoVbo();

        // [ImGui] Setup Dear ImGui context
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        (void)io;
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable Gamepad Controls
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        // ImGui::StyleColorsLight();
        //  Setup Platform/Renderer backends
        ImGui_ImplGlfw_InitForOpenGL(window, true);
#ifdef __EMSCRIPTEN__
        ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
#endif
        const char* glsl_version = "#version 130";
        ImGui_ImplOpenGL3_Init(glsl_version);
        // Load Fonts
        // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
        // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
        // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
        // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
        // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
        // - Read 'docs/FONTS.md' for more instructions and details.
        // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
        // - Our Emscripten build process allows embedding fonts to be accessible at runtime from the "fonts/" folder. See Makefile.emscripten for details.
        // io.Fonts->AddFontDefault();
        // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
        // io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
        // ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    }

    ~Application() {
        // [ImGui] Cleanup
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwDestroyWindow(window);
        glfwTerminate();
        if (VAO != 0)
            glDeleteVertexArrays(1, &VAO);
        if (VBO != 0)
            glDeleteBuffers(1, &VBO);
    }

    void genVaoVbo() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glBindVertexArray(0);
        glCheckError(__FILE__, __LINE__);
    }

    // get the window id
    GLFWwindow* getWindow() const {
        return window;
    }

    // window control
    void exit() {
        state = State::stateExit;
    }

    // delta time between frame and time from beginning
    float getFrameDeltaTime() const {
        return deltaTime;
    }

    float getTime() const {
        return time;
    }

    // application run
    void run() {
        state = State::stateRun;
        // // Make the window's context current
        // glfwMakeContextCurrent(window);
        time = glfwGetTime();
        while (state == State::stateRun) {
            // compute new time and delta time
            float t = glfwGetTime();
            deltaTime = t - time;
            time = t;
            // detech window related changes
            detectWindowDimensionChange();
            // execute the frame code
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
            loop();
            // Pool and process events
            glfwPollEvents();

            // [ImGui] Start the Dear ImGui frame
            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
            if (show_demo_window)
                ImGui::ShowDemoWindow(&show_demo_window);
            // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
            {
                static int counter = 0;
                ImGui::Begin("Hello, OpenGL Cmake CS!");  // Create a window called "Hello, world!" and append into it.
                ImGui::BulletText("This is a C++14 CMake project for OpenGL applications by CS.");
                ImGui::BulletText("It includes the following libraries: GLFW, Glew, glm, and assimp.");
                ImGui::BulletText("The project is designed to be cross-platform and can be compiled on Linux, Windows, and Mac.");
                ImGui::SeparatorText("Window Selection");
                ImGui::Checkbox("Demo Window", &show_demo_window);  // Edit bools storing our window open/close state
                ImGui::Checkbox("My Window", &show_another_window);
                ImGui::SeparatorText("General");
                ImGui::ColorEdit3("clear color", (float*)&clear_color);  // Edit 3 floats representing a color
                if (ImGui::Button("Button"))                             // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter=%d", counter);
                ImGuiIO& io = ImGui::GetIO();
                (void)io;
                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
                ImGui::End();
            }
            anotherImGui();  // override this function in derivedClass
            ImGui::Render();
            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

            glfwSwapBuffers(window);  // Swap Front and Back buffers (double buffering)
        }
        glfwTerminate();
    }

    void HelpMarker(const char* desc) {
        ImGui::TextDisabled("(?)");
        if (ImGui::BeginItemTooltip()) {
            ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
            ImGui::TextUnformatted(desc);
            ImGui::PopTextWrapPos();
            ImGui::EndTooltip();
        }
    }

    // Application informations
    int getScreenHeight() const {
        return screenHeight;
    }
    int getScreenWidth() const {
        return screenWidth;
    }
    float getScreenRatio() const {
        return static_cast<float>(screenWidth) / static_cast<float>(screenHeight);
    }
    int getWidth() const {
        return width;
    }
    int getHeight() const {
        return height;
    }
    float getWindowRatio() const {
        return static_cast<float>(width) / static_cast<float>(height);
    }
    bool windowDimensionChanged() const {
        return dimensionChanged;
    }

   protected:
    GLuint VAO = 0, VBO = 0;
    bool show_another_window = true;
    bool show_demo_window = false;
    std::string title = "OpenGL CMake CS";
    std::vector<float> vertices;
    Application(const Application&) {};
    virtual void loop() {
        cout << "[INFO] : loop" << endl;
    }
    virtual void anotherImGui() {
        cout << "[INFO] : anotherImGui" << endl;
    }
    void setWindowAspectRatio(int texWidth, int texHeight) {
        float textureAspectRatio = static_cast<float>(texHeight / texWidth);
        glfwSetWindowSize(window, static_cast<int>(screenWidth / 3), static_cast<int>(screenWidth / 3 * textureAspectRatio));
        glfwSetWindowAspectRatio(window, texWidth, texHeight);
    }

   private:
    enum class State { stateReady,
                       stateRun,
                       stateExit };
    State state = State::stateReady;
    Application& operator=(const Application&) { return *this; }
    GLFWwindow* window = nullptr;
    // Time:
    float time = 0.0f;
    float deltaTime = 0.0f;
    // Dimensions:
    int screenWidth = 0;
    int screenHeight = 0;
    int width = WINDOW_WIDTH;
    int height = WINDOW_HEIGHT;
    bool dimensionChanged = false;
    // [ImGui] Our state
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);

    void detectWindowDimensionChange() {
        int w, h;
        glfwGetWindowSize(getWindow(), &w, &h);
        dimensionChanged = (w != width || h != height);
        if (dimensionChanged) {
            width = w;
            height = h;
            glViewport(0, 0, width, height);
            std::cout << "[WindowSize] width:" << width << ", height:" << height << std::endl;
        }
    }
};
