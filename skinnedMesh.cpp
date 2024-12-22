#include "skinnedMesh.h"

SkinnedMesh::~SkinnedMesh()
{

}

unsigned int SkinnedMesh::getBoneID(const aiBone* pBone)
{

	int boneID = 0;
	string boneName(pBone->mName.C_Str());

	// If not found we create one, else return found one
	if (m_boneNameToIndexMap.find(boneName) == m_boneNameToIndexMap.end())
	{
		boneID = m_boneNameToIndexMap.size();
		m_boneNameToIndexMap[boneName] = boneID;
	}
	else
		boneID = m_boneNameToIndexMap[boneName];

	return boneID;
}

void SkinnedMesh::loadSingleBone(int meshIndex, const aiBone* pBone)
{

	int boneID = getBoneID(pBone);

	if (boneID == m_boneInfo.size())
	{
		BoneInfo bi(pBone->mOffsetMatrix);
		m_boneInfo.push_back(bi);
	}

	for (int i = 0; i < pBone->mNumWeights; i++)
	{
		const aiVertexWeight& vw = pBone->mWeights[i];
		
		unsigned int globalVertexID = m_meshes[meshIndex].baseVertex + vw.mVertexId;
		//printf("Vertex id %d ", globalVertexID);
		assert(globalVertexID < m_bones.size());
		m_bones[globalVertexID].addBoneData(boneID, vw.mWeight);
	}
}


void SkinnedMesh::loadMeshBones(int meshIndex,const aiMesh* pMesh)
{
	for (int i = 0; i < pMesh->mNumBones; i++)
	{
		loadSingleBone(meshIndex, pMesh->mBones[i]);
	}

}

void SkinnedMesh::loadTextures(const string& dir, const aiMaterial* pMaterial, int index)
{
	loadDiffuseTexture(dir, pMaterial, index);
	loadSpecularTexture(dir, pMaterial, index);
}

void SkinnedMesh::loadDiffuseTexture(const string& dir, const aiMaterial* pMaterial, int index)
{
	m_materials[index].pDiffuse = NULL;

	if (pMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
	{
		aiString path;

		if (pMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			string p(path.data);

			if (p.substr(0, 2) == ".\\")
			{
				p = p.substr(2, p.size() - 2);
			}

			string fullPath = dir + "/" + p;

			m_materials[index].pDiffuse = new Texture(GL_TEXTURE_2D, fullPath.c_str());

			if (!m_materials[index].pDiffuse->load())
			{
				printf("Error loading diffuse texture '%s'\n", fullPath.c_str());
				exit(1);

			}
			else
			{
				printf("Success loading diffuse texture '%s'\n", fullPath.c_str());

			}
		}
	}
}

void SkinnedMesh::loadSpecularTexture(const string& dir, const aiMaterial* pMaterial, int index)
{
	m_materials[index].pSpecularExponent = NULL;

	if (pMaterial->GetTextureCount(aiTextureType_SHININESS) > 0)
	{
		aiString path;

		if (pMaterial->GetTexture(aiTextureType_SHININESS, 0, &path, NULL, NULL, NULL, NULL, NULL) == AI_SUCCESS)
		{
			string p(path.data);

			if (p.substr(0, 2) == ".\\")
			{
				p = p.substr(2, p.size() - 2);
			}

			string fullPath = dir + "/" + p;

			m_materials[index].pSpecularExponent = new Texture(GL_TEXTURE_2D, fullPath.c_str());

			if (!m_materials[index].pSpecularExponent->load())
			{
				printf("Error loading specular texture '%s'\n", fullPath.c_str());
				exit(1);

			}
			else
			{
				printf("Success loading specular texture '%s'\n", fullPath.c_str());

			}
		}
	}
}

void SkinnedMesh::loadColors(const string& dir, const aiMaterial* pMaterial, int index)
{
	aiColor3D ambientColor(0.0f, 0.0f, 0.0f);

	if (pMaterial->Get(AI_MATKEY_COLOR_AMBIENT, ambientColor) == AI_SUCCESS)
	{
		m_materials[index].ambientColor.x = ambientColor.r;
		m_materials[index].ambientColor.y = ambientColor.g;
		m_materials[index].ambientColor.z = ambientColor.b;

	}

	aiColor3D diffuseColor(0.0f, 0.0f, 0.0f);

	if (pMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) == AI_SUCCESS)
	{
		m_materials[index].diffuseColor.x = diffuseColor.r;
		m_materials[index].diffuseColor.y = diffuseColor.g;
		m_materials[index].diffuseColor.z = diffuseColor.b;

	}

	aiColor3D specularColor(0.0f, 0.0f, 0.0f);

	if (pMaterial->Get(AI_MATKEY_COLOR_SPECULAR, specularColor) == AI_SUCCESS)
	{
		m_materials[index].specularColor.x = specularColor.r;
		m_materials[index].specularColor.y = specularColor.g;
		m_materials[index].specularColor.z = specularColor.b;

	}
}

