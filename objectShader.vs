#version 330

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec3 normal;

uniform mat4 gWVP;

out vec3 fragNormal;

void main()
{
	
	gl_Position = gWVP * vec4(vertPos, 1.0);

	fragNormal = normal;

}