#version 430

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 texcoord;

out vec3 o_position;
out vec3 o_normal;
out vec2 o_texcoord;
	
uniform mat4 model;
uniform mat4 viewProj;
uniform mat4 projection;
	
void main()
{
	o_position = vec3(model * vec4(position, 1.0f));
    o_normal   = mat3(transpose(inverse(model))) * normal;
    o_texcoord = texcoord.xy;
	
    gl_Position = projection * viewProj * model * vec4(position, 1.0f);
}