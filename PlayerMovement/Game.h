#ifndef GAME_H
#define GAME_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include<stb_image.h>

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Window.h"
#include "Bullet.h"
#include "Player.h"
#include "Enemy.h"
#include "SpriteRenderer.h"
#include "Animator.h"
#include "Settings.h"
#include "GameLevel.h"
#include "TextRenderer.h"
#include"Terrain.h"

#include <iostream>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <tuple>
#include<map>

enum class GameState {

    GAME_ACTIVE,
    GAME_MENU,
    GAME_INST,
    GAME_WIN
};

const float toRadians = 3.14159265f / 180.0f;


class Game
{
public:

    GameState state;
    Settings settings;
    GameLevel level;

    Player player;
    GameObject* floorObject;
    Camera* camera;
    Animator* zombieAnimator;


    int score;

    glm::mat4 perspectiveProjection , orthoProjection;
    glm::vec3 cameraOffset;

    //Animations

    std::map<std::string , Animation> animations;
    std::map<std::string, Animator> animators;
    std::map<std::string, Shader> shaders;
    std::map<std::string, Model> models;
    std::map<std::string, GameObject> gameObjects;
    std::map<std::string, Texture2D> textures;



    Game();
    void Init();
    void InitGameObjects();
    void configureLighting(Shader& shader, glm::vec3 spotPos);


    void bulletHandler(vector<Bullet>& bullets, vector<Enemy>& objects, Shader shader, float deltaTime);
    void playerCollisionCheck(Player player, vector<Enemy>& objects);
    void playerCollisionCheck(Player& player, vector<GameObject>& objects, float deltaTime);
    glm::vec3 spawnPosition(glm::vec3 playerPosition);


    ~Game();


private:

    void InitShaders();
    void InitTextures();
    void InitAnimations();
    void InitModels();

   

};

#endif