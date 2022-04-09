#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb_image.h>
#include <assimp/texture.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

// Texture2D is able to store and configure a texture in OpenGL.
// It also hosts utility functions for easy management.
enum class TextureType { DIFFUSE, SPECULAR, NORMAL, HEIGHT };

class Texture2D
{
public:
    // holds the ID of the texture object, used for all texture operations to reference to this particlar texture
    unsigned int ID;
    // texture image dimensions
    int Width, Height; // width and height of loaded image in pixels  
    TextureType type;
//    unsigned char* data;
    std::string path; // we store the path of the texture to compare with other textures
    // texture configuration
    unsigned int Wrap_S; // wrapping mode on S axis
    unsigned int Wrap_T; // wrapping mode on T axis
    unsigned int Filter_Min; // filtering mode if texture pixels < screen pixels
    unsigned int Filter_Max; // filtering mode if texture pixels > screen pixels
    // constructor (sets default texture modes)
    Texture2D();
    Texture2D(const char* path, const std::string& directory, bool flip);

    Texture2D(const aiTexture* texture, bool flip);

};

#endif