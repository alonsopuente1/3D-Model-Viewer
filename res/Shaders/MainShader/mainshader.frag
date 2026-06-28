#version 330 core
out vec4 FragColour;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

vec3 lightPos = vec3(0, 1, 0);
vec3 lightColour = vec3(1);

vec3 objectColour = vec3(1, 0, 0);

uniform sampler2D uTexture;
uniform bool uUseTexture;
uniform vec3 uObjectColour;

void main()
{
	// Phong shading algorithm taken from learnopengl.com/Lighting/Basic-Lighting

	if(uUseTexture)
	{
		objectColour = vec3(1); 
	}
	else
	{
		objectColour = uObjectColour;
	}

	// Ambient lighting
	float ambientStrength = 0.1;
	vec3 ambient = ambientStrength * lightColour;

	// Diffuse lighting
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);

	float diff = max(dot(norm, lightDir), 0.0f);
	vec3 diffuse = diff * lightColour;

	vec3 result = (ambient + diffuse) * objectColour;
	if(uUseTexture)
	{
		FragColour = texture(uTexture, TexCoord) * vec4(result, 1.0f);
	}
	else
	{
		FragColour = vec4(result, 1.0f);
	}
}