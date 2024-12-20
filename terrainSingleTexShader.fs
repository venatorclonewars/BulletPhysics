#version 330 core

layout (location = 0) out vec4 FragColor;

in vec4 fragColor;
in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragPos;

uniform sampler2D gTerrainTexture;
uniform vec3 gLightDir;
uniform vec3 gCamPos;
uniform float gExpFogDensity = 0.2;

vec3 fogColor = vec3(0.8, 0.8, 0.8);
float gFogStart = 20;
float gFogEnd = 30;

float calcLinearFog()
{
	float camToPixelDist = length(fragPos - gCamPos);
	float fogRange = gFogEnd - gFogStart;
	float fogDist = gFogEnd - camToPixelDist;
	float fogFactor = fogDist / fogRange;
	fogFactor = clamp(fogFactor, 0.5, 1.0);

	return fogFactor;
}

float calcExpFogFactor()
{
	float camToPixelDist = length(fragPos - gCamPos);
	float distRatio = 4.0 * camToPixelDist / gFogEnd;
	float fogFactor = exp(-distRatio * gExpFogDensity * distRatio * gExpFogDensity);
	fogFactor = clamp(fogFactor, 0.3, 1.0);
	return fogFactor;
}

float calcFogFactor()
{
	float fogFactor = calcExpFogFactor();
	//float fogFactor = calcLinearFog();
	return fogFactor;
}

void main()
{
	//FragColor = fragColor;
	vec4 texColor = texture(gTerrainTexture, fragTexCoord);

	vec3 normal = normalize(fragNormal);

	float diffuse = dot(normal, gLightDir);
	diffuse = max(0.3, diffuse);

	float fogFactor = calcFogFactor();

	vec4 tempColor = fragColor * texColor * diffuse;

	vec3 finalColor = mix(fogColor, tempColor.rgb, fogFactor);

    FragColor = vec4(finalColor, 1.0);
}