void SkinnedMesh::countVerticesAndIndices(const aiScene* pScene, unsigned int& numVertices, unsigned int& numIndices)
{
	for (unsigned int i = 0; i < m_meshes.size(); i++)
	{
		m_meshes[i].materialIndex = pScene->mMeshes[i]->mMaterialIndex;
		m_meshes[i].numIndices = pScene->mMeshes[i]->mNumFaces * 3;
		m_meshes[i].baseVertex = numVertices;
		m_meshes[i].baseIndex = numIndices;

		numVertices += pScene->mMeshes[i]->mNumVertices;
		numIndices += m_meshes[i].numIndices;
	}
}

bool SkinnedMesh::initFromScene(const aiScene* pScene, const string& filename)
{
	m_meshes.resize(pScene->mNumMeshes);
	m_materials.resize(pScene->mNumMaterials);

	unsigned int numVertices = 0;
	unsigned int numIndices = 0;

	countVerticesAndIndices(pScene, numVertices, numIndices);

	reserveSpace(numVertices, numIndices);

	initAllMeshes(pScene);

	initMaterials(pScene, filename);

	populateBuffers();

	return true;
}

void SkinnedMesh::loadMesh(const string& filename)
{

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

	bool ret = false;

	pScene = importer.ReadFile(filename.c_str(), ASSIMP_LOAD_FLAGS);
	if (pScene)
	{
		m_globalInverseTransform = pScene->mRootNode->mTransformation;
		m_globalInverseTransform.inverse();
		ret = initFromScene(pScene, filename);
	}
	else
		printf("Error loading 3D file '%s'\n", filename);

	glBindVertexArray(0);


}

void SkinnedMesh::initMaterials(const aiScene* pScene, const string& filename)
{
	string::size_type slashIndex = filename.find_last_of("/");
	string dir;

	if (slashIndex == string::npos)
	{
		dir = ".";
	}
	else if (slashIndex == 0)
	{
		dir = "/";
	}
	else
	{
		dir = filename.substr(0, slashIndex);
	}

	for (unsigned int i = 0; i < pScene->mNumMaterials; i++)
	{
		const aiMaterial* pMaterial = pScene->mMaterials[i];

		loadTextures(dir, pMaterial, i);
		loadColors(dir, pMaterial, i);
	}
}

void SkinnedMesh::initSingleMesh(int meshIndex, const aiMesh* paiMesh)
{

	const aiVector3D zero3D(0.0f, 0.0f, 0.0f);

	for (unsigned int i = 0; i < paiMesh->mNumVertices; i++)
	{
		const aiVector3D& pPos = paiMesh->mVertices[i];
		const aiVector3D& pNormal = paiMesh->mNormals[i];
		const aiVector3D& pTexCoord = paiMesh->HasTextureCoords(0) ? paiMesh->mTextureCoords[0][i] : zero3D;

		m_positions.push_back(Vector3f(pPos.x, pPos.y, pPos.z));
		m_normals.push_back(Vector3f(pNormal.x, pNormal.y, pNormal.z));
		m_texCoords.push_back(Vector2f(pTexCoord.x, pTexCoord.y));
	}
	
	loadMeshBones(meshIndex, paiMesh);

	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace& face = paiMesh->mFaces[i];
		assert(face.mNumIndices == 3);
		m_indices.push_back(face.mIndices[0]);
		m_indices.push_back(face.mIndices[1]);
		m_indices.push_back(face.mIndices[2]);
	}

}

