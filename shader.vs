#version 330

layout (location = 0) in vec3 vertPos;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 3) in ivec4 boneIDs;
layout (location = 4) in vec4 weights;

const int MAX_BONES = 100;

uniform mat4 gWVP;
uniform mat4 gBones[MAX_BONES];

out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragVertPos;
flat out ivec4 fragBoneIDs;
out vec4 fragWeights;

void main()
{
	mat4 boneTransform = gBones[boneIDs[0]] * weights[0];
	boneTransform += gBones[boneIDs[1]] * weights[1];
	boneTransform += gBones[boneIDs[2]] * weights[2];
	boneTransform += gBones[boneIDs[3]] * weights[3];

	vec4 posL = boneTransform * vec4(vertPos, 1.0);
	gl_Position = gWVP * posL;
	//gl_Position = gWVP * vec4(vertPos, 1.0);

	fragTexCoord = texCoord;
	mat3 normalMatrix = mat3(boneTransform);
    normalMatrix = transpose(inverse(normalMatrix)); // Compute the inverse transpose
    fragNormal = normalize(normalMatrix * normal);
	fragVertPos = vertPos;

	fragBoneIDs = boneIDs;
	fragWeights = weights;

}