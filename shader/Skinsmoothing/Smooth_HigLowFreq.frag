/*
高低频磨皮：
    是指将图像划分为高频信息和低频信息分开处理，低频磨皮，高频保留细节，进而达到细节保留的磨皮效果的方法。
划分高低频：
    低频：高斯滤波之后即为低频图； 高频：高频图=原图-低频图。
    有了高频信息和低频信息，我们使用如下公式将其还原：原图=高频+低频-255。
步骤：
    1，低频图层与高频图层做线性光图层50%透明度的混合；
    2，如果透明度超过50%，那么，即会出现细节增强效果，反之，细节弱化。
    3，对G_HP和G_HP自身做“叠加”图层混合，执行3-5次，最后一次执行“线性光”图层混合，得到痘痘区域图层alpha；
    4，对痘印区域图进行曲线调节-提亮处理，亮度调节到痘印消失即可；
    5，最后，根据AI皮肤检测的mask，只处理的皮肤部分。
*/

#version 430
precision highp float;
in vec2 o_texcoord;

layout(binding = 0) uniform sampler2D s_texture;
layout(binding = 1) uniform sampler2D s_textureBlur;
layout(binding = 2) uniform sampler2D s_textureMask;
// layout(binding = 3) uniform sampler2D s_texLutSpline1D;

uniform float intensity;

layout(location = 0) out vec4 fragColor;

void main()
{
    float faceMASK = texture(s_textureMask, o_texcoord).r;
    vec3 inColor = texture(s_texture, o_texcoord).rgb;
    vec3 lowFrequency = texture(s_textureBlur, o_texcoord).rgb;
    vec3 highFrequency = inColor - lowFrequency + 0.5;

    // lowFrequency = mix(inColor, lowFrequency, faceMASK);

    vec3 result = mix(inColor, lowFrequency, highFrequency);

    // merge with alpha
    vec3 res = mix(inColor.rgb, result, faceMASK * abs(sin(intensity)));
    
    fragColor  = vec4(vec3(res), 1.0);
}
