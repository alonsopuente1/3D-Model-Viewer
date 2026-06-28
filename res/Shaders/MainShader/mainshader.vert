#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTex;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 uMVP;
uniform mat4 uModelMat;

void main()
{
	gl_Position = uMVP * vec4(aPos, 1.0);

	Normal = mat3(transpose(inverse(uModelMat))) * aNormal;
	FragPos = vec3(uModelMat * vec4(aPos, 1.0));
	TexCoord = aTex;
}