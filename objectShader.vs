#version 330

layout (location = 0) in vec3 vertPos;

uniform mat4 gWVP;


void main()
{
	
	gl_Position = gWVP * vec4(vertPos, 1.0);	
}