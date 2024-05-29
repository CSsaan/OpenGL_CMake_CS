/**
 * MyApplication.hpp skeleton
 * Contributors:
 *      * CS
 * Licence:
 *      * MIT
 */

#ifndef MY_MODEL_HPP
#define MY_MODEL_HPP

#include "Application.hpp"
#include "Model.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "camera.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

#include "asset.hpp"
#include "glError.hpp"

class MyApplication : public Application {
 public:
  MyApplication();
  ~MyApplication();

 protected:
  virtual void loop();
  static void mouse_callback(GLFWwindow *window, double xpos, double ypos);         // *鼠标位置回调*
  static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);  // *鼠标滚轮回调*
  void processInput(GLFWwindow *window);                                            // *窗口输入*

 private:
  // shader & program
  Shader vertexShader;
  Shader fragmentShader;
  ShaderProgram shaderProgram;
  // Model
  Model* mesh = nullptr;
  Texture* texture = nullptr;

  // camera
  // std::shared_ptr<Camera> camera{}; //相机位置
};

#endif  // MY_MODEL_HPP
