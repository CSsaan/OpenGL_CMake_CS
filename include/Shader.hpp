/**
 * Shader.hpp
 * Code taken from www.learnopengl.com
 * Contributors:
 *      * Arthur CS (author)
 * Licence:
 *      * MIT
 */
#pragma once

#define GLM_FORCE_RADIANS
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <initializer_list>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

class Shader {
public:
    unsigned int ID; // program ID
    // ------------------------------------------------------------------------
    Shader(const char *vertexPath, const char *fragmentPath) {
        // 1. 从文件路径中获取顶点/片段着色器
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
        try {
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();
            vShaderFile.close();
            fShaderFile.close();
            vertexCode = vShaderStream.str();
            fragmentCode = fShaderStream.str();
        }
        catch (std::ifstream::failure &e) {
            std::cout <<"["<<vertexPath<<"]:" <<"["<<fragmentPath<<"]:" << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ: " << e.what() << std::endl;
        }
        const char *vShaderCode = vertexCode.c_str();
        const char *fShaderCode = fragmentCode.c_str();
        unsigned int vertex, fragment;
        vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &vShaderCode, NULL);
        glCompileShader(vertex);
        checkCompileErrors(vertex, "VERTEX");

        fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &fShaderCode, NULL);
        glCompileShader(fragment);
        checkCompileErrors(fragment, "FRAGMENT");

        ID = glCreateProgram();
        glAttachShader(ID, vertex);
        glAttachShader(ID, fragment);
        glLinkProgram(ID);
        checkCompileErrors(ID, "PROGRAM");

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }
    // ------------------------------------------------------------------------
    void use() const {
        glUseProgram(ID);
    }
    void unuse() const {
        glUseProgram(0);
    }
    // ------------------------------------------------------------------------
    void setUniform1f(const std::string &name, float value) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform1f(have_uniform, value);
    }
    void setUniform2f(const std::string &name, float x, float y) const { 
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform2f(have_uniform, x, y); 
    }
    void setUniform2f(const std::string &name, const glm::vec2 &value) const { 
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform2fv(have_uniform, 1, &value[0]); 
    }
    void setUniform3f(const std::string &name, float x, float y, float z) const { 
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform3f(have_uniform, x, y, z); 
    }
    void setUniform3f(const std::string &name, const glm::vec3 &value) const { 
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform3fv(have_uniform, 1, &value[0]); 
    }
    void setUniform4f(const std::string &name, float x, float y, float z, float w) const { 
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform4f(have_uniform, x, y, z, w); 
    }
    void setUniform4f(const std::string &name, const glm::vec4 &value) const { 
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform4fv(have_uniform, 1, &value[0]); 
    }
    // ------------------------------------------------------------------------
    void setUniformBool(const std::string &name, bool value) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform1i(have_uniform, (int)value);
    }
    void setUniform1i(const std::string &name, int value) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform1i(have_uniform, value);
    }
    void setUniform2i(const std::string &name, int x, int y) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform2i(have_uniform, x, y);
    }
    void setUniform2i(const std::string &name, const glm::ivec2 &value) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform2iv(have_uniform, 1, &value[0]);
    }
    void setUniform3i(const std::string &name, int x, int y, int z) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform3i(have_uniform, x, y, z);
    }
    void setUniform3i(const std::string &name, const glm::ivec3 &value) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform3iv(have_uniform, 1, &value[0]);
    }
    void setUniform4i(const std::string &name, int x, int y, int z, int w) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform4i(have_uniform, x, y, z, w);
    }
    void setUniform4i(const std::string &name, const glm::ivec4 &value) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform4iv(have_uniform, 1, &value[0]);
    }
    // ------------------------------------------------------------------------
    void setUniform1fv(const std::string &name, const std::vector<float> &value) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform1fv(have_uniform, value.size(), &*value.begin());
    }
    void setUniform2fv(const std::string &name, const std::vector<float> &value) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform2fv(have_uniform, value.size() / 2, &*value.begin());
    }
    void setUniform3fv(const std::string &name, const std::vector<float> &value) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform3fv(have_uniform, value.size() / 3, &*value.begin());
    }
    void setUniform4fv(const std::string &name, const std::vector<float> &value) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform4fv(have_uniform, value.size() / 4, &*value.begin());
    }
    void setUniform1iv(const std::string &name, const std::vector<int> &value) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform1iv(have_uniform, value.size(), &*value.begin());
    }
    void setUniform2iv(const std::string &name, const std::vector<int> &value) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform2iv(have_uniform, value.size() / 2, &*value.begin());
    }
    void setUniform3iv(const std::string &name, const std::vector<int> &value) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform3iv(have_uniform, value.size() / 3, &*value.begin());
    }
    void setUniform4iv(const std::string &name, const std::vector<int> &value) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniform4iv(have_uniform, value.size() / 4, &*value.begin());
    }
    // ------------------------------------------------------------------------
    void setUniformMat2(const std::string &name, const glm::mat2 &mat) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniformMatrix2fv(have_uniform, 1, GL_FALSE, &mat[0][0]);
    }
    void setUniformMat3(const std::string &name, const glm::mat3 &mat) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniformMatrix3fv(have_uniform, 1, GL_FALSE, &mat[0][0]);
    }
    void setUniformMat4(const std::string &name, const glm::mat4 &mat) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniformMatrix4fv(have_uniform, 1, GL_FALSE, &mat[0][0]);
    }
    void setUniformMat2x3(const std::string &name, const glm::mat2x3 &mat) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniformMatrix2x3fv(have_uniform, 1, GL_FALSE, &mat[0][0]);
    }
    void setUniformMat3x2(const std::string &name, const glm::mat3x2 &mat) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniformMatrix3x2fv(have_uniform, 1, GL_FALSE, &mat[0][0]);
    }
    void srtUniformMat2x4(const std::string &name, const glm::mat2x4 &mat) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniformMatrix2x4fv(have_uniform, 1, GL_FALSE, &mat[0][0]);
    }
    void setUniformMat4x2(const std::string &name, const glm::mat4x2 &mat) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniformMatrix4x2fv(have_uniform, 1, GL_FALSE, &mat[0][0]);
    }
    void setUniformMat3x4(const std::string &name, const glm::mat3x4 &mat) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniformMatrix3x4fv(have_uniform, 1, GL_FALSE, &mat[0][0]);
    }
    void setUniformMat4x3(const std::string &name, const glm::mat4x3 &mat) const {
        GLint have_uniform = glGetUniformLocation(ID, name.c_str());
        checkUniformLocation(have_uniform, name);
        glUniformMatrix4x3fv(have_uniform, 1, GL_FALSE, &mat[0][0]);
    }

