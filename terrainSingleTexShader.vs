#version 330 core

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;

out vec4 fragColor;
out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragPos;

uniform mat4 gWVP;
uniform float gMinHeight;
uniform float gMaxHeight;

void main()
{
	gl_Position = gWVP * vec4(vertPos, 1.0);

	float deltaHeight = gMaxHeight - gMinHeight;

	float heightRatio = (vertPos.y - gMinHeight) / deltaHeight;

	float c = heightRatio * 0.8 + 0.2;

	fragColor = vec4(c, c, c, 1.0);
	fragTexCoord = texCoord;
	fragNormal = normal;
	fragPos = vec3(gl_Position);
}
