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

#include <iostream>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bulletHandler(vector<Bullet>& bullets, vector<Enemy>&objects , Shader shader, float deltaTime);
void playerCollisionCheck(Player player, vector<Enemy>& objects);
glm::vec3 spawnPosition(glm::vec3 playerPosition);
// window
const float toRadians = 3.14159265f / 180.0f;
Window mainWindow;

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
const float BULLET_SPEED = 20.0f;
const float BULLET_SCALE = 0.05f;

const float PLAYER_SCALE = 0.005f;
const float PLAYER_YAW_OFFSET = 90.0f;

const float ENEMY_SCALE = 0.007f;
const float ENEMY_SPEED = 2.0f;
const float SPAWN_RADIUS_MAX = 10.0f;
const float SPAWN_RADIUS_MIN = 2.0f;
const float SPAWN_DELAY = 2.0f;

const float ENEMY_YAW_OFFSET = 90.0f;

const float TERRAIN_HEIGHT = 0.0f;
const float TERRAIN_SCALE = 40.0f;

// camera
glm::vec3 cameraOffset = glm::vec3(0.0f, 3.0f, 3.0f);
Camera camera(cameraOffset);

//projection matrices
glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
glm::mat4 orthoProjection = glm::ortho(0.0f, (float)SCR_WIDTH, (float)SCR_HEIGHT, 0.0f, -1.0f, 1.0f);


// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastEnemyFrame = 1.0f;


float crosshairRadius = 300.0f;


