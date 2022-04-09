#include "Terrain.h"



Terrain::Terrain(int gridX, int gridZ, const char* heightMap , const std::string& directory, vector<Texture2D> textures, int size = 100, int maxHeight = 5, int tilling = 1)
	:size(size), maxHeight(maxHeight), tilling(tilling), x(gridX* size), z(gridZ* size)
{
    std::string filename = std::string(heightMap);
    std::string path = directory + '/' + filename;

    int nrComponents;
    unsigned char* data = stbi_load(path.c_str(), &heightmapWidth, &heightmapHeight, &nrComponents, 0);
    if (data)
    {
        this->vertexCount = heightmapHeight;
		heights.resize(this->vertexCount, std::vector<float>(this->vertexCount));
    }
	generateTerrain(textures , data);
}

Terrain::Terrain(int gridX, int gridZ, const char* heightMap, const std::string& directory, int size = 100, int maxHeight = 5, int tilling = 1)
	:size(size), maxHeight(maxHeight) , tilling(tilling), x(gridX*size), z(gridZ* size)
{

	std::string filename = std::string(heightMap);
	std::string path = directory + '/' + filename;

	int nrComponents;
	unsigned char* data = stbi_load(path.c_str(), &heightmapWidth, &heightmapHeight, &nrComponents, 0);
	if (data)
	{
		this->vertexCount = heightmapHeight;
		heights.resize(this->vertexCount, std::vector<float>(this->vertexCount));


	}
	generateTerrain(data);
}

glm::vec3 Terrain::calcNormal(int x,int z ,unsigned char*data){

	float heightL = getHeight(x - 1, z, data, 3);
	float heightR = getHeight(x + 1, z, data, 3);
	float heightD = getHeight(x, z-1, data, 3);
	float heightU = getHeight(x, z+1, data, 3);
	glm::vec3 normal = glm::normalize(glm::vec3(heightL - heightR, 2.0f, heightD - heightU));
	return normal;


}

float Terrain::getHeight(int x, int z, unsigned char* data,int channelCount = 3) {

	if (x<0 || x>=heightmapWidth || z<0 || z>=heightmapHeight)
		return 0.0f;

	unsigned char* pixelOffset = data + (x + heightmapWidth * z) * channelCount;
	unsigned char r = pixelOffset[0];
	unsigned char g = pixelOffset[1];
	unsigned char b = pixelOffset[2];

	//std::cout << (float)r<< " " << (float)g <<" " << (float)b << "\n";
	float height = r;
	height = (height / 256) * maxHeight;

	return height;



}

float Terrain::getTerrainHeight(float worldX, float worldZ) {

	float terrainX = worldX - this->x;
	float terrainZ = worldZ - this->z;
	float gridSquareSize = this->size / ((float) heights.size() - 1);
	int gridX = (int)floor(terrainX / gridSquareSize);
	int gridZ = (int)floor(terrainZ / gridSquareSize);

	if (gridX >= heights.size() - 1 || gridZ >= heights.size() - 1 || gridX < 0.0 || gridZ < 0.0f)
		return 0.0f;

	float xCoord = std::fmod(terrainX , gridSquareSize)/gridSquareSize;
	float zCoord = std::fmod(terrainZ, gridSquareSize) / gridSquareSize;
	float answer;

	if (xCoord <= (1 - zCoord)) {
		answer = 
			barryCentric(glm::vec3(0, heights[gridX][gridZ], 0.0f), glm::vec3(1.0f,
				heights[gridX + 1][gridZ], 0.0f), glm::vec3(0.0f,
					heights[gridX][gridZ + 1], 1.0f), glm::vec2(xCoord, zCoord));
	}
	else {
		answer =
			barryCentric(glm::vec3(0, heights[gridX+1][gridZ], 0.0f), glm::vec3(1.0f,
				heights[gridX + 1][gridZ+1],1.0f), glm::vec3(0.0f,
					heights[gridX][gridZ + 1], 1.0f), glm::vec2(xCoord, zCoord));
	}

	return answer;

}

