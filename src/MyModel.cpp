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

static float lastX = WINDOW_WIDTH / 2.0f;
static float lastY = WINDOW_HEIGHT / 2.0f;
static bool firstMouse = true;
Camera camera(glm::vec3(0.0f, 0.0f, 2.0f));

// timing
static float mDeltaTime = 0.0f; // 上下帧时间间隔初始化
static float lastFrame = 0.0f;

MyApplication::MyApplication()
    : Application(),
      vertexShader(SHADER_DIR "/Basic.vert", GL_VERTEX_SHADER),
      fragmentShader(SHADER_DIR "/Basic.frag", GL_FRAGMENT_SHADER),
      shaderProgram({vertexShader, fragmentShader}) 
{
  glCheckError(__FILE__, __LINE__);

  mesh = new Model(RES_DIR "/models/alliance.obj");
  shaderProgram.use();

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
  // 获取前后帧时间间隔
  float currentFrame = static_cast<float>(glfwGetTime());
  mDeltaTime = currentFrame - lastFrame;
  lastFrame = currentFrame;
  // window input
  processInput(getWindow());
  glfwSetCursorPosCallback(getWindow(), mouse_callback);            // *鼠标位置回调*
  glfwSetScrollCallback(getWindow(), scroll_callback);              // *鼠标滚轮回调*
  glfwSetInputMode(getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED); // *告诉GLFW捕捉鼠标*
  // exit on window close button pressed
  if (glfwWindowShouldClose(getWindow()))
    exit();
  // clear
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(0.6784f, 0.8f, 1.0f, 1.0f);

  /* Draw our triangle */
  shaderProgram.use();
  glm::mat4 model = glm::mat4(1.0f);
  model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));               // *不移动，在屏幕中间*
  model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0, 1.0, 0.0)); //*旋转（x,y,z轴）*
  model = glm::scale(model, glm::vec3(1.0f));                               // *缩小模型*
  glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
  shaderProgram.setUniform("model", model);
  shaderProgram.setUniform("viewProj", camera.GetViewMatrix());
  shaderProgram.setUniform("projection", projection);
  shaderProgram.setUniform("cam_pos", camera.Position);
  glCheckError(__FILE__, __LINE__);
  // texture->bind();
  mesh->Draw();
  glCheckError(__FILE__, __LINE__);

  shaderProgram.unuse();
}

void MyApplication::mouse_callback(GLFWwindow* window, double xposIn, double yposIn) 
{
  float xpos = static_cast<float>(xposIn);
  float ypos = static_cast<float>(yposIn);
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;  // *不是第一次使用鼠标*
  }
  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;
  lastX = xpos;
  lastY = ypos;
  camera.ProcessMouseMovement(xoffset, yoffset);
}

void MyApplication::scroll_callback(GLFWwindow* window, double xoffset, double yoffset) 
{
  camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void MyApplication::processInput(GLFWwindow* window) 
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    camera.ProcessKeyboard(FORWARD, mDeltaTime);
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    camera.ProcessKeyboard(BACKWARD, mDeltaTime);
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    camera.ProcessKeyboard(LEFT, mDeltaTime);
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    camera.ProcessKeyboard(RIGHT, mDeltaTime);
}
