#version 330 core

// positions/coords for vertices
layout (location = 0) in vec3 aPos;

// normal coordinates
layout (location = 1) in vec3 aNormal;

// colors for vertices
layout (location = 2) in vec3 aColor;

// texture coordinates
layout (location = 3) in vec2 aTex;


out vec3 crntPos;	// current position
out vec3 Normal;	// normals
out vec3 color;		// outputs color to frag shader
out vec2 texCoord;	// outputs texture to frag shader

uniform mat4 camMatrix;
uniform mat4 model;
uniform mat4 translation;
uniform mat4 rotation;
uniform mat4 scale;

void main()
{
	crntPos = vec3(model * translation * -rotation * scale * vec4(aPos, 1.f));
	
	Normal = aNormal;
	color = aColor;
	texCoord = mat2(0.f, -1.f, 1.f, 0.f) * aTex;
	gl_Position = camMatrix * vec4(crntPos, 1.0f);
}