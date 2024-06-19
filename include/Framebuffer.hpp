/**
 * MySkinsmooth.hpp skeleton
 * Contributors:
 *      * CS
 * Licence:
 *      * MIT
 * usage:
 *      Fbo fbo = Fbo(WINDOW_WIDTH, WINDOW_HEIGHT);
 *      fbo->bind();
 *      fbo->unbind();
 *      fbo.getFboId();
 *      fbo.getTexId();
 *      fbo.bindTexture();
 */

#pragma once

#include <GL/glew.h>

#include "asset.hpp"
#include "glError.hpp"

#include <vector> 
#include <iostream>
using namespace std;

class Fbo {
private:
    GLuint m_renderFbo;
    GLuint m_renderTex;
    GLuint m_renderDepth;

public:
    Fbo(int width, int height) {
        glGenFramebuffers(1, &m_renderFbo);
        glBindFramebuffer(GL_FRAMEBUFFER, m_renderFbo);

        glGenTextures(1, &m_renderTex);
        glBindTexture(GL_TEXTURE_2D, m_renderTex);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_renderTex, 0);

        glGenRenderbuffers(1, &m_renderDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, m_renderDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_renderDepth);

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    ~Fbo() {
        glDeleteFramebuffers(1, &m_renderFbo);
        glDeleteTextures(1, &m_renderTex);
        glDeleteRenderbuffers(1, &m_renderDepth);
    }

    void bind() {
        glBindFramebuffer(GL_FRAMEBUFFER, m_renderFbo);
    }

    void unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    GLuint getFboId() const {
        return m_renderFbo;
    }

    GLuint getTexId() const {
        return m_renderTex;
    }

    void bindTexture(GLuint index = 0) const {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, m_renderTex);
    }
};
