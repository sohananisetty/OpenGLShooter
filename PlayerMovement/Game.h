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

#include <iostream>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <tuple>


enum GameState {

    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};

const float toRadians = 3.14159265f / 180.0f;


class Game
{
public:

    GameState state;
    Settings settings;


    Game();
    ~Game();

    void InitShaders();
    void Init();
    void InitModels();




private:
    void bulletHandler(vector<Bullet>& bullets, vector<Enemy>& objects, Shader shader, float deltaTime);
    void playerCollisionCheck(Player player, vector<Enemy>& objects);
    glm::vec3 spawnPosition(glm::vec3 playerPosition);
};

#endif