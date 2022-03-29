#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h> 

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "assimp_glm_helpers.h"

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include <vector>
#include <limits>

using namespace std;

struct BoneInfo
{
    /*id is index in finalBoneMatrices*/
    int id;

    /*offset matrix transforms vertex from model space to bone space*/
    glm::mat4 offset;

};

class Model
{
public:
    // model data 
    vector<Texture2D> textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh> meshes;
    string directory;
    bool gammaCorrection;
    float tilingFactor;
    std::vector<float> boundingBox;

   

    auto& GetBoneInfoMap() { return m_BoneInfoMap; }
    int& GetBoneCount() { return m_BoneCounter; }

    Model(){}
    // constructor, expects a filepath to a 3D model.
    Model(string const& path, float textureTilingFactor = 1.0f , bool gamma = false) : gammaCorrection(gamma) , tilingFactor(textureTilingFactor)
    {
        boundingBox = { std::numeric_limits<float>::min() , std::numeric_limits<float>::max(),std::numeric_limits<float>::min(),std::numeric_limits<float>::max(),std::numeric_limits<float>::min() ,std::numeric_limits<float>::max() };
        loadModel(path);

    }

    // draws the model, and thus all its meshes
    void Draw(Shader& shader)
    {
        for (unsigned int i = 0; i < meshes.size(); i++)
            meshes[i].Draw(shader);
    }

private:

    std::map<string, BoneInfo> m_BoneInfoMap; //
    int m_BoneCounter = 0;

    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(string const& path);

    
    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
  
    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture2D> loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName , const aiScene* scene);
 

    void SetVertexBoneDataToDefault(Vertex& vertex);
    void SetVertexBoneData(Vertex& vertex, int boneID, float weight);

    void ExtractBoneWeightForVertices(std::vector<Vertex>& vertices, aiMesh* mesh, const aiScene* scene);

};

#endif