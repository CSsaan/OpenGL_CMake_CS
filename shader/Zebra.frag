#version 430
precision mediump float;
in vec2 o_texcoord;

layout(binding = 0) uniform sampler2D tex_sampler;

uniform float IGlobalTime;
uniform vec2 iResolution;

layout(location = 0) out vec4 fragColor;

#define STRIPES 80.0
#define PW STRIPES / iResolution.y
#define PI 3.141592

mat2 rotate2d(float angle)
{
    return mat2 (cos(angle), -sin(angle), sin(angle), cos(angle)) ;
}
float zebra(in vec2 uv)
{
    float shift = mod(IGlobalTime * 6.0, 12.0) * 0.2 * 0.03;
    uv.y-= shift;
    uv*=rotate2d(PI/4.0);
    uv = fract(uv* STRIPES);
    // 清晰边缘： return ((1.-step(0.2-PW, uv.y)) + step(0.6-PW, uv.y)); // 平滑边缘： return ((1.-step(0.2-PW, 0.2+PW, uv.y)) + step(0.6-PW, 0.6+PW, uv.y));
    return ((1.-smoothstep(0.2-PW, 0.2+PW, uv.y)) + smoothstep(0.6-PW, 0.6+PW, uv.y));
}

void main()
{   
    vec2 uv = o_texcoord;
    vec4 px = texture(tex_sampler, o_texcoord);
    vec4 zcolor;
    vec4 result = px;
    
    zcolor = vec4(zebra(uv));
    
    if(px.g > 0.4  && px.g < 0.5)
    {
        result = zcolor.r>0.5?(mix(result,zcolor,0.6)):(mix(result,zcolor,0.8));
    }

    fragColor = result;
}
