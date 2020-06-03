#version 330 core

in vec3 Normal;
in vec3 FragPos;
out vec4 FragColor;

// uniform vec3 objectColor;
// uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

struct Material {
//	vec3 ambient;
//	vec3 diffuse;
	sampler2D diffuse;
//	vec3 specular;
	sampler2D specular;
	float shininess;	
};

uniform Material material;

in vec2 TexCoords;

struct Light {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform Light light;

void main()
{
	// 环境光
	// vec3 ambient = material.ambient * light.ambient;
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// 漫反射
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
//	vec3 diffuse = (diff * material.diffuse) * light.diffuse;
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// 镜面光
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128.0);
//	vec3 specular = light.specular * spec * material.specular;
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	// vec3 result = (ambient + diffuse + specular) * objectColor;
	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0f);
}