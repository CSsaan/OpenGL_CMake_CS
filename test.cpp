#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

class ComputeShader{
public:
    ComputeShader(const std::string& computeShaderPath, GLuint workGroupSize = 128)
        : workGroupSize_(workGroupSize)
    {
        std::ifstream in(computeShaderPath);
        if(!in){
            std::cerr << "Failed to open " << computeShaderPath << std::endl;
            return;
        }
        std::ostringstream source;
        source << in.rdbuf();
        computeShaderSource_ = source.str();

        const GLchar* str = computeShaderSource_.c_str();
        const GLint length = computeShaderSource_.size();
        computeShader_ = glCreateShader(GL_COMPUTE_SHADER);
        glShaderSource(computeShader_, 1, &str, &length);
        glCompileShader(computeShader_);
        glGetShaderiv(computeShader_, GL_COMPILE_STATUS, &compileStatus_);
        if(!compileStatus_){
            std::cerr << "Failed to compile shader" << std::endl;
            return;
        }
        program_ = glCreateProgram();
        glAttachShader(program_, computeShader_);
        glLinkProgram(program_);
        glGetProgramiv(program_, GL_LINK_STATUS, &linkStatus_);
        if(!linkStatus_){
            std::cerr << "Failed to link shader program" << std::endl;
            return;
        }
    }

    ~ComputeShader(){
        glDeleteProgram(program_);
        glDeleteShader(computeShader_);
    }

    void dispatch(GLuint numGroupsX, GLuint numGroupsY, GLuint numGroupsZ){
        glUseProgram(program_);
        glDispatchCompute(numGroupsX, numGroupsY, numGroupsZ);
        glMemoryBarrier(GL_ALL_BARRIER_BITS);
        glUseProgram(0);
    }

    void setUniform(const GLchar* name, GLuint value){
        GLint location = glGetUniformLocation(program_, name);
        glUniform1ui(location, value);
    }

    void setUniform(const GLchar* name, GLfloat value){
        GLint location = glGetUniformLocation(program_, name);
        glUniform1f(location, value);
    }

    void setUniform(const GLchar* name, const glm::vec3& value){
        GLint location = glGetUniformLocation(program_, name);
        glUniform3f(location, value.x, value.y, value.z);
    }

    void setUniform(const GLchar* name, const glm::vec4& value){
        GLint location = glGetUniformLocation(program_, name);
        glUniform4f(location, value.x, value.y, value.z, value.w);
    }

private:
    std::string computeShaderSource_;
    GLuint computeShader_;
    GLuint program_;
    GLint compileStatus_;
    GLint linkStatus_;
    GLuint workGroupSize_;
};

int main(){
    if(!glfwInit()){
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Compute Shader Example", nullptr, nullptr);
    if(!window){
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    if(glewInit() != GLEW_OK){
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
        return -1;
    }

    ComputeShader shader("path/to/compute/shader.glsl");

    while(!glfwWindowShouldClose(window)){
        glfwPollEvents();

        // 在这里设置你的uniform
        shader.setUniform("your_uniform_name", your_value);

        shader.dispatch(1, 1, 1); // 调度计算任务

        glfwSwapBuffers(window);
    }

    glfwTerminate();

    return 0;
}

