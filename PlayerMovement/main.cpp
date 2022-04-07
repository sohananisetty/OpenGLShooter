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

#include <iostream>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void bulletHandler(vector<Bullet>& bullets, vector<Enemy>& objects, Shader shader, float deltaTime);
void playerCollisionCheck(Player player, vector<Enemy>& objects);
void playerCollisionCheck(Player& player, vector<GameObject>& objects, float deltaTime);
glm::vec3 spawnPosition(glm::vec3 playerPosition);

Settings settings;
// window
const float toRadians = 3.14159265f / 180.0f;
Window mainWindow;


enum class GameState {

    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN
};


// camera
glm::vec3 cameraOffset = glm::vec3(0.0f, 3.0f, 3.0f);
Camera camera(cameraOffset);

//projection matrices
glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)settings.SCR_WIDTH / (float)settings.SCR_HEIGHT, 0.1f, 100.0f);
glm::mat4 orthoProjection = glm::ortho(0.0f, (float)settings.SCR_WIDTH, (float)settings.SCR_HEIGHT, 0.0f);

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastEnemyFrame = 1.0f;

glm::vec3 lightPosition = glm::vec3(0.0, 10.0, 0.0);
glm::vec3 lightColour = glm::vec3(1.0, 1.0, 1.0);

float crosshairRadius = 300.0f;

GameState state = GameState::GAME_MENU;

