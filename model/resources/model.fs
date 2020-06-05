#version 330 core

struct Material {
	// 材质纹理
//	sampler2D diffuse;
	sampler2D specular;
	float shininess;	
};

// 点光源
struct PointLight
{
	// 光位置
	vec3 position;

	// 衰减
	float constant;
	float linear;
	float quadratic;

	// 材质 
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform sampler2D texture_diffuse1;
uniform vec3 viewPos;
uniform Material material;
uniform PointLight pointLight;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	// 漫反射着色
	float diff = max(dot(normal, lightDir), 0.0);

	// 镜面光反色
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// 衰减
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// 合并结果
//	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
//	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 ambient = light.ambient * vec3(texture(texture_diffuse1, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(texture_diffuse1, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

void main()
{
	// 属性
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = CalcPointLight(pointLight, norm, FragPos, viewDir);
	FragColor = vec4(result, 1.0);

//	FragColor = texture(texture_diffuse1, TexCoords);
}