int main()
{
    mainWindow = Window(800, 600);
    mainWindow.intitialise();

    stbi_set_flip_vertically_on_load(true);


    // build and compile shaders
    // -------------------------

    Shader shader("Shaders/vertex_shader.vs", "Shaders/fragment_shader.fs");
    Shader animationShader("Shaders/vertex_shader_animation.vs", "Shaders/fragment_shader_animation.fs");

    Shader spriteShader("Shaders/sprite_vertex_shader.vs", "Shaders/sprite_fragment_shader.fs");
    Texture2D crosshair("cross2.png", "C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/sprites", false);
    SpriteRenderer spriteRenderer(spriteShader);


    // load models
    // -----------
    Model floorModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/floor/floor.obj") , TERRAIN_SCALE*2);
    Model bulletModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/bullets/bullet.obj"));
    
    Model zombieModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/Mutant/Mutant.fbx"));
    Animation zombieRunAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/Mutant/Mutant.fbx", &zombieModel);
    Animator zombieAnimator(&zombieRunAnimation);


    Model playerModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/Player.fbx"));

    Animation playerRunAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/RunForward.fbx", &playerModel);
    Animation playerRunBackAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/RunBackward.fbx", &playerModel);
    Animation playerIdleAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/Idle.fbx", &playerModel);
    Animation playerRunLeftAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/RunLeft.fbx", &playerModel);
    Animation playerRunRightAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/RunRight.fbx",&playerModel);

    Animator playerAnimator(&playerIdleAnimation);
    
    //Init models

    GameObject floor(glm::vec3(0.0f, 0.0f, 0.0f) , TERRAIN_SCALE);
    floor.LinkMesh(floorModel);

    Player player;
    player.LinkMesh(playerModel);
    player.objectSize = PLAYER_SCALE;

    bool flagAnim = false;

    vector<Bullet> bullets;
    vector<Enemy> enemies;

   
    // render loop
    // -----------
    while (!mainWindow.getShouldClose())

    {

        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;


       
        // input
        // -----

        player.ProcessKeyboard(mainWindow.getsKeys() , deltaTime);
        player.ProcessMouseMovement(mainWindow.getXChange());

        if (player.currentMovement == Object_Movement::FORWARD && player.lastMovement != player.currentMovement) {
            playerAnimator.NewAnimation(&playerRunAnimation);


        }
        else if (player.currentMovement == Object_Movement::BACKWARD && player.lastMovement != player.currentMovement) {
            playerAnimator.NewAnimation(&playerRunBackAnimation);


        }
       
        
        else if (player.currentMovement == Object_Movement::RIGHT && player.lastMovement != player.currentMovement) {
            playerAnimator.NewAnimation(&playerRunRightAnimation);


        }
        else if (player.currentMovement == Object_Movement::LEFT && player.lastMovement != player.currentMovement) {
            playerAnimator.NewAnimation(&playerRunLeftAnimation);

        }

        zombieAnimator.UpdateAnimation(deltaTime);
        playerAnimator.UpdateAnimation(deltaTime);
        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        animationShader.use();
        animationShader.setBool("useLight", false);
        animationShader.setMat4("projection", projection);

        // render the Player model

        animationShader.setMat4("model", player.GetModelMatrix(PLAYER_YAW_OFFSET));
        glm::vec3 camPos = cameraOffset + player.objectPosition;
        glm::mat4 view = camera.GetViewMatrix(camPos, player.objectPosition,camera.cameraUp);
        animationShader.setMat4("view", view);



        auto transforms = playerAnimator.GetFinalBoneMatrices();
        for (int i = 0; i < transforms.size(); ++i)
            animationShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

        player.Draw(animationShader);

        

        // generate Enemies locations

        if (currentFrame - lastEnemyFrame >= SPAWN_DELAY) {

            std::cout << "spawn\n";
            lastEnemyFrame = currentFrame;
            Enemy enemy(spawnPosition(player.objectPosition), ENEMY_SCALE, ENEMY_SPEED);
            enemy.LinkMesh(zombieModel);
            enemies.push_back(enemy);

        }


        //Render enemies

     
        for (int i = 0; i < enemies.size(); i++) {
            glm::vec3  enemyCurrentPosition = enemies[i].Move(deltaTime, player.objectPosition);
            animationShader.setMat4("model", enemies[i].GetModelMatrix(enemyCurrentPosition , ENEMY_YAW_OFFSET));
            auto transforms = zombieAnimator.GetFinalBoneMatrices();
            for (int i = 0; i < transforms.size(); ++i)
                animationShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

            enemies[i].Draw(animationShader);
        }

        shader.use();
        shader.setBool("useLight", false);
        shader.setMat4("projection", projection);
        shader.setMat4("view", view);

        //Environment

        shader.setMat4("model", floor.GetModelMatrix());
        floor.Draw(shader);

        if (mainWindow.mouseLeftClick) {
            glm::vec3 bulletCurrentPos = glm::vec3(player.objectPosition.x + 0.01 * player.objectFront.x, 0.55f + player.objectPosition.y, player.objectPosition.z + 0.01 * player.objectFront.z);
            Bullet bullet(bulletCurrentPos, BULLET_SCALE, BULLET_SPEED, player.yaw);
            bullet.LinkMesh(bulletModel);
            bullets.push_back(bullet);
            mainWindow.mouseLeftClick = false;

        }

        //Check collisions

        bulletHandler(bullets, enemies, shader, deltaTime);
        playerCollisionCheck(player, enemies);


        // 2D stuff

        spriteShader.use();
        spriteShader.setMat4("projection", orthoProjection);
        glm::vec2 currentCursorPos = glm::vec2(player.objectPosition.x + player.objectFront.x * crosshairRadius + (float)SCR_WIDTH / 2, player.objectPosition.z + player.objectFront.z * crosshairRadius + (float)SCR_HEIGHT / 2);
        glm::vec4 mod = view*glm::vec4(currentCursorPos.x, 0.0f, currentCursorPos.y, 1.0f);
        spriteRenderer.DrawSprite(crosshair, glm::vec2(mod.x, mod.z), glm::vec2(30.0f, 30.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));
       


        // -------------------------------------------------------------------------------
        mainWindow.swapBuffers();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void bulletHandler(vector<Bullet>& bullets, vector<Enemy>& objects , Shader shader, float deltaTime) {

    for (int i = 0; i < (int)bullets.size(); i++) {

        for (int j = 0; j < (int)objects.size(); j++) {

            if (bullets[i].CheckCollision(objects[j])) {
                std::cout << "collision\n";
                bullets[i].destroyed = true;
                objects[j].destroyed = true;
                objects.erase(objects.begin() + j);
                break;
            }
         
        }
        
        if (bullets[i].destroyed) {
            bullets.erase(bullets.begin() + i);
        }
        else {
            glm::vec3  bulletCurrentPosition = bullets[i].Move(deltaTime);

            glm::mat4 model = glm::mat4(1.0f);
            model = bullets[i].GetModelMatrix(bulletCurrentPosition);
            shader.setMat4("model", model);
            bullets[i].Draw(shader);

        }


    }

}


void playerCollisionCheck(Player player, vector<Enemy>& objects) {


    for (int j = 0; j < (int)objects.size(); j++) {

        if (player.CheckCollision(objects[j])) {
            //std::cout << "Game over\n";
            break;
        }
    }

}

glm::vec3 spawnPosition(glm::vec3 playerPosition) {

    srand(time(0));


    float randomX = (rand() % static_cast<int>( 2* SPAWN_RADIUS_MAX)) + playerPosition.x;
    float randomZ = (rand() % static_cast<int>( 2 * SPAWN_RADIUS_MAX)) + playerPosition.z;
    float sign = -1 + (rand() % static_cast<int>(3));
    return glm::vec3(sign*randomX + playerPosition.x + SPAWN_RADIUS_MIN, playerPosition.y, sign* randomZ+ playerPosition.z + SPAWN_RADIUS_MIN);

}