private:
    // ------------------------------------------------------------------------
    void checkCompileErrors(unsigned int shader_program, std::string type) const {
        int success;
        char infoLog[1024];
        if (type != "PROGRAM") { // 顶点、片段着色器都用glGetShaderiv
            glGetShaderiv(shader_program, GL_COMPILE_STATUS, &success);
            if (!success) {
                glGetShaderInfoLog(shader_program, 1024, NULL, infoLog);
                std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
        }
        else { // 程序检查用glGetProgramiv
            glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
            if (!success) {
                glGetProgramInfoLog(shader_program, 1024, NULL, infoLog);
                std::cout <<"ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n"
                          << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
            }
            else { //成功后也要检查（虽然链接成功，但是可能没有把有效的纹理绑定到采样器），一般可以不写
                glValidateProgram(shader_program);
                glGetProgramiv(shader_program, GL_VALIDATE_STATUS, &success);
                if (!success) {
                    glGetProgramInfoLog(shader_program, 1024, NULL, infoLog);
                    std::cout << "ERROR::PROGRAM_VALID_ERROR of type: " << type << "\n"
                              << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
                }
            }
        }
    }
    // ------------------------------------------------------------------------
    void checkUniformLocation(GLint have_uniform, const std::string &name) const {
        if(have_uniform == -1) {
            std::cout << "ERROR::HAVE_NOT_UNIFORM: " << name.c_str() << "\n"
                      << "\n -- --------------------------------------------------- -- " << std::endl;
            // exit(-1);
        }
    }
};
