#version 330 core
out vec4 FragColor;

in vec3 ourColor;
//in vec2 TexCoord;

// uniform sampler2D texture1;
// uniform sampler2D texture2;

void main()
{
	// FragColor = texture(ourTextrue, TexCoord) * vec4(ourColor, 1.0);
	// FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), see);
	// FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(1 - TexCoord.x, TexCoord.y)), 0.2);
	FragColor = vec4(ourColor, 1.0f);
}