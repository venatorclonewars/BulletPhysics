#include "geomipGrid.h"
#include <cassert>
#include "baseTerrain.h"

extern int gShowPoints = 0;

GeomipGrid::GeomipGrid()
{
}

GeomipGrid::~GeomipGrid()
{
	delete m_pTerrain;
	delete m_convexShape;
	delete m_triangleMesh;
}

void GeomipGrid::createGeomipGrid(int width, int depth, int patchSize, const BaseTerrain* pTerrain, btDiscreteDynamicsWorld* dynamicsWorld)
{
	if ((width - 1) % (patchSize - 1) != 0) {
		int RecommendedWidth = ((width - 1 + patchSize - 1) / (patchSize - 1)) * (patchSize - 1) + 1;
		printf("Width minus 1 (%d) must be divisible by PatchSize minus 1 (%d)\n", width, patchSize);
		printf("Try using Width = %d\n", RecommendedWidth);
		exit(0);
	}

	if ((depth - 1) % (patchSize - 1) != 0) {
		int RecommendedDepth = ((depth - 1 + patchSize - 1) / (patchSize - 1)) * (patchSize - 1) + 1;
		printf("Depth minus 1 (%d) must be divisible by PatchSize minus 1 (%d)\n", depth, patchSize);
		printf("Try using Width = %d\n", RecommendedDepth);
		exit(0);
	}

	if (patchSize < 3) {
		printf("The minimum patch size is 3 (%d)\n", patchSize);
		exit(0);
	}

	if (patchSize % 2 == 0) {
		printf("Patch size must be an odd number (%d)\n", patchSize);
		exit(0);
	}

	m_width = width;
	m_depth = depth;
	m_patchSize = patchSize;
	m_pTerrain = pTerrain;
	m_worldScale = pTerrain->getWorldScale();
	m_dynamicsWorld = dynamicsWorld;
	m_convexShape = new btConvexHullShape();
	m_triangleMesh = new btTriangleMesh();

	m_numPatchesX = (width - 1) / (patchSize - 1);
	m_numPatchesZ = (depth - 1) / (patchSize - 1);

	m_maxLOD = m_lodManager.initLodManager(patchSize, m_numPatchesX, m_numPatchesZ, m_worldScale);
	m_lodInfo.resize(m_maxLOD + 1);

	createGLState();

	populateBuffers(pTerrain);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void GeomipGrid::render()
{

	glBindVertexArray(m_VAO);
	//glDrawArrays(GL_POINTS, 0, m_depth * m_width);
	for (int z = 0; z < m_depth - 1; z += (m_patchSize - 1))
	{
		for (int x = 0; x < m_width - 1; x += (m_patchSize - 1)) 
		{
			int baseVertex = z * m_width + x;
			glDrawElementsBaseVertex(GL_TRIANGLES, (m_patchSize - 1) * (m_patchSize - 1) * 6, GL_UNSIGNED_INT, NULL, baseVertex);

		}
	}
	
	

	glBindVertexArray(0);
}

void GeomipGrid::render(const Vector3f& cameraPos, const Matrix4f& WVP)
{
	m_lodManager.update(cameraPos);

	FrustumCulling fc(WVP);

	glBindVertexArray(m_VAO);

	if (gShowPoints > 0) {
		glDrawElementsBaseVertex(GL_POINTS, m_lodInfo[0].info[0][0][0][0].count, GL_UNSIGNED_INT, (void*)0, 0);
	}

	if (gShowPoints != 2) 
	{
		for (int patchZ = 0; patchZ < m_numPatchesZ; patchZ++) 
		{
			for (int patchX = 0; patchX < m_numPatchesX; patchX++) 
			{

				int z = patchZ * (m_patchSize - 1);
				int x = patchX * (m_patchSize - 1);

				if (!isPatchInsideViewFrustum_WorldSpace(x, z, fc))
				//if (!isPatchInsideViewFrustrum_ViewSpace(x, z, WVP))
				{
					printf("0");
					continue;
				}
				else
				{
					printf("1");
				}

				const LodManager::PatchLod& plod = m_lodManager.getPatchLod(patchX, patchZ);
				int C = plod.Core;
				int L = plod.Left;
				int R = plod.Right;
				int T = plod.Top;
				int B = plod.Bottom;

				size_t baseIndex = sizeof(unsigned int) * m_lodInfo[C].info[L][R][T][B].start;

				int baseVertex = z * m_width + x;
				//            printf("%d\n", BaseVertex);

				glDrawElementsBaseVertex(GL_TRIANGLES, m_lodInfo[C].info[L][R][T][B].count,
					GL_UNSIGNED_INT, (void*)baseIndex, baseVertex);
			}
		}
	}

	glBindVertexArray(0);
}

void GeomipGrid::render(const Vector3f& cameraPos, Matrix4f& projection, Matrix4f& view, Matrix4f _view)
{
	m_lodManager.update(cameraPos);

	//FrustumCulling fc(projection * _view);

	glBindVertexArray(m_VAO);

	if (gShowPoints > 0) {
		glDrawElementsBaseVertex(GL_POINTS, m_lodInfo[0].info[0][0][0][0].count, GL_UNSIGNED_INT, (void*)0, 0);
	}

	//Matrix4f WVP = projection * view;

	if (gShowPoints != 2)
	{
		for (int patchZ = 0; patchZ < m_numPatchesZ; patchZ++)
		{
			for (int patchX = 0; patchX < m_numPatchesX; patchX++)
			{

				int z = patchZ * (m_patchSize - 1);
				int x = patchX * (m_patchSize - 1);

				
				const LodManager::PatchLod& plod = m_lodManager.getPatchLod(patchX, patchZ);
				int C = plod.Core;
				int L = plod.Left;
				int R = plod.Right;
				int T = plod.Top;
				int B = plod.Bottom;

				size_t baseIndex = sizeof(unsigned int) * m_lodInfo[C].info[L][R][T][B].start;

				int baseVertex = z * m_width + x;
				// printf("%d\n", BaseVertex);

				glDrawElementsBaseVertex(GL_TRIANGLES, m_lodInfo[C].info[L][R][T][B].count,
					GL_UNSIGNED_INT, (void*)baseIndex, baseVertex);
			}
			//printf("\n");
		}
	}
	
	glBindVertexArray(0);
}

void GeomipGrid::update(const Vector3f& cameraPos, Matrix4f& projection, Matrix4f& view, Matrix4f _view)
{
	render(cameraPos, projection, view, _view);
}

void GeomipGrid::createRigidBody(vector<Vertex>& m_vertices)
{
	btTriangleMesh* triangleMesh = new btTriangleMesh();

	for (int z = 0; z < (m_patchSize - 1) * m_numPatchesZ; z++)  
	{
		for (int x = 0; x < (m_patchSize - 1) * m_numPatchesX; x++)
		{
			unsigned int indexBottomLeft = z * m_width + x;
			unsigned int indexTopLeft = (z + 1) * m_width + x;
			unsigned int indexTopRight = (z + 1) * m_width + x + 1;
			unsigned int indexBottomRight = z * m_width + x + 1;

			btVector3 vertex0(m_vertices[indexBottomLeft].pos.x, m_vertices[indexBottomLeft].pos.y, m_vertices[indexBottomLeft].pos.z);
			btVector3 vertex1(m_vertices[indexTopLeft].pos.x, m_vertices[indexTopLeft].pos.y, m_vertices[indexTopLeft].pos.z);
			btVector3 vertex2(m_vertices[indexTopRight].pos.x, m_vertices[indexTopRight].pos.y, m_vertices[indexTopRight].pos.z);
			btVector3 vertex3(m_vertices[indexBottomRight].pos.x, m_vertices[indexBottomRight].pos.y, m_vertices[indexBottomRight].pos.z);

			triangleMesh->addTriangle(vertex0, vertex1, vertex2, true);
			triangleMesh->addTriangle(vertex0, vertex2, vertex3, true);
		}
	}

	btBvhTriangleMeshShape* meshShape = new btBvhTriangleMeshShape(triangleMesh, true);

	btScalar mass = 0.0f;
	btVector3 inertia(0, 0, 0);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setOrigin(btVector3(0, 0, 0)); // Position at the origin

	btDefaultMotionState* motionState = new btDefaultMotionState(startTransform);

	btRigidBody::btRigidBodyConstructionInfo rbInfo(mass, motionState, meshShape, inertia);
	btRigidBody* rigidBody = new btRigidBody(rbInfo);

	rigidBody->setCollisionFlags(rigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);

	m_dynamicsWorld->addRigidBody(rigidBody);
}


void GeomipGrid::updateRigidBody()
{
}

void GeomipGrid::createGLState()
{
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

	int POS_LOC = 0;
	int TEX_LOC = 1;
	int NORMAL_LOC = 2;

	size_t numFloats = 0;

	glEnableVertexAttribArray(POS_LOC);
	glVertexAttribPointer(POS_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(numFloats * sizeof(float)));
	numFloats += 3;

	glEnableVertexAttribArray(TEX_LOC);
	glVertexAttribPointer(TEX_LOC, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(numFloats * sizeof(float)));
	numFloats += 2;

	glEnableVertexAttribArray(NORMAL_LOC);
	glVertexAttribPointer(NORMAL_LOC, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*)(numFloats * sizeof(float)));
	numFloats += 3;
}

