#version 430
precision mediump float;
in vec2 o_texcoord;

layout(location = 0) out vec4 fragColor;

uniform ivec2 useRGB_choseColor; // 1:RGB_colors  other:One_color ;  // [if One_color] r:1 g:2 b:3 y:4
uniform ivec4 maxRGB;
uniform int pointsR[256];
uniform int pointsG[256];
uniform int pointsB[256];

float alphabscope = 0.8;


int getOneMax(int color)
{
    int Max = color == 1 ? maxRGB.x : color == 2 ? maxRGB.y : color == 3 ? maxRGB.z : maxRGB.w;
    return Max;
}

int getTotalMax()
{
    int Max = max(max(maxRGB.x, maxRGB.y), maxRGB.z);
    return Max;
}

float getOneColorResult(int color, ivec2 uv)
{   
    float resultColor;
    if(color == 1)
    {
        resultColor = float(uv.y < pointsR[uv.x]);
    }
    else if(color == 2)
    {
        resultColor = float(uv.y < pointsG[uv.x]);
    }
    else if(color == 3)
    {
        resultColor = float(uv.y < pointsB[uv.x]);
    }
    else
    {
        resultColor = float(uv.y < (pointsR[uv.x]+pointsG[uv.x]+pointsB[uv.x])/3.0);
    }
    return resultColor;
}

void main()
{       
    vec4 result;
    if(useRGB_choseColor.x == 0)
    {
        int Max = getOneMax(useRGB_choseColor.y); // r:1 g:2 b:3 y:4
        ivec2 UV = ivec2(o_texcoord * vec2(255, Max));
        float resultColor = getOneColorResult(useRGB_choseColor.y, UV);
        vec3 showResult = useRGB_choseColor.y==1?vec3(resultColor,0.0,0.0) : useRGB_choseColor.y==2?vec3(0.0,resultColor,0.0) : useRGB_choseColor.y==3?vec3(0.0,0.0,resultColor) : vec3(resultColor);
        result = vec4(showResult, alphabscope);
    }
    else
    {
        int MaxRGBY = getTotalMax();
        ivec2 UV = ivec2(o_texcoord * vec2(255, MaxRGBY));
        float resultR = float(UV.y < pointsR[UV.x]);
        float resultG = float(UV.y < pointsG[UV.x]);
        float resultB = float(UV.y < pointsB[UV.x]);
        float resultY = float(UV.y < (pointsR[UV.x]+pointsG[UV.x]+pointsB[UV.x])/3.0);
        vec3 mix_color = mix(mix(mix(vec3(0.0), vec3(0.0, 0.0, resultB), resultB*1.0),vec3(0.0, resultG, 0.0), resultG*0.5),vec3(resultR, 0.0, 0.0), resultR*0.6);
        result = vec4(vec3(mix_color), alphabscope);
    }

    fragColor = result;

}
