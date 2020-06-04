#version 330 core

struct Material {
	// 材质纹理
	sampler2D diffuse;
	sampler2D specular;
	float shininess;	
};

struct Light {
//	vec3 position;	// 单一光源
//	vec3 direction; // 定向光

	// 材质
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// 光线衰减
	float constant;	
	float linear;
	float quadratic;

	// 聚光
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;
uniform Light light;

void main()
{
	// 光线衰减距离
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// 环境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// 漫反射
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);	// 单一光源
//	vec3 lightDir = normalize(-light.direction);			// 定向光
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));

	// 镜面光
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess * 128.0);
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// 手电筒+边缘软化
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff)/epsilon, 0.0, 1.0);

	ambient *= intensity;
	diffuse *= intensity;

	vec3 result = ambient + diffuse + specular;
	FragColor = vec4(result, 1.0f);


}