#version 430
precision highp float;
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

uniform vec2 iResolution;
uniform mat4 model;

out vec2 o_texcoord;
out vec2 o_texcoordOffset[16];

void main()
{
    float x_singleStepOffset = 2.0/iResolution.x; //  2.0/1920.0;
    float y_singleStepOffset = 2.0/iResolution.y; //  2.0/1080.0;
    o_texcoordOffset[0] = texcoord + vec2(-4.0 * x_singleStepOffset, 0.0);
    o_texcoordOffset[1] = texcoord + vec2(-3.0 * x_singleStepOffset, 0.0);
    o_texcoordOffset[2] = texcoord + vec2(-2.0 * x_singleStepOffset, 0.0);
    o_texcoordOffset[3] = texcoord + vec2(-1.0 * x_singleStepOffset, 0.0);
    o_texcoordOffset[4] = texcoord + vec2(1.0 * x_singleStepOffset, 0.0);
    o_texcoordOffset[5] = texcoord + vec2(2.0 * x_singleStepOffset, 0.0);
    o_texcoordOffset[6] = texcoord + vec2(3.0 * x_singleStepOffset, 0.0);
    o_texcoordOffset[7] = texcoord + vec2(4.0 * x_singleStepOffset, 0.0);

    o_texcoordOffset[8] = texcoord + vec2(0.0, -4.0 * y_singleStepOffset);
    o_texcoordOffset[9] = texcoord + vec2(0.0, -3.0 * y_singleStepOffset);
    o_texcoordOffset[10] = texcoord + vec2(0.0, -2.0 * y_singleStepOffset);
    o_texcoordOffset[11] = texcoord + vec2(0.0, -1.0 * y_singleStepOffset);
    o_texcoordOffset[12] = texcoord + vec2(0.0, 1.0 * y_singleStepOffset);
    o_texcoordOffset[13] = texcoord + vec2(0.0, 2.0 * y_singleStepOffset);
    o_texcoordOffset[14] = texcoord + vec2(0.0, 3.0 * y_singleStepOffset);
    o_texcoordOffset[15] = texcoord + vec2(0.0, 4.0 * y_singleStepOffset);

    o_texcoord = texcoord;
    gl_Position = model * vec4(position, 1.0);
}