void GeomipGrid::populateBuffers(const BaseTerrain* pTerrain)
{
	vector<Vertex> vertices;

	vertices.resize(m_width * m_depth);

	initVertices(pTerrain, vertices);

	int numIndices = calcNumIndices();
	vector<unsigned int> indices;
	int numQuads = (m_patchSize - 1) * (m_patchSize - 1);
	indices.resize(numIndices);

	numIndices = initIndices(indices);

	calcNormals(indices, vertices);

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numIndices, &indices[0], GL_STATIC_DRAW);

	createRigidBody(vertices);


}

void GeomipGrid::initVertices(const BaseTerrain* pTerrain, vector<Vertex>& vertices)
{
	int index = 0;

	for (int z = 0; z < m_depth; z++)
	{
		for (int x = 0; x < m_width; x++)
		{
			assert(index < vertices.size());
			
			vertices[index].setVertex(pTerrain, x, z);
			m_convexShape->addPoint(btVector3(vertices[index].pos.x, vertices[index].pos.y, vertices[index].pos.z));
			index++;
		}
	}
}

void GeomipGrid::initInidces(vector<unsigned int>& indices)
{
	int index = 0;

	for (int z = 0; z < m_patchSize - 1; z++)
	{
		for (int x = 0; x < m_patchSize - 1; x++)
		{
			
			unsigned int indexBottomLeft = z * m_width + x;
			unsigned int indexTopLeft = (z + 1) * m_width + x;
			unsigned int indexTopRigth = (z + 1) * m_width + x + 1;
			unsigned int indexBottomRigth = z * m_width + x + 1;

			indices[index++] = indexBottomLeft;
			indices[index++] = indexTopLeft;
			indices[index++] = indexTopRigth;

			indices[index++] = indexBottomLeft;
			indices[index++] = indexTopRigth;
			indices[index++] = indexBottomRigth;
			
		}
	}
}

