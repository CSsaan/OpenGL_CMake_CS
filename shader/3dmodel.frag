#version 430

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

struct Material {
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
}; 

uniform vec3 viewPos;
uniform Material material;

layout(location = 0) out vec4 FragColor;

void main()
{   
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	
	// vec3 finalColor = texture(tex_sampler, o_texcoord).xyz;
	float diff = max(0.0, dot(norm, viewDir));
	
	// fragColor = vec4(finalColor, 1.0);
	vec3 ambient = vec3(texture(material.diffuse, TexCoords));
    vec3 diffuse = diff * vec3(texture(material.diffuse, TexCoords));
    vec3 specular = vec3(texture(material.specular, TexCoords));
	FragColor = vec4(vec3((ambient+diffuse+specular)), 1.0);
}