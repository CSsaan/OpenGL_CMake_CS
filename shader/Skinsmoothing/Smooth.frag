/*
通道磨皮：method=0
    是指从原图的RGB三通道中，选取一个特征明显的通道，作为痘印的选区通道（这个通道一般是G通道或者B通道，
    在PS中根据具体用户图来选择），根据选取通道确定痘印区域，然后对该区域对应的原图进行提亮处理，进而将
    痘印颜色减淡至消失。
通道选择：
    对于痘痘区域，痘痘斑点一般多为深色区域，因此，相对而言G和B通道对比要更加明显。
步骤：
    1，选择原图的G、B通道，构建灰度图Gray；
    2，对灰度图Gray做高反差保留，得到G_HP；
    3，对G_HP和G_HP自身做“叠加”图层混合，执行3-5次，最后一次执行“线性光”图层混合，得到痘痘区域图层alpha；
    4，对痘印区域图进行曲线调节-提亮处理，亮度调节到痘印消失即可；
    5，最后，根据AI皮肤检测的mask，只处理的皮肤部分。
*/

/*
新磨皮：method=1
步骤：
    1，选择原图、均值模糊图，做差方得到高频图；
    2，选择原图的通道、均值模糊的R通道，在模糊图基础上,保留黑色毛发边缘；
    3，融合高频图与第2步结果，得到磨皮强度alpha图；
    4，根据alpha强度图，来融合原图、均值模糊图，得到磨皮结果图；
    5，最后，最第4步磨皮结果做锐化处理，突出边缘细节。
*/

#version 430
precision highp float;
in vec2 o_texcoord;

layout(binding = 0) uniform sampler2D s_texture;
layout(binding = 1) uniform sampler2D s_textureBlur;
layout(binding = 2) uniform sampler2D s_textureMask;
// layout(binding = 3) uniform sampler2D s_texLutSpline1D;

uniform float intensity;
uniform vec2 windowSize;
uniform float blurCoefficient;
uniform float sharpenStrength;
uniform int method;

layout(location = 0) out vec4 fragColor;

const float s_texLutSpline1D[256] = float[](
    0.0, 2.0, 4.0, 5.0, 7.0, 9.0, 11.0, 12.0, 14.0, 16.0, 18.0, 20.0, 21.0, 23.0, 25.0, 27.0, 29.0, 30.0, 32.0, 34.0, 36.0, 37.0, 39.0, 41.0, 43.0, 45.0, 46.0, 48.0, 50.0, 52.0, 53.0, 55.0, 57.0, 58.0, 60.0, 62.0, 64.0, 65.0, 67.0, 69.0, 71.0, 72.0, 74.0, 76.0, 77.0, 79.0, 81.0, 82.0, 84.0, 85.0, 87.0, 89.0, 91.0, 92.0, 94.0, 95.0, 97.0, 99.0, 101.0, 101.0, 103.0, 105.0, 107.0, 108.0, 110.0,
    111.0, 113.0, 114.0, 116.0, 117.0, 119.0, 120.0, 122.0, 123.0, 125.0, 126.0, 128.0, 129.0, 131.0, 132.0, 134.0, 135.0, 136.0, 138.0, 139.0, 141.0, 142.0, 143.0, 145.0, 146.0, 147.0, 149.0, 150.0, 151.0, 153.0, 154.0, 155.0, 156.0, 158.0, 160.0, 160.0, 161.0, 162.0, 164.0, 165.0, 166.0, 167.0, 168.0, 169.0, 170.0, 172.0, 173.0, 174.0, 175.0, 176.0, 177.0, 178.0, 179.0, 180.0, 181.0, 182.0, 183.0, 184.0, 185.0, 186.0, 187.0, 188.0, 189.0,
    190.0, 191.0, 191.0, 192.0, 193.0, 194.0, 195.0, 196.0, 197.0, 198.0, 199.0, 199.0, 200.0, 201.0, 202.0, 202.0, 203.0, 204.0, 205.0, 205.0, 206.0, 207.0, 208.0, 208.0, 209.0, 210.0, 211.0, 211.0, 212.0, 213.0, 214.0, 214.0, 215.0, 215.0, 216.0, 216.0, 217.0, 218.0, 218.0, 219.0, 220.0, 220.0, 221.0, 222.0, 222.0, 222.0, 223.0, 224.0, 224.0, 225.0, 225.0, 226.0, 226.0, 227.0, 227.0, 228.0, 228.0, 229.0, 229.0, 230.0, 230.0, 231.0, 231.0, 232.0,
    232.0, 233.0, 233.0, 234.0, 234.0, 234.0, 235.0, 235.0, 236.0, 236.0, 237.0, 237.0, 237.0, 238.0, 238.0, 239.0, 239.0, 239.0, 240.0, 241.0, 241.0, 241.0, 241.0, 242.0, 242.0, 242.0, 243.0, 244.0, 244.0, 244.0, 244.0, 245.0, 245.0, 246.0, 246.0, 246.0, 246.0, 247.0, 247.0, 247.0, 248.0, 248.0, 248.0, 249.0, 249.0, 249.0, 250.0, 250.0, 250.0, 251.0, 251.0, 251.0, 252.0, 252.0, 252.0, 252.0, 253.0, 253.0, 253.0, 254.0, 254.0, 254.0, 255.0, 255.0
);

