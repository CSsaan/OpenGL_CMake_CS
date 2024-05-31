/**
 * MyModel.hpp skeleton
 * Contributors:
 *      * CS
 * Licence:
 *      * MIT
 */

#pragma once

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

class MyModel : public Application {
 public:
  MyModel();
  ~MyModel() = default;

 protected:
  void loop() override;
  static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
  static void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
  void processInput(GLFWwindow *window);

 private:
  Shader vertexShader{SHADER_DIR "/3dmodel.vert", GL_VERTEX_SHADER};
  Shader fragmentShader{SHADER_DIR "/3dmodel.frag", GL_FRAGMENT_SHADER};
  ShaderProgram shaderProgram{{vertexShader, fragmentShader}};
  std::unique_ptr<Model> mesh = std::make_unique<Model>(RES_DIR "/models/alliance.obj");
  std::unique_ptr<Texture> texture = std::make_unique<Texture>();
  void render();
};

