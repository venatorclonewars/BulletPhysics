#version 330

out vec4 FragColor;

in vec3 fragNormal;

uniform vec3 gLightDir;

void main()
{
	
	vec3 normal = normalize(fragNormal);
	float diffuseFactor;

	if (normal != vec3(0))
		diffuseFactor = 1;

	else
		diffuseFactor = dot(normal, -gLightDir);

	FragColor = vec4(1, 1, 1, 1);// * diffuseFactor;

}