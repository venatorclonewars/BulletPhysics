//#include <assimp/Importer.hpp>
//#include <assimp/scene.h>
//#include <assimp/postprocess.h>
//
//#define ASSIMP_LOAD_FLAGS (aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices | aiProcess_PreTransformVertices)
//
//
//void parseSingleBone(int boneIdx, const aiBone* pBone)
//{
//
//	for (int i = 0; i <= pBone->mNumWeights; i++)
//	{
//		const aiVertexWeight& vw = pBone->mWeights[i];
//	}
//}
//
//
//
//void parseScene(const aiScene* pScene)
//{
//	parseMeshes(pScene);
//}
//
//int main(int argc, char* argv[])
//{
//	if (argc != 2)
//		return 1;
//	
//	char* filename = argv[1];
//
//	Assimp::Importer importer;
//	const aiScene* pScene = importer.ReadFile(filename, ASSIMP_LOAD_FLAGS);
//
//	if (!pScene)
//	{
//		printf("Error parsing '%s' : '%s'\n", filename, importer.GetErrorString());
//	}
//
//
//}