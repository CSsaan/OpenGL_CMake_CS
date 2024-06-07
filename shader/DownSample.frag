#version 430
precision mediump float;
in vec2 o_texcoord;

layout(binding = 0) uniform sampler2D s_texture;

uniform vec2 offset;
uniform vec2 halfpixel;

layout(location = 0) out vec4 fragColor;

void main()
{
    vec2 uv = o_texcoord;
    vec4 sum = texture(s_texture, uv) * 4.0;
    sum += texture(s_texture, uv - halfpixel.xy * offset);
    sum += texture(s_texture, uv + halfpixel.xy * offset);
    sum += texture(s_texture, uv + vec2(halfpixel.x, -halfpixel.y) * offset);
    sum += texture(s_texture, uv - vec2(halfpixel.x, -halfpixel.y) * offset);
    fragColor = sum / 8.0;
}
