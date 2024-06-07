#version 430
precision mediump float;
in vec2 o_texcoord;

layout(binding = 0) uniform sampler2D tex_sampler;

layout(location = 0) out vec4 fragColor;

void main()
{       
    fragColor = texture(tex_sampler, o_texcoord);
}