int main()
{
    GameLevel level;
    mainWindow = Window(settings.SCR_WIDTH, settings.SCR_HEIGHT);
    mainWindow.intitialise();
    stbi_set_flip_vertically_on_load(true);


    // build and compile shaders
    // -------------------------
    Shader basicShader("Shaders/vertex_shader.vs", "Shaders/basic_fragment_shader.fs");
    Shader shader("Shaders/vertex_shader.vs", "Shaders/fragment_shader.fs");
    Shader animationShader("Shaders/vertex_shader_animation.vs", "Shaders/fragment_shader_animation.fs");
    Shader textShader("Shaders/vertex_shader_text.vs", "Shaders/fragment_shader_text.fs");

    Shader spriteShader("Shaders/sprite_vertex_shader.vs", "Shaders/sprite_fragment_shader.fs");
    Texture2D crosshair("cross2.png", "C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/sprites", false);
    Texture2D menue("menue.jpg", "C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/sprites", false);

    SpriteRenderer spriteRenderer(spriteShader);


    // load models
    // -----------
    Model floorModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/floor/floor.obj"), settings.TERRAIN_SCALE * 2);
    // Model envModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/Env/environment.obj"));

    Model boxModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/box/box.obj"));

    Model bulletModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/bullets/bullet.obj"));

    Model zombieModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/Mutant/Mutant.fbx"));

    Animation zombieRunAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/Mutant/Mutant.fbx", &zombieModel);
    Animation zombieMeleeAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/Mutant/MeeleCombo.fbx", &zombieModel);

    Animator zombieAnimator(&zombieRunAnimation);


    Model playerModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/Player.fbx"));

    Animation playerRunAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/RunForward.fbx", &playerModel);
    Animation playerRunBackAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/RunBackward.fbx", &playerModel);
    Animation playerIdleAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/Idle.fbx", &playerModel);
    Animation playerRunLeftAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/RunLeft.fbx", &playerModel);
    Animation playerRunRightAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/RunRight.fbx", &playerModel);

    Animator playerAnimator(&playerIdleAnimation);

    //Init models

    TextRenderer Text(textShader , orthoProjection);
    Text.Load("Fonts/arialbd.ttf", 96);

    level.Load("level1.json");

    GameObject floor(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(settings.TERRAIN_SCALE));
    floor.LinkMesh(floorModel);

    Player player;
    player.objectSize = glm::vec3(settings.PLAYER_SCALE);
    player.LinkMesh(playerModel);


    GameObject box(player.objectPosition);
    box.LinkMesh(boxModel);

    bool flagAnim = false;

    vector<Bullet> bullets;
    vector<Enemy> enemies;


    // render loop
    // -----------
    while (!mainWindow.getShouldClose())

    {

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (state == GameState::GAME_MENU) {

            spriteShader.use();
            spriteShader.setMat4("projection", orthoProjection);
            spriteRenderer.DrawSprite(menue, glm::vec2(0.0f,0.0f), glm::vec2(settings.SCR_WIDTH, settings.SCR_HEIGHT), 180.0f, glm::vec3(1.0f, 1.0f, 1.0f));
            
            Text.RenderText("Demon Slayer", 60.0f, 50.0f, 1.0f);
            Text.RenderText("Press enter to start!", 200.0f,200.0f, 0.25f);

            if (mainWindow.getsKeys()[GLFW_KEY_ENTER]) {
                state = GameState::GAME_ACTIVE;
            }
        }

        else if (state == GameState::GAME_ACTIVE) {


            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;



            // input
            // -----

            player.ProcessKeyboard(mainWindow.getsKeys(), deltaTime);
            player.ProcessMouseMovement(mainWindow.getXChange());

            playerCollisionCheck(player, enemies);
            playerCollisionCheck(player, level.objects, deltaTime);

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
            //else if (player.currentMovement == Object_Movement::STILL && player.lastMovement == Object_Movement::STILL) {
            //    playerAnimator.NewAnimation(&playerIdleAnimation);

            //}

            zombieAnimator.UpdateAnimation(deltaTime);
            playerAnimator.UpdateAnimation(deltaTime);


            animationShader.use();
            animationShader.setBool("useLight", false);
            animationShader.setMat4("projection", projection);

            // render the Player model

            animationShader.setMat4("model", player.GetModelMatrix(settings.PLAYER_YAW_OFFSET));
            glm::vec3 camPos = cameraOffset + player.objectPosition;
            glm::mat4 view = camera.GetViewMatrix(camPos, player.objectPosition, camera.cameraUp);
            animationShader.setMat4("view", view);



            auto transforms = playerAnimator.GetFinalBoneMatrices();
            for (int i = 0; i < transforms.size(); ++i)
                animationShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

            player.Draw(animationShader);



            // generate Enemies at locations

            if (currentFrame - lastEnemyFrame >= settings.SPAWN_DELAY) {

                //std::cout << "spawn\n";
                lastEnemyFrame = currentFrame;
                Enemy enemy(spawnPosition(glm::vec3(player.objectPosition.x, 0.0f, player.objectPosition.z)), settings.ENEMY_SCALE, settings.ENEMY_SPEED);
                enemy.LinkMesh(zombieModel);
                enemies.push_back(enemy);

            }


            //Render enemies


            for (int i = 0; i < enemies.size(); i++) {
                glm::vec3  enemyCurrentPosition = enemies[i].Move(deltaTime, glm::vec3(player.objectPosition.x, 0.0f, player.objectPosition.z));
                animationShader.setMat4("model", enemies[i].GetModelMatrix(enemyCurrentPosition, settings.ENEMY_YAW_OFFSET));


                auto transforms = zombieAnimator.GetFinalBoneMatrices();
                for (int i = 0; i < transforms.size(); ++i)
                    animationShader.setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);



                enemies[i].Draw(animationShader);
            }

            //Environment

            shader.use();
            shader.setMat4("projection", projection);
            shader.setMat4("view", view);

            shader.setVec3("lightPosition", glm::vec3(camPos.x, camPos.y + 10.0f, camPos.z + 10.0f));
            shader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
            shader.setVec3("light.diffuse", 0.8f, 0.8f, 0.8f);
            shader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
            shader.setBool("useTex", true);

            shader.setMat4("model", floor.GetModelMatrix());
            floor.Draw(shader);

            if (mainWindow.mouseLeftClick) {
                glm::vec3 bulletCurrentPos = glm::vec3(player.objectPosition.x, 0.55f + player.objectPosition.y, player.objectPosition.z);
                Bullet bullet(bulletCurrentPos, settings.BULLET_SCALE, settings.BULLET_SPEED, player.yaw);
                bullet.LinkMesh(bulletModel);
                bullets.push_back(bullet);
                mainWindow.mouseLeftClick = false;

            }

            //Check collisions

            bulletHandler(bullets, enemies, shader, deltaTime);

            level.Draw(shader);


            // 2D stuff

            spriteShader.use();
            spriteShader.setMat4("projection", orthoProjection);
            glm::vec2 currentCursorPos = glm::vec2(player.objectPosition.x + player.objectFront.x * crosshairRadius + (float)settings.SCR_WIDTH / 2, player.objectPosition.z + player.objectFront.z * crosshairRadius + (float)settings.SCR_HEIGHT / 2);
            glm::vec4 mod = view * glm::vec4(currentCursorPos.x, 0.0f, currentCursorPos.y, 1.0f);
            spriteRenderer.DrawSprite(crosshair, glm::vec2(mod.x, mod.z), glm::vec2(30.0f, 30.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

            /* std::cout << "player "<< player.objectPosition.y<<"\n";
             std::cout << "x1:" << player.objectPosition.x + player.boundingBox[0] << "\n";
             std::cout << "x2:" << player.objectPosition.x - glm::abs(player.boundingBox[1]) << "\n";

             std::cout << "z1:" << player.objectPosition.z + player.boundingBox[4] << "\n";
             std::cout << "z2:" << player.objectPosition.z - glm::abs(player.boundingBox[5]) << "\n";

             std::cout << "y1:" << player.objectPosition.y + player.boundingBox[2] << "\n";
             std::cout << "y2:" << player.objectPosition.y - glm::abs(player.boundingBox[3]) << "\n";*/




        }
        // -------------------------------------------------------------------------------
        mainWindow.swapBuffers();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

void bulletHandler(vector<Bullet>& bullets, vector<Enemy>& objects, Shader shader, float deltaTime) {

    for (int i = 0; i < (int)bullets.size(); i++) {

        for (int j = 0; j < (int)objects.size(); j++) {

            if (bullets[i].CheckCollision(objects[j])) {
                std::cout << "bullet collision\n";
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
            std::cout << "collision\n";
            //state = GameState::GAME_MENU;
            break;
        }
    }

}

void playerCollisionCheck(Player& player, vector<GameObject>& objects, float deltaTime) {


    for (int j = 0; j < (int)objects.size(); j++) {

        if (player.CheckCollision(objects[j])) {
            std::cout << "collision\n";
            float distance = player.objectSpeed * deltaTime;

            if (player.lastMovement == Object_Movement::FORWARD)
                player.objectPosition -= player.objectFront * distance;

            else if (player.lastMovement == Object_Movement::BACKWARD)
                player.objectPosition += player.objectFront * distance;

            else if (player.lastMovement == Object_Movement::LEFT)
                player.objectPosition += player.objectRight * distance;

            else if (player.lastMovement == Object_Movement::RIGHT)
                player.objectPosition -= player.objectRight * distance;

            else if (player.lastMovement == Object_Movement::JUMP) {

                player.objectPosition.y -= player.currentUpSpeed * deltaTime;

                //player.objectPosition.y += GRAVITY * deltaTime;


            }

            //state = GameState::GAME_MENU;
            break;
        }
    }

}

glm::vec3 spawnPosition(glm::vec3 playerPosition) {

    srand(time(0));


    float randomX = (rand() % static_cast<int>(2 * settings.SPAWN_RADIUS_MAX)) + playerPosition.x;
    float randomZ = (rand() % static_cast<int>(2 * settings.SPAWN_RADIUS_MAX)) + playerPosition.z;
    float sign = -1 + (rand() % static_cast<int>(3));
    return glm::vec3(sign * randomX + playerPosition.x + settings.SPAWN_RADIUS_MIN, playerPosition.y, sign * randomZ + playerPosition.z + settings.SPAWN_RADIUS_MIN);

}
