#pragma once
#include<vector>
#include<json.hpp>
#include <cmath>

#include "GameObject.h"
#include "Model.h"


using json = nlohmann::json;

class GameLevel
{
public:
    // level state
    std::vector<GameObject> objects;
    json level;
    // constructor
    GameLevel() { }
    // loads level from file
    void Load(const char* file);
    // render level
    void Draw(Shader& shader);
    // check if the level is completed (all non-solid tiles are destroyed)
    bool IsCompleted();
private:
    
    void parseLevel(json level);
};
