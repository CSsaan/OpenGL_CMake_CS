#version 430
precision mediump float;
in vec2 o_texcoord;

layout(binding = 0) uniform sampler2D tex_sampler;

uniform vec2 iResolution;

layout(location = 0) out vec4 fragColor;


void main()
{
	vec2 fragCoord = vec2(o_texcoord.x * iResolution.x, o_texcoord.y * iResolution.y);
	vec2 iChannelResolution = vec2(1080, 1920);
    float u = fragCoord.x / iResolution.x * 3.0;
    vec2 coords;
    float accum = 1.0 / iResolution.y;
    accum *= 20.0;

    vec4 col = vec4(0., 0., 0., 1.);
    float ys = (fragCoord.xy[1] - 1.0)/ iResolution.y;
    float ye = (fragCoord.xy[1]) / iResolution.y;

    for (int y = 1; y < int(iChannelResolution.y); ++y){
        coords.x = fragCoord.x / (iResolution.x / 3.0);
        coords.y = float(y) / iChannelResolution.y;
        vec4 texcol;
        if (u <= 1.0){
            texcol = texture(tex_sampler, coords);
            if (texcol.x > ys && texcol.x < ye){
                col += vec4(accum, 0., 0., 0.);
            }
        }

        if (u > 1.0 && u <= 2.0){
            coords.x -= 1.;
            texcol = texture(tex_sampler, coords);
            if (texcol.y > ys && texcol.y < ye){
                col += vec4(0., accum, 0., 0.);
            }
        }
        if (u > 2.0){
            coords.x -= 2.;
            texcol = texture(tex_sampler, coords);
            if (texcol.z > ys && texcol.z < ye){
                col += vec4(0., 0., accum, 0.);
            }
        }
    }

	if ( ((o_texcoord.x >= 0.995 || o_texcoord.x <= 0.005 || o_texcoord.y >= 0.995 || o_texcoord.y <= 0.005)) )
	{ 
	    col.rgb = vec3(1.0, 1.0, 1.0);
	}

    fragColor = vec4(col.rgb, 0.8);
}
