#version 330 core
#define NR_POINT_LIGHT 4
struct Material {
	// 材质纹理
	sampler2D diffuse;
	sampler2D specular;
	float shininess;	
};

// 定向光
struct DirLight
{
	// 光方向
	vec3 direction;

	// 材质
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
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

// 聚光光源 
struct SportLight
{
	// 光源位置、方向
	vec3 position;
	vec3 direction;

	// 衰减
	float constant;
	float linear;
	float quadratic;

	// 材质
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// 聚光参数
	float cutOff;
	float outerCutOff;
};

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 viewPos;
uniform Material material;

uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHT];
uniform SportLight sportLight;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);

	// 漫反射着色
	float diff = max(dot(normal, lightDir), 0.0);

	// 镜面光反色
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

	// 合并结果
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	return (ambient + diffuse + specular);
}

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
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
	vec3 specular = light.specular * spec * vec3(texture(material.specular, TexCoords));

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return (ambient + diffuse + specular);
}

vec3 CalcSportLight(SportLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

	// 漫反射着色
	float diff = max(dot(normal, lightDir), 0.0);

	// 镜面光反色
	// 冯氏光照
	// vec3 reflectDir = reflect(-lightDir, normal);
	// float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	// Blinn-Phong 光照
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(normal, halfwayDir), 0.0), material.shininess);

	// 衰减
	float distance = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	// 合并结果
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));
	vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, TexCoords));
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

	return (ambient + diffuse + specular);
}


void main()
{
	// 属性
	vec3 norm = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPos);

	// 第一阶段：定向光照
	vec3 result = CalcDirLight(dirLight, norm, viewDir);
	// vec3 result = vec3(0.0f, 0.0f, 0.0f);

	// 第二阶段：点光源
	for (int i=0; i<NR_POINT_LIGHT; i++) {
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);
	}

	// 第三阶段：聚光
	result += CalcSportLight(sportLight, norm, FragPos, viewDir);

	FragColor = vec4(result, 1.0);
}