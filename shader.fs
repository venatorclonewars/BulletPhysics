#version 330

const int MAX_POINT_LIGHTS = 2;

out vec4 FragColor;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragVertPos;

flat in ivec4 fragBoneIDs;
in vec4 fragWeights;

struct BaseLight
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight
{
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
	vec3 direction;
};

struct Attenuation
{
	float constant;
	float linear;
	float exp;
};

struct PointLight
{
	BaseLight base;
	vec3 localPos;
	Attenuation atten;
};
struct Material
{
	vec3 ambientColor;
	vec3 diffuseColor;
	vec3 specularColor;
};


uniform DirectionalLight gDirectionalLight;
uniform int gNumPointLights;
uniform PointLight gPointLights[MAX_POINT_LIGHTS];
uniform Material gMaterial;
uniform sampler2D gSampler;
uniform sampler2D gSamplerSpecularExp;
uniform vec3 gCameraLocalPos;
uniform int gDisplayBoneIndex;

void main()
{
	vec4 ambientColor = texture(gSampler, fragTexCoord.xy) *
	vec4(gMaterial.ambientColor, 1.0f) *
	gDirectionalLight.ambientIntensity;

	vec3 normal = normalize(fragNormal);

	float diffuseFactor = dot(normal, -gDirectionalLight.direction);
	vec4 diffuseColor = vec4(0, 0, 0, 0);
	vec4 specularColor = vec4(0, 0, 0, 0);

	if (diffuseFactor > 0)
	{
		diffuseColor = vec4(gDirectionalLight.color, 1.0f) *
		gDirectionalLight.diffuseIntensity *
		vec4(gMaterial.diffuseColor, 1.0f) *
		diffuseFactor;

		vec3 pixelToCam = normalize(gCameraLocalPos - fragVertPos);
		vec3 lightReflect = normalize(reflect(gDirectionalLight.direction, normal));
		float specularFactor = dot(pixelToCam, lightReflect);

		if (specularFactor > 0)
		{
			float specularExp = texture(gSamplerSpecularExp, fragTexCoord).r * 255.0;
			specularFactor = pow(specularFactor, specularExp);
			specularColor = vec4(gDirectionalLight.color, 1.0f) *
							vec4(gMaterial.specularColor, 1.0f) *
							specularFactor;
		}
	}
		
	bool found = false;

    for (int i = 0 ; i < 4 ; i++) 
	{
        if (fragBoneIDs[i] == gDisplayBoneIndex) 
		{
           if (fragWeights[i] >= 0.7) 
		   {
               FragColor = vec4(1.0, 0.0, 0.0, 0.0) * fragWeights[i];
           } 
		   else if (fragWeights[i] >= 0.4 && fragWeights[i] <= 0.6) 
		   {
               FragColor = vec4(0.0, 1.0, 0.0, 0.0) * fragWeights[i];
           } 
		   else if (fragWeights[i] >= 0.1) 
		   {
               FragColor = vec4(1.0, 1.0, 0.0, 0.0) * fragWeights[i];
           }

           found = true;
           break;
        }
    }

	
	FragColor = texture(gSampler, fragTexCoord.xy) *
						clamp((ambientColor + diffuseColor + specularColor), 0, 1);

	//if (!found)	
	//FragColor = texture(gSampler, fragTexCoord.xy) *
	//					clamp((ambientColor + diffuseColor + specularColor), 0, 1)  + vec4(0.0, 0.0, 1.0, 0.0);

}