#version 430
precision highp float;
layout(binding = 0) uniform sampler2D tex_sampler;

in vec2 o_texcoord;
in vec2 o_texcoordOffset[16];

layout(location = 0) out vec4 fragColor;

vec3 gauss() {
	vec3 sum = vec3(0.0);
	//9x9 或者使用weights(0.227027, 0.1945946, 0.1216216, 0.054054, 0.016216)
	sum += texture(tex_sampler, o_texcoord).rgb * 0.18 * 0.5;
	sum += texture(tex_sampler, o_texcoordOffset[0]).rgb * 0.05 * 0.5;
	sum += texture(tex_sampler, o_texcoordOffset[1]).rgb * 0.09 * 0.5;
	sum += texture(tex_sampler, o_texcoordOffset[2]).rgb * 0.12 * 0.5;
	sum += texture(tex_sampler, o_texcoordOffset[3]).rgb * 0.15 * 0.5;
	sum += texture(tex_sampler, o_texcoordOffset[4]).rgb * 0.15 * 0.5;
	sum += texture(tex_sampler, o_texcoordOffset[5]).rgb * 0.12 * 0.5;
	sum += texture(tex_sampler, o_texcoordOffset[6]).rgb * 0.09 * 0.5;
	sum += texture(tex_sampler, o_texcoordOffset[7]).rgb * 0.05 * 0.5;
	sum += texture(tex_sampler, o_texcoord).rgb * 0.18 * 0.5;
	sum += texture(tex_sampler, o_texcoordOffset[8]).rgb * 0.05 * 0.5;
	sum += texture(tex_sampler, o_texcoordOffset[9]).rgb * 0.09 * 0.5;
	sum += texture(tex_sampler, o_texcoordOffset[10]).rgb * 0.12 * 0.5;
	sum += texture(tex_sampler, o_texcoordOffset[11]).rgb * 0.15 * 0.5;
	sum += texture(tex_sampler, o_texcoordOffset[12]).rgb * 0.15 * 0.5;
	sum += texture(tex_sampler, o_texcoordOffset[13]).rgb * 0.12 * 0.5;
	sum += texture(tex_sampler, o_texcoordOffset[14]).rgb * 0.09 * 0.5;
	sum += texture(tex_sampler, o_texcoordOffset[15]).rgb * 0.05 * 0.5;
	return sum;
}

void main()
{
    vec3 resultColor = gauss();
	fragColor = vec4(resultColor, 1.0);
}
