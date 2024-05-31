/**
 * MyZebra.hpp skeleton
 * Contributors:
 *      * CS
 * Licence:
 *      * MIT
 */

#pragma once

#include "Application.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_operation.hpp>

#include "asset.hpp"
#include "glError.hpp"

class MyZebra : public Application {
 public:
  MyZebra();
  ~MyZebra() = default;

 protected:
  void loop() override;
  void processInput(GLFWwindow *window);

 private:
  unsigned int VAO{};
  // shader & program
  Shader vertexShader{SHADER_DIR "/Zebra.vert", GL_VERTEX_SHADER};
  Shader fragmentShader{SHADER_DIR "/Zebra.frag", GL_FRAGMENT_SHADER};
  ShaderProgram shaderProgram{{vertexShader, fragmentShader}};
  std::unique_ptr<Texture> texture = std::make_unique<Texture>();
  void render();
};
