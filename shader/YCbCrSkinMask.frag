#version 430
precision highp float;
in vec2 o_texcoord;

layout(binding = 0) uniform sampler2D tex_sampler;

layout(location = 0) out vec4 fragColor;

vec3 rgb2yuv_bt709(vec3 c){
   float y = 0.062745 + 0.183000 * c.x + 0.614000 * c.y + 0.062000 * c.z;
   float u = 0.501961 - 0.100644 * c.x - 0.338572 * c.y + 0.439216 * c.z;
   float v = 0.501961 + 0.439216 * c.x - 0.398942 * c.y - 0.040274 * c.z;
   return vec3(y,u,v);
}
vec3 RGBtoHSV(vec3 c)
{
    vec4 K = vec4(0.0f, -1.0f / 3.0f, 2.0f / 3.0f, -1.0f);
    vec4 p = mix(vec4(c.zy, K.wz), vec4(c.yz, K.xy), step(c.z, c.y));
    vec4 q = mix(vec4(p.xyw, c.x), vec4(c.x, p.yzx), step(p.x, c.x));
    float d = q.x - min(q.w, q.y);
    float e = 1.0e-5f;
    return vec3(abs(q.z + (q.w - q.y) / (6.0f * d + e)), d / (q.x + e), q.x);
}

void main()
{
    vec4 color = texture(tex_sampler, o_texcoord);
    vec3 yuv   = rgb2yuv_bt709(color.xyz);
    vec3 hsv   = RGBtoHSV(color.xyz);
    float cb   = yuv.y;
    float cr   = yuv.z;
    float mask  = 0.0;
    // 肤色颜色范围
    bool skin_CbCr = (cr > 133.0/255.0 && cr < 173.0/255.0) &&  (cb > 77.0/255.0 && cb  < 127.0/255.0);
    if (skin_CbCr){
        mask = 1.0;
    }
    fragColor = vec4(mask);
}
