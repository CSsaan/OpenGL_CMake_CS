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
  void processInput(GLFWwindow *window);

 private:
  // shader & program
  Shader vertexShader;
  Shader fragmentShader;
  ShaderProgram shaderProgram;
  // Model
  Model* mesh = nullptr;
  Texture* texture = nullptr;

  float startTime = static_cast<float>(glfwGetTime());
  /* Matrices */
  glm::vec3 cam_position = glm::vec3(0.0f, 1.0f, 1.2f);
  glm::mat4 world_matrix = glm::mat4(1.0f);
  glm::mat4 view_matrix = glm::mat4(1.0f);
  glm::mat4 projection_matrix = glm::mat4(1.0f);
};

#endif  // MY_MODEL_HPP
