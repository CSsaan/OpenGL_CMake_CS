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

    fragColor = vec4(resultcolor, 0.8); 
}




// #define HIGH_QUALITY
// #ifndef HIGH_QUALITY
// const int hres = 200;
// const float intensity = 0.04;
// const float thres = 0.004;
// #else
// const int hres = 1080;
// const float intensity = 0.04;
// const float thres = 1.0/iResolution.y;
// #endif

// void main()
// {
// 	vec2 uv = o_texcoord;
//     vec3 col = vec3(0.0);
//     float s = uv.y * 1.890 - 0.045;
//     float maxb = s+thres;
//     float minb = s-thres;
    
//     for (int i = 0; i <= hres; i++) {
//         vec3 x = texture(tex_sampler, vec2(uv.x, float(i)/float(hres))).rgb;
// 		col += vec3(intensity)*step(x, vec3(maxb))*step(vec3(minb), x);

// 		float l = dot(x, x);
// 		col += vec3(intensity)*step(l, maxb*maxb)*step(minb*minb, l);
//     }

// 	if ( ((o_texcoord.x >= 0.995 || o_texcoord.x <= 0.005 || o_texcoord.y >= 0.995 || o_texcoord.y <= 0.005)) )
// 	{ 
// 	    col = vec3(1.0, 1.0, 1.0);
// 	}

// 	fragColor = vec4(col, 0.8);
// }






// void main()
// {
// 	vec2 fragCoord = vec2(o_texcoord.x * iResolution.x, o_texcoord.y * iResolution.y);
// 	vec2 iChannelResolution = vec2(1080, 1920);
//     float u = fragCoord.x / iResolution.x * 3.0;
//     vec2 coords;
//     float accum = 1.0 / iResolution.y;
//     accum *= 20.0;

//     vec4 col = vec4(0., 0., 0., 1.);
//     float ys = (fragCoord.xy[1] - 1.0)/ iResolution.y;
//     float ye = (fragCoord.xy[1]) / iResolution.y;

//     for (int y = 1; y < int(iChannelResolution.y); ++y){
//         coords.x = fragCoord.x / (iResolution.x / 3.0);
//         coords.y = float(y) / iChannelResolution.y;
//         vec4 texcol;
//         if (u <= 1.0){
//             texcol = texture(tex_sampler, coords);
//             if (texcol.x > ys && texcol.x < ye){
//                 col += vec4(accum, 0., 0., 0.);
//             }
//         }

//         if (u > 1.0 && u <= 2.0){
//             coords.x -= 1.;
//             texcol = texture(tex_sampler, coords);
//             if (texcol.y > ys && texcol.y < ye){
//                 col += vec4(0., accum, 0., 0.);
//             }
//         }
//         if (u > 2.0){
//             coords.x -= 2.;
//             texcol = texture(tex_sampler, coords);
//             if (texcol.z > ys && texcol.z < ye){
//                 col += vec4(0., 0., accum, 0.);
//             }
//         }
//     }

// 	if ( ((o_texcoord.x >= 0.995 || o_texcoord.x <= 0.005 || o_texcoord.y >= 0.995 || o_texcoord.y <= 0.005)) )
// 	{ 
// 	    col.rgb = vec3(1.0, 1.0, 1.0);
// 	}

//     fragColor = vec4(col.rgb, 0.8);
// }
