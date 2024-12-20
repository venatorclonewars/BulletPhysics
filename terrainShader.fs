#version 330 core

layout (location = 0) out vec4 FragColor;

in vec4 fragColor;
in vec2 fragTexCoord;

uniform sampler2D gTerrainTexture;

void main()
{
	//FragColor = fragColor;
	vec4 texColor = texture2D(gTerrainTexture, fragTexCoord);
    FragColor = FragColor * texColor;
}