void SkinnedMesh::initAllMeshes(const aiScene* pScene)
{
	for (unsigned int i = 0; i < m_meshes.size(); i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		initSingleMesh(i, paiMesh);
	}
}


void SkinnedMesh::reserveSpace(unsigned int numVertices, unsigned int numIndices)
{
	m_positions.reserve(numVertices);
	m_normals.reserve(numVertices);
	m_texCoords.reserve(numVertices);
	m_indices.reserve(numIndices);
	m_bones.resize(numVertices);
}

void SkinnedMesh::populateBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_positions[0]) * m_positions.size(), &m_positions[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(POSITION_LOCATION);
	glVertexAttribPointer(POSITION_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_texCoords[0]) * m_texCoords.size(), &m_texCoords[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TEX_COORD_LOCATION);
	glVertexAttribPointer(TEX_COORD_LOCATION, 2, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_normals[0]) * m_normals.size(), &m_normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMAL_LOCATION);
	glVertexAttribPointer(NORMAL_LOCATION, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[BONE_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_bones[0]) * m_bones.size(), &m_bones[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(BONE_ID_LOCATION);
	glVertexAttribIPointer(BONE_ID_LOCATION, MAX_NUM_BONES_PER_VERTEX, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);

	glEnableVertexAttribArray(BONE_WEIGHT_LOCATION);
	glVertexAttribPointer(BONE_WEIGHT_LOCATION, MAX_NUM_BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData),
		(const GLvoid*)(MAX_NUM_BONES_PER_VERTEX * sizeof(int32_t)));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices[0]) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);


}

void SkinnedMesh::render()
{
	glBindVertexArray(m_VAO);

	for (unsigned int i = 0; i < m_meshes.size(); i++)
	{
		unsigned int materialIndex = m_meshes[i].materialIndex;

		assert(materialIndex < m_materials.size());

		if (m_materials[materialIndex].pDiffuse)
			m_materials[materialIndex].pDiffuse->bind(COLOR_TEXTURE_UNIT_0);

		if (m_materials[materialIndex].pSpecularExponent)
			m_materials[materialIndex].pSpecularExponent->bind(SPECULAR_EXPONENT_UNIT_0);

		glDrawElementsBaseVertex(
			GL_TRIANGLES,
			m_meshes[i].numIndices,
			GL_UNSIGNED_INT,
			(void*)(sizeof(unsigned int) * m_meshes[i].baseIndex),
			m_meshes[i].baseVertex);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR) {
			printf("OpenGL error %d during draw call for mesh %d\n", error, i);
		}
	}

	glBindVertexArray(0);
}

const Material& SkinnedMesh::getMaterial()
{
	for (unsigned int i = 0; i < m_materials.size(); i++)
		if (m_materials[i].ambientColor != Vector3f(0.0f, 0.0f, 0.0f))
		{
			return m_materials[i];
		}

	if (m_materials.size() == 0)
	{
		printf("No materials\n");
		exit(0);
	}

	return m_materials[0];
}

void SkinnedMesh::getBoneTransforms(vector<Matrix4f>& transforms)
{
	transforms.resize(m_boneInfo.size());

	Matrix4f identity;
	identity.setIdentity();
	printf("Size: %z\n", m_boneInfo.size());
	readNodeHiearchy(pScene->mRootNode, identity);
	
	for (unsigned int i = 0; i < m_boneInfo.size(); i++)
	{
		transforms[i] = m_boneInfo[i].finalTransform;
	}
}

void SkinnedMesh::getBoneTransforms(float timeInSeconds, vector<Matrix4f>& transforms)
{
	transforms.resize(m_boneInfo.size());

	Matrix4f identity;
	identity.setIdentity();

	float ticksPerSecond = (float)(pScene->mAnimations[0]->mTicksPerSecond != 0 ? pScene->mAnimations[0]->mTicksPerSecond : 25.0f);
	float timeInTicks = timeInSeconds * ticksPerSecond;
	float animationTimeTicks = fmod(timeInTicks, (float)pScene->mAnimations[0]->mDuration);


	readNodeHiearchy(animationTimeTicks, pScene->mRootNode, identity);
	transforms.resize(m_boneInfo.size());

	for (unsigned int i = 0; i < m_boneInfo.size(); i++)
	{
		transforms[i] = m_boneInfo[i].finalTransform;
	}
}

