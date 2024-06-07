#version 430
precision highp float;
in vec2 o_texcoord;

layout(binding = 0) uniform sampler2D tex_mask;

uniform vec2 blur_offset[225];
uniform float blur_kernel[225]; // 最多支持15x15卷积核
uniform int blur_count;         // 卷积核个数

layout(location = 0) out vec4 fragColor;

void main()
{
    float blur = 0.0;
    for(int z = 0; z < blur_count * blur_count; z++) {
        blur += texture(tex_mask, o_texcoord + blur_offset[z]).x * blur_kernel[z];   
    }
    fragColor = vec4(blur);
    //fragColor = texture(s_IN_mask, v_texCoord);
}
