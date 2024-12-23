#version 330

out vec4 FragColor;

in vec3 fragNormal;

uniform vec3 gLightDir;
uniform mat4 gModelMatrix;

void main()
{
	
	 mat3 normalMatrix = transpose(inverse(mat3(gModelMatrix)));
   
    vec3 normal = normalMatrix * fragNormal;

	normal = normalize(normal);

	vec3 lightDir = normalize(-gLightDir);

	float diffuseFactor = max(0.0, dot(normal, lightDir));

	//FragColor = vec4(normal, 1);
	FragColor = vec4(1, 1, 1, 1) * diffuseFactor;

}