float pre_handle_of_chl(vec3 color)
{
    vec3 aColor = color * 0.5; // ExposureAdjust
    return (aColor.b * aColor.g * 2.0); // GreenBlueChannelOverlayBlend
}
vec4 face_lighten(vec4 color)
{
    float red = s_texLutSpline1D[int(color.r*255.0)]; // texture(s_texLutSpline1D, vec2(color.r, 0.0)).r;
    float gre = s_texLutSpline1D[int(color.g*255.0)]; // texture(s_texLutSpline1D, vec2(color.g, 0.0)).r;
    float blu = s_texLutSpline1D[int(color.b*255.0)]; // texture(s_texLutSpline1D, vec2(color.b, 0.0)).r;
    return vec4(vec3(red, gre, blu)/255.0, 1.0);
}

float computeBlendRatio(float inColor, float blurColor) {
    float minColor = min(inColor, blurColor - 0.1);
    float adjustedColor = minColor - 0.2;
    return clamp(adjustedColor * 4.0, 0.0, 1.0);
}

vec3 calculateSharpen(vec3 inColor, float flag) {
	vec3 lowerBound = max(vec3(0.0), inColor);
	vec3 upperBound = min(vec3(1.0), inColor);
	return mix(lowerBound, upperBound, flag);
}

void main()
{
    float faceMASK = texture(s_textureMask, o_texcoord).r;
    vec4 inColor = texture(s_texture, o_texcoord);
    lowp vec4 blurColor = texture(s_textureBlur, o_texcoord);

    vec3 res;
    if(method == 0) 
    {
       float currC = pre_handle_of_chl(inColor.rgb);
        float blurC = pre_handle_of_chl(blurColor.rgb);
        
        float hightPassColor = currC - blurC + 0.5;
        for (int i = 0; i < 3; i++) {
            if (hightPassColor < 0.5) {
                hightPassColor = hightPassColor * hightPassColor * 2.0;
            }
            else {
                hightPassColor = 1.0 - (1.0-hightPassColor) * (1.0-hightPassColor) * 2.0;
            }
        }
        float alpha = 1.0 - hightPassColor;
        // alpha = clamp(4.0*pow(alpha-0.2, 2.0), 0.0, 0.5) *smoothstep(0.0, 1.0, smoothstep(0.0, 1.0, alpha-0.2)); // 抑制背景提亮
        
        alpha = clamp(alpha, 0.0, 1.0);
        // lighten 
        vec4 lightenColor = face_lighten(inColor);
        lightenColor = mix(inColor, lightenColor, alpha);
        // merge with alpha
        res = mix(inColor.rgb, lightenColor.rgb, faceMASK * abs(sin(intensity))); 
    }
    else if(method == 1)
    {
        // varianceColor
        highp vec3 squaredDiff = (inColor.rgb - blurColor.rgb) * 7.07;
        squaredDiff = min(squaredDiff * squaredDiff, 1.0);
        lowp vec4 varianceColor = vec4(squaredDiff, 1.0);
        varianceColor = (inColor - blurColor) * (inColor - blurColor); // 高频图

        const float threshold = 0.1;
        mediump float blendRatio = computeBlendRatio(inColor.r, blurColor.r); // R通道. 在模糊图基础上,保留黑色毛发边缘
        mediump float meanVariance = dot(varianceColor.rgb, vec3(0.33333));
        mediump float minCoefficient = (1.0 - meanVariance / (meanVariance + threshold)) * blendRatio * blurCoefficient; // 磨皮强度
        lowp vec3 finalColor = mix(inColor.rgb, blurColor.rgb, minCoefficient); 

        // finalColor's sharpen
        mediump float sum_gaussian = 0.25 * inColor.g;
        vec2 texOffset = vec2(0.5 / windowSize.x, 0.5 / windowSize.y);
        sum_gaussian += 0.125 *texture(s_texture, o_texcoord - vec2(texOffset.x, 0.0)).g;
        sum_gaussian += 0.125 *texture(s_texture, o_texcoord + vec2(texOffset.x, 0.0)).g;
        sum_gaussian += 0.125 *texture(s_texture, o_texcoord - vec2(0.0, texOffset.y)).g;
        sum_gaussian += 0.125 *texture(s_texture, o_texcoord + vec2(0.0, texOffset.y)).g;
        sum_gaussian += 0.0625*texture(s_texture, o_texcoord + vec2(texOffset.x, texOffset.y)).g;
        sum_gaussian += 0.0625*texture(s_texture, o_texcoord - vec2(texOffset.x, texOffset.y)).g;
        sum_gaussian += 0.0625*texture(s_texture, o_texcoord + vec2(-texOffset.x, texOffset.y)).g;
        sum_gaussian += 0.0625*texture(s_texture, o_texcoord + vec2(texOffset.x, -texOffset.y)).g;

        float highPassValue = inColor.g - sum_gaussian + 0.5;
        float thresholdFlag = step(0.5, highPassValue);
        vec3 sharpenColor = calculateSharpen((2.0 * highPassValue + finalColor - 1.0), thresholdFlag);
        vec3 result = mix(finalColor.rgb, sharpenColor.rgb, sharpenStrength);

        res = mix(inColor.rgb, result, faceMASK * intensity);
    }

    fragColor  = vec4(vec3(res).rgb, inColor.a);
}