int GeomipGrid::initIndices(std::vector<unsigned int>& indices)
{
	int index = 0;

	for (int lod = 0; lod <= m_maxLOD; lod++) 
	{
		printf("*** Init indices lod %d ***\n", lod);
		index = initIndicesLOD(index, indices, lod);
		printf("\n");
	}

	return index;

}

int GeomipGrid::initIndicesLOD(int index, std::vector<unsigned int>& indices, int lod)
{
	int totalIndicesForLOD = 0;

	for (int l = 0; l < LEFT; l++) {
		for (int r = 0; r < RIGHT; r++) {
			for (int t = 0; t < TOP; t++) {
				for (int b = 0; b < BOTTOM; b++)
				{
					m_lodInfo[lod].info[l][r][t][b].start = index;
					index = initIndicesLODSingle(index, indices, lod, lod + l, lod + r, lod + t, lod + b);

					m_lodInfo[lod].info[l][r][t][b].count = index - m_lodInfo[lod].info[l][r][t][b].start;
					totalIndicesForLOD += m_lodInfo[lod].info[l][r][t][b].count;
				}
			}
		}
	}

	printf("Total indices for LOD: %d\n", totalIndicesForLOD);

	return index;
}

int GeomipGrid::initIndicesLODSingle(int index, std::vector<unsigned int>& indices, int lodCore, int lodLeft, int lodRight, int lodTop, int lodBottom)
{
	int fanStep = pow(2, lodCore + 1);   // lod = 0 --> 2, lod = 1 --> 4, lod = 2 --> 8, etc
	int endPos = m_patchSize - 1 - fanStep;  // patch size 5, fan step 2 --> EndPos = 2; patch size 9, fan step 2 --> EndPos = 6

	for (int z = 0; z <= endPos; z += fanStep) {
		for (int x = 0; x <= endPos; x += fanStep) {
			int lLeft = x == 0 ? lodLeft : lodCore;
			int lRight = x == endPos ? lodRight : lodCore;
			int lBottom = z == 0 ? lodBottom : lodCore;
			int lTop = z == endPos ? lodTop : lodCore;

			index = createTriangleFan(index, indices, lodCore, lLeft, lRight, lTop, lBottom, x, z);
		}
	}

	return index;
}

