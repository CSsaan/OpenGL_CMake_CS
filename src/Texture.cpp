/** 
 * Code taken from www.learnopengl.com
 * Contributors:
 *      * Arthur CS (author)
 **/

#include <stb_image.h>
#include "Texture.hpp"
#include <iostream>
#include "asset.hpp"

Texture::Texture()
    : use_linear(true), to_id(0)
{
}

Texture::~Texture()
{
    if(to_id != 0)
    {
        glDeleteTextures(1, &to_id);
        to_id = 0;
    }
}

bool Texture::load(const std::string & file_name)
{
    if(file_name.empty())
    {
        return false;
    }

    bool is_loaded = false;
    int width, height, components;
    unsigned char* pixels = stbi_load((file_name).c_str(), &width, &height, &components, 0);

    if(pixels != nullptr)
    {
        GLenum internal_format = GL_RGB;
        GLenum format = GL_RGB;
        if(components == 1)
        {
            internal_format = GL_R8;
            format = GL_RED;
        }
        else if(components == 2)
        {
            internal_format = GL_RG8;
            format = GL_RG;
        }
        else if(components == 3)
        {
            internal_format = GL_RGB8;
            format = GL_RGB;
        }
        else if(components == 4)
        {
            internal_format = GL_RGBA8;
            format = GL_RGBA;
        }
        glGenTextures(1, &to_id);
        glBindTexture(GL_TEXTURE_2D, to_id);
        glTexStorage2D(GL_TEXTURE_2D, 2 /* mip map levels */, internal_format, width, height);
        glTexSubImage2D(GL_TEXTURE_2D, 0 /* mip map level */, 0 /* xoffset */, 0 /* yoffset */, width, height, format, GL_UNSIGNED_BYTE, pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, use_linear ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, use_linear ? GL_LINEAR : GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        std::cout << "Texture '" << file_name << "' loaded " << "(width:" << width << ", height:" << height << ")" << std::endl;
        is_loaded = true;
    }
    else
    {
        std::cout << "Could not load file " << file_name << std::endl;
    }

    stbi_image_free(pixels);

    return is_loaded;
}
