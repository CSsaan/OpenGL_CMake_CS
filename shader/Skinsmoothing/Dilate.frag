#version 430
precision highp float;
in vec2 o_texcoord;

layout(binding = 0) uniform sampler2D tex_sampler;
uniform vec2 offset[81]; // at most 9x9 size
uniform int dilate_size;

layout(location = 0) out vec4 fragColor;

void main()
{
    float AI_mask = texture(tex_sampler, o_texcoord).x;
    for(int z = 0; z < dilate_size * dilate_size; z++){
        AI_mask = max(AI_mask, texture(tex_sampler, o_texcoord + offset[z]).x);
    }     
    fragColor = vec4(AI_mask);
}