void Terrain::generateTerrain(vector<Texture2D> textures , unsigned char* heightMap)
{


		int count = vertexCount*vertexCount;
		//std::cout << count << "\n";
		std::vector<Vertex> meshVertices(count);
		glm::vec3 vector;

		std::vector<float> vertices(count * 3);
		std::vector<float> normals(count * 3);
		std::vector<float> textureCoords(count * 2);
		std::vector<unsigned int> indices(6 * (vertexCount - 1) * (vertexCount - 1));


		int vertexPointer = 0;
		for (int i = 0; i < vertexCount; i++) {
			for (int j = 0; j < vertexCount; j++) {
				
				vector.x = (float)j / ((float)vertexCount - 1) * size;
				vector.z = (float)i / ((float)vertexCount - 1) * size;
				float height = getHeight(j, i, heightMap, 3);
				vector.y = height;
				heights[j][i] = height;

				meshVertices[vertexPointer].Position = vector;
				meshVertices[vertexPointer].Normal = calcNormal(j,i,heightMap);
				
				meshVertices[vertexPointer].TexCoords = glm::vec2( ((float)j / ((float)vertexCount - 1))*tilling , ((float)i / ((float)vertexCount - 1))*tilling);
				
				
				vertexPointer++;
			}
		}


		int pointer = 0;
		for (int gz = 0; gz < vertexCount - 1; gz++) {
			for (int gx = 0; gx < vertexCount - 1; gx++) {
				int topLeft = (gz * vertexCount) + gx;
				int topRight = topLeft + 1;
				int bottomLeft = ((gz + 1) * vertexCount) + gx;
				int bottomRight = bottomLeft + 1;
				indices[pointer++] = topLeft;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = topRight;
				indices[pointer++] = topRight;
				indices[pointer++] = bottomLeft;
				indices[pointer++] = bottomRight;
			}
		}

		mesh = Mesh(meshVertices, indices , textures);

	}

void Terrain::generateTerrain(unsigned char* heightMap)
{


	int count = vertexCount * vertexCount;
	//std::cout << count << "\n";
	std::vector<Vertex> meshVertices(count);
	glm::vec3 vector;

	std::vector<float> vertices(count * 3);
	std::vector<float> normals(count * 3);
	std::vector<float> textureCoords(count * 2);
	std::vector<unsigned int> indices(6 * (vertexCount - 1) * (vertexCount - 1));


	int vertexPointer = 0;
	for (int i = 0; i < vertexCount; i++) {
		for (int j = 0; j < vertexCount; j++) {

			vector.x = (float)j / ((float)vertexCount - 1) * size;
			vector.z = (float)i / ((float)vertexCount - 1) * size;
			vector.y = getHeight(j, i, heightMap, 3);
			meshVertices[vertexPointer].Position = vector;

			meshVertices[vertexPointer].Normal = calcNormal(j, i, heightMap);

			meshVertices[vertexPointer].TexCoords = glm::vec2(((float)j / ((float)vertexCount - 1)) * tilling, ((float)i / ((float)vertexCount - 1)) * tilling);


			vertexPointer++;
		}
	}


	int pointer = 0;
	for (int gz = 0; gz < vertexCount - 1; gz++) {
		for (int gx = 0; gx < vertexCount - 1; gx++) {
			int topLeft = (gz * vertexCount) + gx;
			int topRight = topLeft + 1;
			int bottomLeft = ((gz + 1) * vertexCount) + gx;
			int bottomRight = bottomLeft + 1;
			indices[pointer++] = topLeft;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = topRight;
			indices[pointer++] = topRight;
			indices[pointer++] = bottomLeft;
			indices[pointer++] = bottomRight;
		}
	}

	mesh = Mesh(meshVertices, indices);

}


float Terrain::barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;


}