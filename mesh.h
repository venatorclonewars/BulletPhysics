#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <assimp/scene.h>
#include "texture.h"
#include "material.h"

using namespace std;

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a) / sizeof((a)[0]))
// If wrong UVs add aiProcess_FlipUVs 
// Also for blender imports add aiProcess_PreTransformVertices
#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices)
#define COLOR_TEXTURE_UNIT_INDEX_0 0 
#define COLOR_TEXTURE_UNIT_0 GL_TEXTURE0 

#define SPECULAR_EXPONENT_UNIT_INDEX_0 6 
#define SPECULAR_EXPONENT_UNIT_0 GL_TEXTURE6 

#define POSITION_LOCATION 0 
#define TEX_COORD_LOCATION 1 
#define NORMAL_LOCATION 2 

class Mesh
{
public:
	bool loadMesh(const string& filename);
	void loadTextures(const string& dir, const aiMaterial* pMaterial, int index);
	void loadDiffuseTexture(const string& dir, const aiMaterial* pMaterial, int index);
	void loadSpecularTexture(const string& dir, const aiMaterial* pMaterial, int index);
	void loadColors(const string& dir, const aiMaterial* pMaterial, int index);
	bool initFromScene(const aiScene* pScene, const string& filename);
	void countVerticesAndIndices(const aiScene* pScene, unsigned int& numVertices, unsigned int& numIndices);
	void reserveSpace(unsigned int numVertices, unsigned int numIndices);;
	void initAllMeshes(const aiScene* pScene);
	void initSingleMesh(const aiMesh* paiMesh);
	void initMaterials(const aiScene* pScene, const string& filename);
	void populateBuffers();
	void render();
	const Material& getMaterial();
	Matrix4f& getWorldTransform() { return m_worldTransform; }
	

	enum BUFFER_TYPE
	{
		INDEX_BUFFER = 0,
		POS_VB = 1,
		TEXCOORD_VB = 2,
		NORMAL_VB = 3,
		WVP_MAT_VB = 4,
		WORLD_MAT_VB = 5,
		NUM_BUFFERS = 6
	};

#define INVALID_MATERIAL 0xFFFFFFFF

	struct BasicMeshEntry
	{
		BasicMeshEntry()
		{
			numIndices = 0;
			baseVertex = 0;
			baseIndex = 0;
			materialIndex = INVALID_MATERIAL;
		}

		unsigned int numIndices;
		unsigned int baseVertex;
		unsigned int baseIndex;
		unsigned int materialIndex;
	};

private:
	GLuint m_VAO = -1;
	GLuint m_Buffers[NUM_BUFFERS] = { 0 };

	vector<BasicMeshEntry> m_meshes;
	vector<Texture*> m_textures;
	vector<Material> m_materials;

	vector<Vector3f> m_positions;
	vector<Vector3f> m_normals;
	vector<Vector2f> m_texCoords;
	vector<int> m_indices;

	Matrix4f m_worldTransform;
};

