#version 430
precision highp float;
in vec2 o_texcoord;

layout(binding = 0) uniform sampler2D tex_skinmask;
layout(binding = 1) uniform sampler2D tex_aimask;

uniform vec2 resolu;

layout(location = 0) out vec4 fragColor;

void main()
{
    // SKIN mask
    float skin_msk = texture(tex_skinmask, vec2(o_texcoord.x, o_texcoord.y)).x;
    skin_msk = max(skin_msk, texture(tex_skinmask, vec2(o_texcoord.x - resolu.x, o_texcoord.y - resolu.y)).x);
    skin_msk = max(skin_msk, texture(tex_skinmask, vec2(o_texcoord.x,            o_texcoord.y - resolu.y)).x);
    skin_msk = max(skin_msk, texture(tex_skinmask, vec2(o_texcoord.x + resolu.x, o_texcoord.y - resolu.y)).x);
    skin_msk = max(skin_msk, texture(tex_skinmask, vec2(o_texcoord.x - resolu.x, o_texcoord.y)).x);
    skin_msk = max(skin_msk, texture(tex_skinmask, vec2(o_texcoord.x + resolu.x, o_texcoord.y)).x);       
    skin_msk = max(skin_msk, texture(tex_skinmask, vec2(o_texcoord.x - resolu.x, o_texcoord.y + resolu.y)).x);
    skin_msk = max(skin_msk, texture(tex_skinmask, vec2(o_texcoord.x,            o_texcoord.y + resolu.y)).x);
    skin_msk = max(skin_msk, texture(tex_skinmask, vec2(o_texcoord.x + resolu.x, o_texcoord.y + resolu.y)).x);
       
    // AI mask
    float AI_msk = texture(tex_aimask, vec2(o_texcoord.x, o_texcoord.y)).x;
    AI_msk = max(AI_msk, texture(tex_aimask, vec2(o_texcoord.x - resolu.x, o_texcoord.y - resolu.y)).x);
    AI_msk = max(AI_msk, texture(tex_aimask, vec2(o_texcoord.x,            o_texcoord.y - resolu.y)).x);
    AI_msk = max(AI_msk, texture(tex_aimask, vec2(o_texcoord.x + resolu.x, o_texcoord.y - resolu.y)).x);
    AI_msk = max(AI_msk, texture(tex_aimask, vec2(o_texcoord.x - resolu.x, o_texcoord.y)).x);
    AI_msk = max(AI_msk, texture(tex_aimask, vec2(o_texcoord.x + resolu.x, o_texcoord.y)).x);       
    AI_msk = max(AI_msk, texture(tex_aimask, vec2(o_texcoord.x - resolu.x, o_texcoord.y + resolu.y)).x);
    AI_msk = max(AI_msk, texture(tex_aimask, vec2(o_texcoord.x,            o_texcoord.y + resolu.y)).x);
    AI_msk = max(AI_msk, texture(tex_aimask, vec2(o_texcoord.x + resolu.x, o_texcoord.y + resolu.y)).x);
       
    fragColor = vec4(AI_msk * skin_msk);
}