void SkinnedMesh::readNodeHiearchy(const aiNode* pNode, Matrix4f& parentTransform)
{
	string nodeName(pNode->mName.data);
	Matrix4f nodeTransformation(pNode->mTransformation);

	/*if (!nodeName.empty()) {
		printf("Node name: %s\n", nodeName.c_str());
	}
	else {
		printf("Node name is empty or invalid\n");
	}*/

	Matrix4f globalTransofrmation = parentTransform * nodeTransformation;

	if (m_boneNameToIndexMap.find(nodeName) != m_boneNameToIndexMap.end())
	{
		unsigned int boneIndex = m_boneNameToIndexMap[nodeName];
		m_boneInfo[boneIndex].finalTransform = globalTransofrmation * m_boneInfo[boneIndex].offsetMatrix;
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; i++)
	{
		readNodeHiearchy(pNode->mChildren[i], globalTransofrmation);
	}
}

void SkinnedMesh::readNodeHiearchy(float animationTimeInTicks, const aiNode* pNode, Matrix4f& parentTransform)
{
	string nodeName(pNode->mName.data);
	Matrix4f nodeTransformation(pNode->mTransformation);

	const aiAnimation* pAnimation = pScene->mAnimations[0];
	const aiNodeAnim* pNodeAnim = findNodeAnim(pAnimation, nodeName);

	if (pNodeAnim)
	{
		aiVector3D scaling;
		calcInterpolatedScaling(scaling, animationTimeInTicks, pNodeAnim);
		Matrix4f scalingM;
		scalingM.setScale(Vector3f(scaling.x, scaling.y, scaling.z));

		aiQuaternion rotationQ;
		calcInterpolatedRotation(rotationQ, animationTimeInTicks, pNodeAnim);
		Matrix4f rotationM(rotationQ.GetMatrix());

		aiVector3D translation;
		calcInterpolatedTranslation(translation, animationTimeInTicks, pNodeAnim);
		Matrix4f translationM;
		translationM.setTranslation(Vector3f(translation.x, translation.y, translation.z));
	
		nodeTransformation = translationM * rotationM * scalingM;
		//nodeTransformation = translationM * rotationM * scalingM;
	}

	Matrix4f globalTransofrmation = parentTransform * nodeTransformation;

	
	if (m_boneNameToIndexMap.find(nodeName) != m_boneNameToIndexMap.end())
	{
		unsigned int boneIndex = m_boneNameToIndexMap[nodeName];
		m_boneInfo[boneIndex].finalTransform = m_globalInverseTransform * globalTransofrmation * m_boneInfo[boneIndex].offsetMatrix;
	}

	for (unsigned int i = 0; i < pNode->mNumChildren; i++)
	{
		readNodeHiearchy(animationTimeInTicks, pNode->mChildren[i], globalTransofrmation);
	}
}

const aiNodeAnim* SkinnedMesh::findNodeAnim(const aiAnimation* pAnimation, const string nodeName)
{
	for (unsigned int i = 0; i < pAnimation->mNumChannels; i++)
	{
		const aiNodeAnim* pNodeAnim = pAnimation->mChannels[i];

		if (string(pNodeAnim->mNodeName.data) == nodeName)
			return pNodeAnim;
	}

	return NULL;
}

