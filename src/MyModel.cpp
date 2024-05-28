/**
 * MyApplication.cpp
 * Contributors:
 *      * CS (author)
 * Licence:
 *      * MIT
 */
#include "MyModel.hpp"

#include <iostream>
#include <vector>

MyApplication::MyApplication()
    : Application(),
      vertexShader(SHADER_DIR "/Basic.vert", GL_VERTEX_SHADER),
      fragmentShader(SHADER_DIR "/Basic.frag", GL_FRAGMENT_SHADER),
      shaderProgram({vertexShader, fragmentShader}) 
{
  glCheckError(__FILE__, __LINE__);

  world_matrix = glm::mat4(1.0f);
  view_matrix = glm::lookAt(cam_position, glm::vec3(0.0f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
  projection_matrix = glm::perspectiveFov(glm::radians(60.0f), float(WINDOW_WIDTH), float(WINDOW_HEIGHT), 0.1f, 10.0f);

  // model
  mesh = new Model(RES_DIR "/models/alliance.obj");
  shaderProgram.use();

  shaderProgram.setUniform("world", world_matrix);
  shaderProgram.setUniform(
      "normalMatrix", glm::inverse(glm::transpose(glm::mat3(world_matrix))));
  shaderProgram.setUniform("viewProj", projection_matrix * view_matrix);
  shaderProgram.setUniform("cam_pos", cam_position);

  texture = new Texture();
  texture->load(RES_DIR "/models/alliance.png");
  texture->bind();

  // creation of the mesh ------------------------------------------------------

  // creation of the vertex array buffer----------------------------------------

  // vao end
}

MyApplication::~MyApplication() 
{
  delete mesh;
  delete texture;
}

void MyApplication::loop() 
{
  // window input
  processInput(getWindow());

  /* Update game time value */
  float gameTime = static_cast<float>(glfwGetTime()) - startTime;

  // exit on window close button pressed
  if (glfwWindowShouldClose(getWindow()))
    exit();

  // clear
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.6784f, 0.8f, 1.0f, 1.0f);

  shaderProgram.use();

  /* Draw our triangle */
  world_matrix = glm::rotate(glm::mat4(1.0f), gameTime * glm::radians(-90.0f), glm::vec3(0, 1, 0));
  shaderProgram.setUniform("world", world_matrix);
  shaderProgram.setUniform("normalMatrix", glm::inverse(glm::transpose(glm::mat3(world_matrix))));
  glCheckError(__FILE__, __LINE__);
  shaderProgram.use();
  texture->bind();
  mesh->Draw();
  glCheckError(__FILE__, __LINE__);

  shaderProgram.unuse();
}

void MyApplication::processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);
}