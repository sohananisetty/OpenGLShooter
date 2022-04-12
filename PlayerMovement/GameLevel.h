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
private:
    
    void parseLevel(json level);
};
