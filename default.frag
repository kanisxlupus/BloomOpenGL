#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BloomColor;

in vec3 crntPos;
in vec3 Normal;
in vec3 color;		// vertex colors from vert shader
in vec2 texCoord;	// texture coords from vert shader



// texture uniforms
uniform sampler2D diffuse0;
uniform sampler2D specular0;

// light and camera uniforms
uniform vec4 lightColor0;
uniform vec3 lightPos0;
uniform vec4 lightColor1;
uniform vec3 lightPos1;
uniform vec4 lightColor2;
uniform vec3 lightPos2;
uniform vec4 lightColor3;
uniform vec3 lightPos3;
uniform vec3 camPos;

vec4 pointLight(vec4 lightColor, vec3 lightPos)
{
	vec3 lightVec = lightPos - crntPos;
	float dist = length(lightVec);
	float a = 3.f;
	float b = .7f;
	float inten = 1.f / (a * dist * dist + b * dist * 1.f);		// lighting quadratic equation

	// ambient lighting
	float ambient = .2f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightVec);
	// take the max to avoid negative numbers
	float diffuse = max(dot(normal, lightDirection), 0.f);

	// specular lighting
	float specularLight = .5f; 
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.f), 16);
	float specular = specAmount * specularLight;

	return((texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor);
}

vec4 direcLight(vec4 lightColor, vec3 lightPos)
{
	// ambient lighting
	float ambient = .2f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(vec3(1.f, 1.f, 0.f));
	// take the max to avoid negative numbers
	float diffuse = max(dot(normal, lightDirection), 0.f);

	// specular lighting
	float specularLight = .5f; 
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.f), 16);
	float specular = specAmount * specularLight;

	return((texture(diffuse0, texCoord) * (diffuse + ambient) + texture(specular0, texCoord).r * specular) * lightColor);
}

vec4 spotLight(vec4 lightColor, vec3 lightPos)
{
	float outerCone = .9f;
	float innerCone = .95f;


	// ambient lighting
	float ambient = .2f;

	// diffuse lighting
	vec3 normal = normalize(Normal);
	vec3 lightDirection = normalize(lightPos - crntPos);
	// take the max to avoid negative numbers
	float diffuse = max(dot(normal, lightDirection), 0.f);

	// specular lighting
	float specularLight = .5f; 
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.f), 16);
	float specular = specAmount * specularLight;

	float angle = dot(vec3(0.f, -1.f, 0.f), -lightDirection);
	float inten = clamp((angle - outerCone) / (innerCone - outerCone), 0.f, 1.f);

	return((texture(diffuse0, texCoord) * (diffuse * inten + ambient) + texture(specular0, texCoord).r * specular * inten) * lightColor);
}

void main()
{
	FragColor = pointLight(lightColor0, lightPos0) / 10.f + pointLight(lightColor1, lightPos1) / 10.f + pointLight(lightColor2, lightPos2) / 10.f + pointLight(lightColor3, lightPos3) / 10.f;
	float brightness = dot(FragColor.rgb, vec3(0.2126f, 0.7152f, 0.0722f));
	if(brightness > 1.f)
	{
		BloomColor = vec4(FragColor.rgb, 1.f);
	}
	else
	{
		BloomColor = vec4(0.f, 0.f, 0.f, 1.f);
	}
}