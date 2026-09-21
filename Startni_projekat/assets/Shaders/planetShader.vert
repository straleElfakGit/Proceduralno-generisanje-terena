#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

out vec3 normal;
out vec3 fragPos;
out vec3 objectPos;

uniform mat4 model;
uniform mat4 camMat;
uniform mat3 normalMatrix;

void main()
{
	vec4 worldPos = model * vec4(aPos, 1.0f);
	gl_Position = camMat * worldPos;

	normal = normalMatrix * aNormal;
	fragPos = vec3(worldPos);

	objectPos = aPos;
}