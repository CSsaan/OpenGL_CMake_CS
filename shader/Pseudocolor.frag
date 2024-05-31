#version 430
precision mediump float;
in vec2 o_texcoord;

layout(binding = 0) uniform sampler2D tex_sampler;
layout(binding = 1) uniform sampler2D s_LutTexture;

layout(location = 0) out vec4 fragColor;


void main()
{   
	// /*  法一：LUT伪彩色(8*8)  */ 
    lowp vec4 textureColor = texture(tex_sampler, o_texcoord);
    mediump float blueColor = textureColor.b * 63.0;
    mediump vec2 quad1;
    quad1.y = floor(floor(blueColor) / 8.0);
    quad1.x = floor(blueColor) - (quad1.y * 8.0);
    mediump vec2 quad2;
    quad2.y = floor(ceil(blueColor) / 7.9999);
    quad2.x = ceil(blueColor) - (quad2.y * 8.0);
    highp vec2 texPos1;
    texPos1.x = (quad1.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);
    texPos1.y = (quad1.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);
    highp vec2 texPos2;
    texPos2.x = (quad2.x * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.r);
    texPos2.y = (quad2.y * 0.125) + 0.5/512.0 + ((0.125 - 1.0/512.0) * textureColor.g);
    lowp vec4 newColor1 = texture(s_LutTexture, texPos1);
    lowp vec4 newColor2 = texture(s_LutTexture, texPos2);
    lowp vec4 newColor = mix(newColor1, newColor2, fract(blueColor)); //最终LUT结果
    fragColor = vec4(newColor.rgb, textureColor.a);

    // /*  法一：LUT伪彩色(4*4)  */
    // lowp vec4 textureColor = texture(tex_sampler, o_texcoord);
    // mediump float blueColor = textureColor.b * 15.0;
    // mediump vec2 quad1;
    // quad1.y = floor(blueColor / 4.0);
    // quad1.x = floor(blueColor) - (quad1.y * 4.0);
    // mediump vec2 quad2;
    // quad2.y = floor(ceil(blueColor) / 4.0);
    // quad2.x = ceil(blueColor) - (quad2.y * 4.0);
    // highp vec2 texPos1;
    // texPos1.x = (quad1.x * 0.25) + 0.5/64.0 + ((0.25 - 1.0/64.0) * textureColor.r);
    // texPos1.y = (quad1.y * 0.25) + 0.5/64.0 + ((0.25 - 1.0/64.0) * textureColor.g);
    // highp vec2 texPos2;
    // texPos2.x = (quad2.x * 0.25) + 0.5/64.0 + ((0.25 - 1.0/64.0) * textureColor.r);
    // texPos2.y = (quad2.y * 0.25) + 0.5/64.0 + ((0.25 - 1.0/64.0) * textureColor.g);
    // lowp vec4 newColor1 = texture(s_LutTexture, texPos1);
    // lowp vec4 newColor2 = texture(s_LutTexture, texPos2);
    // lowp vec4 newColor = mix(newColor1, newColor2, fract(blueColor));
    // fragColor = mix(textureColor, vec4(newColor.rgb, textureColor.w), 1.0);


    // /*  法二：渐变伪彩色  */
    // float luminance = textureColor.r*0.299 + textureColor.g*0.587 + textureColor.b*0.114;
    // fragColor = vec4(luminance,abs(abs(luminance+luminance-1.0)-1.0),1.0-luminance, textureColor.a);

    // /*  法三：固定阈值伪彩色  */
    // lowp vec4 textureColor = texture(tex_sampler, o_texcoord);
    // vec3 resultcolor;
    // float num = 1.0 / 255.0 * (219.0 / 255.0);
    // float gray = dot(textureColor.rgb, vec3(0.2125, 0.7154, 0.0721));
    // if (gray < 8.0 * num)
    //     resultcolor = vec3(123.0, 106.0, 176.0) / 255.0;
    // else if ((gray >= 8.0 * num) && (gray <= 10.0 * num))
    //     resultcolor = vec3(2.0, 117.0, 190.0) / 255.0;
    // else if ((gray >= 97.0 * num) && (gray <= 108.0 * num))
    //     resultcolor = vec3(129.0, 194.0, 64.0) / 255.0;
    // else if ((gray >= 133.0 * num) && (gray <= 143.0 * num))
    //     resultcolor = vec3(244.0, 137.0, 183.0) / 255.0;
    // else if ((gray >= 248.0 * num) && (gray < 253.0 * num))
    //     resultcolor = vec3(254.0, 242.0, 60.0) / 255.0;
    // else if ((gray >= 253.0 * num) && (gray <= 255.0 * num))
    //     resultcolor = vec3(238.0, 47.0, 45.0) / 255.0;
    // else
    //     resultcolor = vec3(gray);
    // fragColor = vec4(resultcolor, textureColor.a);
}