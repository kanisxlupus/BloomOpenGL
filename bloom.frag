#version 330 core
out vec4 FragColor;

in vec2 texCoords;

uniform sampler2D screenTexture;
uniform bool horizontal;

const int radius = 6;
float spreadBlur = 2.f;
float weights[radius];

void main()
{
	// calculate the weights with the Gaussian equation
	float x = 0.f;
	for (int i = 0; i < radius; i++)
	{
		if (spreadBlur <= 2.f)
		{
			x += 3.f / radius;
		}
		else
		{
			x += 6.f / radius;
		}

		weights[i] = exp(-.5f * pow(x / spreadBlur, 2.f)) / (spreadBlur * sqrt(2 * 3.14159265f));
	}
	


	vec2 tex_offset = 1.f / textureSize(screenTexture, 0);
	vec3 result = texture(screenTexture, texCoords).rgb * weights[0];

	if (horizontal)
	{
		for (int i = 1; i < radius; i++)
		{
			result += texture(screenTexture, texCoords + vec2(tex_offset.x * i, 0.f)).rgb * weights[i];
			result += texture(screenTexture, texCoords - vec2(tex_offset.x * i, 0.f)).rgb * weights[i]; 
		}
	}
	else
	{
		for (int i = 1; i < radius; i++)
		{
			result += texture(screenTexture, texCoords + vec2(0.f, tex_offset.y * i)).rgb * weights[i];
			result += texture(screenTexture, texCoords - vec2(0.f, tex_offset.y * i)).rgb * weights[i];
		}
	}

	FragColor = vec4(result, 1.f);
}