void GeomipGrid::calcNormals(vector<unsigned int>& indices, vector<Vertex>& vertices)
{
	unsigned int index = 0;

	for (unsigned int z = 0; z < m_depth - 1; z += (m_patchSize - 1))
	{
		for (unsigned int x = 0; x < m_width - 1; x += (m_patchSize - 1))
		{
			int baseVertex = z * m_width + x;
			int numIndices = m_lodInfo[0].info[0][0][0][0].count;
			for (unsigned int i = 0; i < numIndices; i += 3)
			{
				unsigned int index0 = baseVertex + indices[i];
				unsigned int index1 = baseVertex + indices[i + 1];
				unsigned int index2 = baseVertex + indices[i + 2];

				Vector3f v1 = vertices[index1].pos - vertices[index0].pos;
				Vector3f v2 = vertices[index2].pos - vertices[index0].pos;
				Vector3f normal = v1.cross(v2);
				normal = normal.normalize();

				vertices[index0].normal += normal;
				vertices[index1].normal += normal;
				vertices[index2].normal += normal;
			}
		}
		
	}

	for (unsigned int i = 0; i < vertices.size(); i++)
		vertices[i].normal.normalize();
}

unsigned int GeomipGrid::addTriangle(unsigned int index, vector<unsigned int>& indices, unsigned int v1, unsigned int v2, unsigned int v3)
{
	assert(index < indices.size());
	indices[index++] = v1;
	assert(index < indices.size());
	indices[index++] = v2;
	assert(index < indices.size());
	indices[index++] = v3;

	return index;
}

unsigned int GeomipGrid::createTriangleFan(int index, std::vector<unsigned int>& indices, int lodCore, int lodLeft, int lodRight, int lodTop, int lodBottom, int x, int z)
{
	int stepLeft = pow(2, lodLeft); // because LOD starts at zero...
	int stepRight = pow(2, lodRight);
	int stepTop = pow(2, lodTop);
	int stepBottom = pow(2, lodBottom);
	int stepCenter = pow(2, lodCore);

	unsigned int indexCenter = (z + stepCenter) * m_width + x + stepCenter;

	// first up
	unsigned int indexTemp1 = z * m_width + x;
	unsigned int indexTemp2 = (z + stepLeft) * m_width + x;

	index = addTriangle(index, indices, indexCenter, indexTemp1, indexTemp2);

	// second up
	if (lodLeft == lodCore) {
		indexTemp1 = indexTemp2;
		indexTemp2 += stepLeft * m_width;

		index = addTriangle(index, indices, indexCenter, indexTemp1, indexTemp2);
	}

	// first right
	indexTemp1 = indexTemp2;
	indexTemp2 += stepTop;

	index = addTriangle(index, indices, indexCenter, indexTemp1, indexTemp2);

	// second right
	if (lodTop == lodCore) {
		indexTemp1 = indexTemp2;
		indexTemp2 += stepTop;

		index = addTriangle(index, indices, indexCenter, indexTemp1, indexTemp2);
	}

	// first down
	indexTemp1 = indexTemp2;
	indexTemp2 -= stepRight * m_width;

	index = addTriangle(index, indices, indexCenter, indexTemp1, indexTemp2);

	// second down
	if (lodRight == lodCore) {
		indexTemp1 = indexTemp2;
		indexTemp2 -= stepRight * m_width;

		index = addTriangle(index, indices, indexCenter, indexTemp1, indexTemp2);
	}

	// first left
	indexTemp1 = indexTemp2;
	indexTemp2 -= stepBottom;

	index = addTriangle(index, indices, indexCenter, indexTemp1, indexTemp2);

	// second left
	if (lodBottom == lodCore) {
		indexTemp1 = indexTemp2;
		indexTemp2 -= stepBottom;

		index = addTriangle(index, indices, indexCenter, indexTemp1, indexTemp2);
	}

	return index;
}

int GeomipGrid::calcNumIndices()
{
	int numQuads = (m_patchSize - 1) * (m_patchSize - 1);
	int numIndices = 0;
	int maxPermutationPerLevel = 16;
	const int indicesPerQuad = 6;

	for (int lod = 0; lod <= m_maxLOD; lod++)
	{
		numIndices += numQuads * indicesPerQuad * maxPermutationPerLevel;
		numQuads /= 4;
	}

	return numIndices;

}

