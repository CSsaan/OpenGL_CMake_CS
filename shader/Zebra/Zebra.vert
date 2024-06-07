#version 430
precision mediump float;
layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texcoord;

uniform mat4 model;

out vec2 o_texcoord;
	
void main()
{
    o_texcoord = texcoord.xy;
    gl_Position = model * vec4(position, 1.0f);
}