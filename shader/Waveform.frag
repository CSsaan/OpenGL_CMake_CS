#version 430
precision mediump float;
in vec2 o_texcoord;

layout(binding = 0) uniform sampler2D tex_sampler;

uniform vec2 iResolution;

layout(location = 0) out vec4 fragColor;

float getY(vec2 coord)
{
    return dot(texture(tex_sampler, coord).rgb, vec3(0.2125, 0.7154, 0.0721));
}

vec4 waveform_graph()
{
	lowp vec2 uv = o_texcoord;
	int hres = 1080;
	float intensity = 0.028;
	const float thres = 1.0/iResolution.y;
	float factor = 1.0;
	float s = uv.y * 1.090 - 0.045;
	float maxb = s + thres;
	float minb = s - thres;
    float col = 0.0;
	lowp vec4 result;
	{
		for (int i = 0; i <= hres; i++)
		{
			float dy = float(i) / float(hres);
			float xc = getY(vec2(uv.x, 1.0 - dy));
			col += intensity * step(xc, maxb) * step(minb, xc);
			float l = xc * xc;
			col += intensity * step(l, maxb * maxb) * step(minb * minb, l);
		}
		col = col * factor;
		result = vec4(col, col, col, 1.0);
	}
	return result;
}

void main()
{       
    vec3 resultcolor = waveform_graph().rgb; 

	if ( ((o_texcoord.x >= 0.995 || o_texcoord.x <= 0.005 || o_texcoord.y >= 0.995 || o_texcoord.y <= 0.005)) )
    { 
        resultcolor = vec3(1.0, 1.0, 1.0);
    }

    fragColor = vec4(resultcolor, 1.0); 
}
