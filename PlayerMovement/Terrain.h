#pragma once
#include "Mesh.h"

#include<vector>

class Terrain
{
public:
	const float size ,maxHeight , x , z;
	const int tilling;

	int vertexCount=2;
	int heightmapWidth, heightmapHeight;
	std::vector<std::vector<float>> heights;
	/*std::vector<std::vector<float>> heights(
		vertexCount,
		std::vector<float>(vertexCount));*/
	Mesh mesh;

	Terrain(int gridX, int gridZ ,const char* heightMap , const std::string& directory, vector<Texture2D> textures, int size, int maxHeight,int tiling);
	Terrain(int gridX, int gridZ, const char* heightMap, const std::string& directory, int size, int maxHeight, int tiling);
	float getTerrainHeight(float worldX, float worldZ);

private:
	void generateTerrain(vector<Texture2D> textures, unsigned char* heightMap);
	void generateTerrain(unsigned char* heightMap);
	float getHeight(int x, int z, unsigned char* data, int channelCount);
	glm::vec3 calcNormal(int x, int z, unsigned char* data);
	float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos);

};

