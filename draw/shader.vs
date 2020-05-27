#version 330 core
layout (location = 0) in vec3 aPos;
// layout (location = 1) in vec3 aColor;
layout (location = 1) in vec2 aTexCoord;

// out vec3 ourColor;
out vec2 TexCoord;

// uniform float pos;
uniform mat4 transform;

void main()
{
	// gl_Position = vec4(aPos, 1.0);
	// ourColor = aColor;
	// TexCoord = aTexCoord;
	gl_Position = transform * vec4(aPos, 1.0);
	TexCoord = vec2(aTexCoord.x, 1.0 - aTexCoord.y);
}