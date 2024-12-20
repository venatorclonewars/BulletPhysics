//#include <cassert>
//#include "baseTerrain.h"
//#include "triangleList.h"
//
//TriangleList::TriangleList()
//{
//}
//
//void TriangleList::createTriangleList(int width, int depth, const BaseTerrain* pTerrain)
//{
//	m_width = width;
//	m_depth = depth;
//
//	createGLState();
//
//	populateBuffers(pTerrain);
//
//	glBindVertexArray(0);
//	glBindBuffer(GL_ARRAY_BUFFER, 0);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//}
//
//void TriangleList::render()
//{
//
//	glBindVertexArray(m_VAO);
//	//glDrawArrays(GL_POINTS, 0, m_depth * m_width);
//	glDrawElements(GL_TRIANGLES, (m_depth - 1) * (m_width - 1) * 6, GL_UNSIGNED_INT, NULL);
//
//
//	glBindVertexArray(0);
//}
//
//void TriangleList::createGLState()
//{
//	glGenVertexArrays(1, &m_VAO);
//	glBindVertexArray(m_VAO);
//
//	glGenBuffers(1, &m_VBO);
//	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
//
//	glGenBuffers(1, &m_IBO);
//	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
//
//	int POS_LOC = 0;
//	int TEX_LOC = 1;
//	int NORMAL_LOC = 2;
//
//	size_t numFloats = 0;
//
//	glEnableVertexAttribArray(POS_LOC);
//	glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(numFloats * sizeof(float)));
//	numFloats += 3;
//
//	glEnableVertexAttribArray(TEX_LOC);
//	glVertexAttribPointer(TEX_LOC, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(numFloats * sizeof(float)));
//	numFloats += 2;
//
//	glEnableVertexAttribArray(NORMAL_LOC);
//	glVertexAttribPointer(NORMAL_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(numFloats * sizeof(float)));
//	numFloats += 3;
//}
//
//void TriangleList::populateBuffers(const BaseTerrain* pTerrain)
//{
//	vector<Vertex> vertices;
//
//	vertices.resize(m_width * m_depth);
//
//	initVertices(pTerrain, vertices);
//
//	vector<unsigned int> indices;
//	int numQuads = (m_width - 1) * (m_depth - 1);
//	indices.resize(numQuads * 6);
//
//	initInidces(indices);
//
//	calcNormals(indices, vertices);
//
//	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
//	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);
//
//
//}
//
//void TriangleList::initVertices(const BaseTerrain* pTerrain, vector<Vertex>& vertices)
//{
//	int index = 0;
//
//	for (int z = 0; z < m_depth; z++)
//	{
//		for (int x = 0; x < m_width; x++)
//		{
//			assert(index < vertices.size());
//			//vertices[index].setVertex(x, pTerrain->getHeight(x, z), z);
//			vertices[index].setVertex(pTerrain, x, z);
//			index++;
//		}
//	}
//}
//
//void TriangleList::initInidces(vector<unsigned int>& indices)
//{
//	int index = 0;
//
//	for (int z = 0; z < m_depth - 1; z++)
//	{
//		for (int x = 0; x < m_depth - 1; x++)
//		{
//			unsigned int indexBottomLeft = z * m_width + x;
//			unsigned int indexTopLeft = (z + 1) * m_width + x;
//			unsigned int indexTopRigth = (z + 1) * m_width + x + 1;
//			unsigned int indexBottomRigth = z * m_width + x + 1;
//		
//			indices[index++] = indexBottomLeft;
//			indices[index++] = indexTopLeft;
//			indices[index++] = indexTopRigth;
//
//			indices[index++] = indexBottomLeft;
//			indices[index++] = indexTopRigth;
//			indices[index++] = indexBottomRigth;
//		}
//	}
//}
//
//void TriangleList::calcNormals(vector<unsigned int>& indices, vector<Vertex>& vertices)
//{
//	unsigned int index = 0;
//
//	for (unsigned int i = 0; i < indices.size(); i += 3)
//	{
//		unsigned int index0 = indices[i];
//		unsigned int index1 = indices[i + 1];
//		unsigned int index2 = indices[i + 2];
//
//		Vector3f v1 = vertices[index1].pos - vertices[index0].pos;
//		Vector3f v2 = vertices[index2].pos - vertices[index0].pos;
//		Vector3f normal = v1.cross(v2);
//		normal = normal.normalize();
//
//		vertices[index0].normal += normal;
//		vertices[index1].normal += normal;
//		vertices[index2].normal += normal;
//	}
//
//	for (unsigned int i = 0; i < vertices.size(); i++)
//		vertices[i].normal.normalize();
//}
//
//void Vertex::setVertex(const BaseTerrain* pTerrain, int x, int z)
//{
//	float y = pTerrain->getHeight(x, z);
//
//	float worldScale = pTerrain->getWorldScale();
//	pos = Vector3f(x * worldScale, y, z * worldScale);
//	
//	float size = (float)pTerrain->getSize();
//	float textureScale = pTerrain->getTextureScale();
//
//	tex = Vector2f(textureScale * (float)x / size, textureScale * (float)z / size);
//}
