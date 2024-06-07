/**
 * Application.cpp
 * Contributors:
 *      * CS (author)
 * Licence:
 *      * MIT
 */

#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <stdexcept>
#include <string>
#include <memory>
#include <vector>

#include "asset.hpp"

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
class Application 
{
 public:
  Application() 
  {
    cout << "[Info] GLFW initialisation" << endl;
    // initialize the GLFW library
    if (!glfwInit()) {
      throw std::runtime_error("Couldn't init GLFW");
    }
    // setting the opengl version
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
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
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (err != GLEW_OK) {
      glfwTerminate();
      throw std::runtime_error(string("Could initialize GLEW, error = ") +
                              (const char*)glewGetErrorString(err));
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
      -1.0, -1.0, 0.0,  0.0, 1.0,
      1.0, -1.0, 0.0,  1.0, 1.0,
      1.0,  1.0, 0.0,  1.0, 0.0,
      -1.0, -1.0, 0.0,  0.0, 1.0,
      1.0,  1.0, 0.0,  1.0, 0.0,
      -1.0,  1.0, 0.0,  0.0, 0.0
    };
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
    // Make the window's context current
    glfwMakeContextCurrent(window);
    time = glfwGetTime();
    while (state == State::stateRun) {
      // compute new time and delta time
      float t = glfwGetTime();
      deltaTime = t - time;
      time = t;
      // detech window related changes
      detectWindowDimensionChange();
      // execute the frame code
      loop();
      // Swap Front and Back buffers (double buffering)
      glfwSwapBuffers(window);
      // Pool and process events
      glfwPollEvents();
    }
    glfwTerminate();
  }

  // Application informations
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
  std::string title = "Application";
  std::vector<float> vertices;
  Application(const Application&){};
  virtual void loop(){
    cout << "[INFO] : loop" << endl;
  }
  
 private:
  enum class State { stateReady, stateRun, stateExit };
  State state = State::stateReady;
  Application& operator=(const Application&) { return *this; }
  GLFWwindow* window = nullptr;
  // Time:
  float time = 0.0f;
  float deltaTime = 0.0f;
  // Dimensions:
  int width = WINDOW_WIDTH;
  int height = WINDOW_HEIGHT;
  bool dimensionChanged = false;

  void detectWindowDimensionChange() 
  {
    int w, h;
    glfwGetWindowSize(getWindow(), &w, &h);
    dimensionChanged = (w != width || h != height);
    if (dimensionChanged) {
      width = w;
      height = h;
      glViewport(0, 0, width, height);
      std::cout << "width, height: " << width << height << std::endl;
    }
  }
};

