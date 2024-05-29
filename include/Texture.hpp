/** 
 * Code taken from www.learnopengl.com
 * Contributors:
 *      * Arthur CS (author)
 **/

#pragma once
#include <string>
#include <GL/glew.h>

class Texture
{
public:
    Texture();
    ~Texture();

    bool load(const std::string & file_name);
    
    void bind(int index = 0) const
    {
        if(to_id != 0)
        {
            glActiveTexture(GL_TEXTURE0 + index);
            glBindTexture(GL_TEXTURE_2D, to_id);
        }
    }

    bool use_linear;

private:
    GLuint to_id;
};
