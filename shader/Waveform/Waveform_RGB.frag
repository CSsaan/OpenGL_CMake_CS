#version 430
precision mediump float;
in vec2 o_texcoord;

layout(binding = 0) uniform sampler2D tex_sampler;

uniform vec2 iResolution;

layout(location = 0) out vec4 fragColor;

#define HIGH_QUALITY
#ifndef HIGH_QUALITY
	const int hres = 200;
	const float intensity = 0.04;
	const float thres = 0.004;
#else
	const int hres = 1080;
	const float intensity = 0.04;
	const float thres = 1.0/iResolution.y;
#endif

void main()
{
	vec2 uv = o_texcoord;
    vec3 col = vec3(0.0);
    float s = uv.y * 1.890 - 0.045;
    float maxb = s+thres;
    float minb = s-thres;
    
    for (int i = 0; i <= hres; i++) {
        vec3 x = texture(tex_sampler, vec2(uv.x, float(i)/float(hres))).rgb*1.8;
		col += vec3(intensity)*step(x, vec3(maxb))*step(vec3(minb), x);

		x = texture(tex_sampler, vec2(uv.x, float(i)/float(hres))).rgb;
		float l = dot(x, x);
		col += vec3(intensity)*step(l, maxb*maxb)*step(minb*minb, l);
    }

	if ( ((o_texcoord.x >= 0.995 || o_texcoord.x <= 0.005 || o_texcoord.y >= 0.995 || o_texcoord.y <= 0.005)) )
	{ 
	    col = vec3(1.0, 1.0, 1.0);
	}

	fragColor = vec4(col, 0.8);
}
