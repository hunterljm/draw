#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

uniform sampler2D texture1;



//void main()
//{
	//vec4 textColor = texture(texture1, TexCoords);
	//FragColor = textColor;

	// 反相
	//FragColor = vec4(vec3(1.0-texture(texture1, TexCoords)), 1.0);
		
	// 灰度
//	FragColor = texture(texture1, TexCoords);
//	float average = (FragColor.r + FragColor.g + FragColor.b) / 3.0;
//	FragColor = vec4(average, average, average, 1.0);
//}

// 核
const float offset = 1.0 / 300.0;  

void main()
{
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // 左上
        vec2( 0.0f,    offset), // 正上
        vec2( offset,  offset), // 右上
        vec2(-offset,  0.0f),   // 左
        vec2( 0.0f,    0.0f),   // 中
        vec2( offset,  0.0f),   // 右
        vec2(-offset, -offset), // 左下
        vec2( 0.0f,   -offset), // 正下
        vec2( offset, -offset)  // 右下
    );

    float kernel[9] = float[](
        -1, -1, -1,
        -1,  8, -1,
        -1, -1, -1
		
		// 模糊
		//1.0 / 16, 2.0 / 16, 1.0 / 16,
		//2.0 / 16, 4.0 / 16, 2.0 / 16,
		//1.0 / 16, 2.0 / 16, 1.0 / 16  
    );

    vec3 sampleTex[9];
    for(int i = 0; i < 9; i++)
    {
        sampleTex[i] = vec3(texture(texture1, TexCoords.st + offsets[i]));
    }
    vec3 col = vec3(0.0);
    for(int i = 0; i < 9; i++)
        col += sampleTex[i] * kernel[i];

    FragColor = vec4(col, 1.0);
}