void SkinnedMesh::calcInterpolatedScaling(aiVector3D& out, float animationTimeTicks, const aiNodeAnim* pNodeAnim)
{

	if (pNodeAnim->mNumScalingKeys == 1)
	{
		out = pNodeAnim->mScalingKeys[0].mValue;
		return;
	}

	unsigned int scalingIndex = findScaling(animationTimeTicks, pNodeAnim);
	unsigned int nextScallingIndex = scalingIndex + 1;
	assert(nextScallingIndex < pNodeAnim->mNumScalingKeys);

	float t1 = (float)pNodeAnim->mScalingKeys[scalingIndex].mTime;
	float t2 = (float)pNodeAnim->mScalingKeys[nextScallingIndex].mTime;

	float deltaTime = t2 - t1;
	float factor = (animationTimeTicks - t1) / deltaTime;
	const aiVector3D& start = pNodeAnim->mScalingKeys[scalingIndex].mValue;
	const aiVector3D& end = pNodeAnim->mScalingKeys[nextScallingIndex].mValue;
	aiVector3D delta = end - start;

	out = start + factor * delta;

}

unsigned int SkinnedMesh::findScaling(float animationTimeTicks, const aiNodeAnim* pNodeAnim)
{
	assert(pNodeAnim->mNumScalingKeys > 0);

	for (unsigned int i = 0; i < pNodeAnim->mNumScalingKeys - 1; i++)
	{
		float t = (float)pNodeAnim->mScalingKeys[i + 1].mTime;
		if (animationTimeTicks < t)
			return i;
	}

	return 0;
}

void SkinnedMesh::calcInterpolatedRotation(aiQuaternion& out, float animationTimeTicks, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumRotationKeys == 1)
	{
		out = pNodeAnim->mRotationKeys[0].mValue;
		return;
	}

	unsigned int rotationIndex = findRotation(animationTimeTicks, pNodeAnim);
	unsigned int nextRotationIndex = rotationIndex + 1;
	assert(nextRotationIndex < pNodeAnim->mNumRotationKeys);

	float t1 = (float)pNodeAnim->mRotationKeys[rotationIndex].mTime;
	float t2 = (float)pNodeAnim->mRotationKeys[nextRotationIndex].mTime;

	float deltaTime = t2 - t1;
	float factor = (animationTimeTicks - t1) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	const aiQuaternion& start = pNodeAnim->mRotationKeys[rotationIndex].mValue;
	const aiQuaternion& end = pNodeAnim->mRotationKeys[nextRotationIndex].mValue;
	
	aiQuaternion::Interpolate(out, start, end, factor);

	out.Normalize();
}

unsigned int SkinnedMesh::findRotation(float animationTimeTicks, const aiNodeAnim* pNodeAnim)
{
	for (unsigned int i = 0; i < pNodeAnim->mNumRotationKeys - 1; i++)
	{
		float t = (float)pNodeAnim->mRotationKeys[i + 1].mTime;
		if (animationTimeTicks < t)
			return i;
	}

	return 0;
	
}

void SkinnedMesh::calcInterpolatedTranslation(aiVector3D& out, float animationTimeTicks, const aiNodeAnim* pNodeAnim)
{
	if (pNodeAnim->mNumPositionKeys == 1)
	{
		out = pNodeAnim->mPositionKeys[0].mValue;
		return;
	}

	unsigned int positionIndex = findTranslation(animationTimeTicks, pNodeAnim);
	unsigned int nextPositionIndex = positionIndex + 1;
	assert(nextPositionIndex < pNodeAnim->mNumPositionKeys);

	float t1 = (float)pNodeAnim->mPositionKeys[positionIndex].mTime;
	float t2 = (float)pNodeAnim->mPositionKeys[nextPositionIndex].mTime;

	float deltaTime = t2 - t1;
	float factor = (animationTimeTicks - t1) / deltaTime;
	assert(factor >= 0.0f && factor <= 1.0f);
	const aiVector3D& start = pNodeAnim->mPositionKeys[positionIndex].mValue;
	const aiVector3D& end = pNodeAnim->mPositionKeys[nextPositionIndex].mValue;
	aiVector3D delta = end - start;

	out = start + factor * delta;
}

unsigned int SkinnedMesh::findTranslation(float animationTimeTicks, const aiNodeAnim* pNodeAnim)
{
	for (unsigned int i = 0; i < pNodeAnim->mNumPositionKeys - 1; i++) {
		float t = (float)pNodeAnim->mPositionKeys[i + 1].mTime;
		if (animationTimeTicks < t) 
			return i;
	}

	return 0;
}
