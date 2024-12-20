#include "mesh.h"
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>       // Output data structure
#include <assimp/postprocess.h> // Post processing flags
#include <cassert>


bool Mesh::loadMesh(const string& filename)
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(ARRAY_SIZE_IN_ELEMENTS(m_Buffers), m_Buffers);

	bool ret = false;
	Assimp::Importer importer;

	const aiScene* pScene = importer.ReadFile(filename.c_str(), ASSIMP_LOAD_FLAGS);
	if (pScene)
	{
		ret = initFromScene(pScene, filename);
	}
	else
		printf("Error loading 3D file '%s'\n", filename.c_str());
	
	glBindVertexArray(0);

	return ret;
}

void Mesh::loadTextures(const string& dir, const aiMaterial* pMaterial, int index)
{

	loadDiffuseTexture(dir, pMaterial, index);
	loadSpecularTexture(dir, pMaterial, index);
}

void Mesh::loadDiffuseTexture(const string& dir, const aiMaterial* pMaterial, int index)
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

void Mesh::loadSpecularTexture(const string& dir, const aiMaterial* pMaterial, int index)
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

void Mesh::loadColors(const string& dir, const aiMaterial* pMaterial, int index)
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

bool Mesh::initFromScene(const aiScene* pScene, const string& filename)
{
	m_meshes.resize(pScene->mNumMeshes);
	m_textures.resize(pScene->mNumMaterials);
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

void Mesh::countVerticesAndIndices(const aiScene* pScene, unsigned int& numVertices, unsigned int& numIndices)
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

void Mesh::reserveSpace(unsigned int numVertices, unsigned int numIndices)
{
	m_positions.reserve(numVertices);
	m_normals.reserve(numVertices);
	m_texCoords.reserve(numVertices);
	m_indices.reserve(numIndices);
}

void Mesh::initAllMeshes(const aiScene* pScene)
{
	for (unsigned int i = 0; i < m_meshes.size(); i++)
	{
		const aiMesh* paiMesh = pScene->mMeshes[i];
		initSingleMesh(paiMesh);
	}
}

void Mesh::initSingleMesh(const aiMesh* paiMesh)
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


	for (unsigned int i = 0; i < paiMesh->mNumFaces; i++)
	{
		const aiFace& face = paiMesh->mFaces[i];
		assert(face.mNumIndices == 3);
		m_indices.push_back(face.mIndices[0]);
		m_indices.push_back(face.mIndices[1]);
		m_indices.push_back(face.mIndices[2]);
	}

}

void Mesh::initMaterials(const aiScene* pScene, const string& filename)
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

void Mesh::populateBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_Buffers[POS_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(m_positions[0]) * m_positions.size(),&m_positions[0], GL_STATIC_DRAW);
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

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Buffers[INDEX_BUFFER]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_indices[0]) * m_indices.size(), &m_indices[0], GL_STATIC_DRAW);
	
}

void Mesh::render()
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

const Material& Mesh::getMaterial()
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