bool GeomipGrid::isPatchInsideViewFrustrum_ViewSpace(int x, int z, const Matrix4f& viewProj)
{
	int x0 = x;
	int x1 = x + m_patchSize - 1;
	int z0 = z;
	int z1 = z + m_patchSize - 1;

	Vector3f p00((float)x0 * m_worldScale, m_pTerrain->getHeight(x0, z0), (float)z0 * m_worldScale);
	Vector3f p01((float)x0 * m_worldScale, m_pTerrain->getHeight(x0, z1), (float)z1 * m_worldScale);
	Vector3f p10((float)x1 * m_worldScale, m_pTerrain->getHeight(x1, z0), (float)z0 * m_worldScale);
	Vector3f p11((float)x1 * m_worldScale, m_pTerrain->getHeight(x1, z1), (float)z1 * m_worldScale);

	bool insideViewFrustrum =
		isPointInsideViewFrustrum(p00, viewProj) ||
		isPointInsideViewFrustrum(p01, viewProj) ||
		isPointInsideViewFrustrum(p10, viewProj) ||
		isPointInsideViewFrustrum(p11, viewProj);

	return insideViewFrustrum;
}

bool GeomipGrid::isPointInsideViewFrustrum(const Vector3f& p, const Matrix4f& VP)
{
	Vector4f p4D(p.x, p.y, p.z, 1.0f);

	Vector4f clipSpaceP = VP * p4D;

	bool insideViewFrustrum = ((clipSpaceP.x <= clipSpaceP.w) &&
							   (clipSpaceP.x >= -clipSpaceP.w) &&
							   (clipSpaceP.y <= clipSpaceP.w) &&
							   (clipSpaceP.y >= -clipSpaceP.w) &&
							   (clipSpaceP.z <= clipSpaceP.w) &&
							   (clipSpaceP.z >= -clipSpaceP.w));
	return insideViewFrustrum;
}

bool GeomipGrid::isPatchInsideViewFrustum_WorldSpace(int x, int z, const FrustumCulling& fc)
{
	int x0 = x;
	int x1 = x + m_patchSize - 1;
	int z0 = z;
	int z1 = z + m_patchSize - 1;

	float h00 = m_pTerrain->getHeight(x0, z0);
	float h01 = m_pTerrain->getHeight(x0, z1);
	float h10 = m_pTerrain->getHeight(x1, z0);
	float h11 = m_pTerrain->getHeight(x1, z1);

	float MinHeight = std::min(h00, std::min(h01, std::min(h10, h11)));
	float MaxHeight = std::max(h00, std::max(h01, std::max(h10, h11)));

	Vector3f p00_low((float)x0 * m_worldScale, MinHeight, (float)z0 * m_worldScale);
	Vector3f p01_low((float)x0 * m_worldScale, MinHeight, (float)z1 * m_worldScale);
	Vector3f p10_low((float)x1 * m_worldScale, MinHeight, (float)z0 * m_worldScale);
	Vector3f p11_low((float)x1 * m_worldScale, MinHeight, (float)z1 * m_worldScale);

	Vector3f p00_high((float)x0 * m_worldScale, MaxHeight, (float)z0 * m_worldScale);
	Vector3f p01_high((float)x0 * m_worldScale, MaxHeight, (float)z1 * m_worldScale);
	Vector3f p10_high((float)x1 * m_worldScale, MaxHeight, (float)z0 * m_worldScale);
	Vector3f p11_high((float)x1 * m_worldScale, MaxHeight, (float)z1 * m_worldScale);

	bool insideViewFrustm =
		fc.isPointInsideViewFrustum(p00_low) ||
		fc.isPointInsideViewFrustum(p01_low) ||
		fc.isPointInsideViewFrustum(p10_low) ||
		fc.isPointInsideViewFrustum(p11_low) ||
		fc.isPointInsideViewFrustum(p00_high) ||
		fc.isPointInsideViewFrustum(p01_high) ||
		fc.isPointInsideViewFrustum(p10_high) ||
		fc.isPointInsideViewFrustum(p11_high);

	return insideViewFrustm;
}

void Vertex::setVertex(const BaseTerrain* pTerrain, int x, int z)
{
	float y = pTerrain->getHeight(x, z);

	float worldScale = pTerrain->getWorldScale();
	pos = Vector3f(x * worldScale, y, z * worldScale);

	float size = (float)pTerrain->getSize();
	float textureScale = pTerrain->getTextureScale();

	tex = Vector2f(textureScale * (float)x / size, textureScale * (float)z / size);
}
