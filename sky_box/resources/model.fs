#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 viewPos;
uniform samplerCube skybox;

void main()
{
	//  Ù–‘
//	vec3 norm = normalize(Normal);
//	vec3 viewDir = normalize(viewPos - FragPos);
//	vec3 result = CalcPointLight(pointLight, norm, FragPos, viewDir);
//	FragColor = vec4(result, 1.0);

//	FragColor = texture(texture_diffuse1, TexCoords);

	vec3 I = normalize(FragPos - viewPos);
	vec3 R = reflect(I, normalize(Normal));
	FragColor = vec4(texture(skybox, R).rgb, 1.0);
}