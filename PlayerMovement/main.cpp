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
#include "Game.h"

#include <iostream>
#include <random>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>





Settings settings;
Window mainWindow;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float lastEnemyFrame = 1.0f;
float currentFrame = 0.0f;
float timeOffset = 0.0f;

//glm::vec3 lightPosition = glm::vec3(0.0, 10.0, 0.0);
//glm::vec3 lightColour = glm::vec3(1.0, 1.0, 1.0);


//float crosshairRadius = 300.0f;

int main()
{

    mainWindow = Window(settings.SCR_WIDTH, settings.SCR_HEIGHT);
    mainWindow.intitialise();


    Game gameManager;

    Animator zombieAnimator(&gameManager.animations["zombieRunAnimation"]);
    Animator playerAnimator(&gameManager.animations["playerIdleAnimation"]);

    SpriteRenderer spriteRenderer(gameManager.shaders["spriteShader"]);
    TextRenderer Text(gameManager.shaders["textShader"], gameManager.orthoProjection);
    Text.Load("Fonts/arialbd.ttf", 96);

    gameManager.level.Load("level1.json");

    vector<GameObject> terrainObjects;
    vector <Texture2D> terrainTexures = { gameManager.textures["floorTexture"] };

                            
    Terrain terrain(0,0, "heightmap2.png", "../Assets/sprites", terrainTexures,settings.TERRAIN_SCALE,settings.TERRAIN_HEIGHT,settings.TERRAIN_TILLING);

    vector<Mesh> meshes; meshes.push_back(terrain.mesh);
    Model terrainModel; terrainModel.meshes = meshes;

    GameObject terrainObject(glm::vec3(terrain.x, 0.0f, terrain.z));
    terrainObject.LinkMesh(terrainModel);
    terrainObject.objectColor = glm::vec3(0.5);

    terrainObjects.push_back(terrainObject);

  




    bool flagAnim = false;

    vector<Bullet> bullets;
    vector<Enemy> enemies;


    // render loop
    // -----------
    while (!mainWindow.getShouldClose())

    {

        glClearColor(0.9215f, 0.96078f, 0.9843f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (gameManager.score == settings.WIN_SCORE) {
            gameManager.state = GameState::GAME_WIN;
            gameManager.score = 0;
        }
       



        if (gameManager.state == GameState::GAME_MENU) {

            gameManager.shaders["spriteShader"].use();
            gameManager.shaders["spriteShader"].setMat4("projection", gameManager.orthoProjection);
            spriteRenderer.DrawSprite(gameManager.textures["menueTexture"], glm::vec2(0.0f, 0.0f), glm::vec2(settings.SCR_WIDTH, settings.SCR_HEIGHT), 180.0f, glm::vec3(1.0f, 1.0f, 1.0f));
            
            Text.RenderText("Monster Slayer", settings.SCR_WIDTH/10, 100.0f, settings.SCR_WIDTH/800.0f);
            Text.RenderText("Press enter to start!", settings.SCR_WIDTH / 2.0f - settings.SCR_WIDTH / 4.0f, 100.0f + 96.0f* settings.SCR_WIDTH / 800.0f + 30.0f, 0.33);

            if (mainWindow.getsKeys()[GLFW_KEY_ENTER]) {

                gameManager.state = GameState::GAME_INST;
                mainWindow.keys[GLFW_KEY_ENTER] = false;

            }
        }

        else if (gameManager.state == GameState::GAME_INST) {

            spriteRenderer.DrawSprite(gameManager.textures["controlsTexture"], glm::vec2(50.0f, settings.SCR_HEIGHT - settings.SCR_HEIGHT / 600 * 250.0f - 50), glm::vec2(settings.SCR_WIDTH - 150.0f, settings.SCR_HEIGHT / 600 * 250.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

            Text.RenderText("Instructions", settings.SCR_WIDTH/2.0f - settings.SCR_WIDTH/3.0f, 50.0f, settings.SCR_WIDTH / 1200.0f, glm::vec3(0.0f));
            Text.RenderText("Defeat the monsters to get points.", 50.0f, 50.0f + 96.0f * settings.SCR_WIDTH / 1200.0f + 50.0f, settings.SCR_WIDTH / 2400.0f, glm::vec3(0.0f));
            Text.RenderText("Get " + std::to_string(settings.WIN_SCORE)+" points under " + std::to_string(settings.WIN_TIME/60) + " minutes to win!", 50.0f, 100.0f + 96.0f * settings.SCR_WIDTH / 1200.0f + 96.0f * settings.SCR_WIDTH / 2400.0f + 10.0f, settings.SCR_WIDTH / 2400.0f, glm::vec3(0.0f));

            timeOffset = static_cast<float>(glfwGetTime());

            if (mainWindow.getsKeys()[GLFW_KEY_ENTER] ) {
                gameManager.state = GameState::GAME_ACTIVE;
                mainWindow.keys[GLFW_KEY_ENTER] = false;
                playerAnimator.NewAnimation(&gameManager.animations["playerIdleAnimation"]);
                gameManager.player.objectPosition = glm::vec3(0.0f);
                bullets.clear();
                enemies.clear();
                gameManager.score = 0;
            }

        }

        else if (gameManager.state == GameState::GAME_WIN) {

            gameManager.shaders["spriteShader"].use();
            gameManager.shaders["spriteShader"].setMat4("projection", gameManager.orthoProjection);
            spriteRenderer.DrawSprite(gameManager.textures["menueTexture"], glm::vec2(0.0f, 0.0f), glm::vec2(settings.SCR_WIDTH, settings.SCR_HEIGHT), 180.0f, glm::vec3(1.0f, 1.0f, 1.0f));

            Text.RenderText("You Won!!!", settings.SCR_WIDTH / 2 - settings.SCR_WIDTH / 3, 100.0f, settings.SCR_WIDTH / 800.0f);


            if (mainWindow.getsKeys()[GLFW_KEY_ENTER]) {
                gameManager.state = GameState::GAME_MENU;
                mainWindow.keys[GLFW_KEY_ENTER] = false;

            }
        }

        else if (gameManager.state == GameState::GAME_LOSE) {

            gameManager.shaders["spriteShader"].use();
            gameManager.shaders["spriteShader"].setMat4("projection", gameManager.orthoProjection);
            spriteRenderer.DrawSprite(gameManager.textures["menueTexture"], glm::vec2(0.0f, 0.0f), glm::vec2(settings.SCR_WIDTH, settings.SCR_HEIGHT), 180.0f, glm::vec3(1.0f, 1.0f, 1.0f));

            Text.RenderText("You Lose!!!", settings.SCR_WIDTH / 2.0f - settings.SCR_WIDTH / 3.0f, 100.0f, settings.SCR_WIDTH / 800.0f);
            Text.RenderText("Press enter to try again.", settings.SCR_WIDTH / 2 - settings.SCR_WIDTH / 3, 100.0f+96.0f* settings.SCR_WIDTH / 800.0f + 50.0f, settings.SCR_WIDTH / 2400.0f);


            if (mainWindow.getsKeys()[GLFW_KEY_ENTER]) {
                gameManager.state = GameState::GAME_MENU;
                mainWindow.keys[GLFW_KEY_ENTER] = false;

            }
        }

        else if (gameManager.state == GameState::GAME_ACTIVE) {


            currentFrame = static_cast<float>(glfwGetTime()) - timeOffset;
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            if (gameManager.score < settings.WIN_SCORE && currentFrame >= (float)settings.WIN_TIME) {
                gameManager.state = GameState::GAME_LOSE;
                gameManager.score = 0;

            }

            std::stringstream ss; ss << gameManager.score;
            Text.RenderText("Score:" + ss.str(), 50.0f, 10.0f, 0.3f);
            
            Text.RenderText("Time: " + std::to_string((int)(currentFrame/60)) + ":" + std::to_string((int)fmod(currentFrame , 60)), settings.SCR_WIDTH-200, 10.0f, 0.3f);


            // input
            // -----

            gameManager.player.ProcessKeyboard(mainWindow.getsKeys(), deltaTime , terrain);
            gameManager.player.ProcessMouseMovement(mainWindow.getXChange());

            gameManager.playerCollisionCheck(gameManager.player, enemies);
            gameManager.playerCollisionCheck(gameManager.player, gameManager.level.objects, deltaTime);

            if (gameManager.player.currentMovement == Object_Movement::FORWARD && gameManager.player.lastMovement != gameManager.player.currentMovement) {
                playerAnimator.NewAnimation(&gameManager.animations["playerRunAnimation"]);


            }
            else if (gameManager.player.currentMovement == Object_Movement::BACKWARD && gameManager.player.lastMovement != gameManager.player.currentMovement) {
                playerAnimator.NewAnimation(&gameManager.animations["playerRunBackAnimation"]);


            }
            else if (gameManager.player.currentMovement == Object_Movement::RIGHT && gameManager.player.lastMovement != gameManager.player.currentMovement) {
                playerAnimator.NewAnimation(&gameManager.animations["playerRunRightAnimation"]);


            }
            else if (gameManager.player.currentMovement == Object_Movement::LEFT && gameManager.player.lastMovement != gameManager.player.currentMovement) {
                playerAnimator.NewAnimation(&gameManager.animations["playerRunLeftAnimation"]);

            }
            //else if (player.currentMovement == Object_Movement::STILL && player.lastMovement == Object_Movement::STILL) {
            //    playerAnimator.NewAnimation(&playerIdleAnimation);

            //}

            zombieAnimator.UpdateAnimation(deltaTime);
            playerAnimator.UpdateAnimation(deltaTime);


            gameManager.shaders["animationShader"].use();
            gameManager.shaders["animationShader"].setBool("useLight", false);
            gameManager.shaders["animationShader"].setMat4("projection", gameManager.perspectiveProjection);

            // render the Player model

            gameManager.shaders["animationShader"].setMat4("model", gameManager.player.GetModelMatrix(settings.PLAYER_YAW_OFFSET));
            glm::vec3 camPos = gameManager.cameraOffset + gameManager.player.objectPosition;
            glm::mat4 view = gameManager.camera->GetViewMatrix(camPos, gameManager.player.objectPosition, gameManager.camera->cameraUp);
            gameManager.shaders["animationShader"].setMat4("view", view);



            auto transforms = playerAnimator.GetFinalBoneMatrices();
            for (int i = 0; i < transforms.size(); ++i)
                gameManager.shaders["animationShader"].setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);

            gameManager.player.Draw(gameManager.shaders["animationShader"]);



            // generate Enemies at locations

            if (currentFrame - lastEnemyFrame >= settings.SPAWN_DELAY) {

                //std::cout << "spawn\n";
                lastEnemyFrame = currentFrame;
                Enemy enemy(gameManager.spawnPosition(glm::vec3(gameManager.player.objectPosition.x, 0.0f, gameManager.player.objectPosition.z)), settings.ENEMY_SCALE, settings.ENEMY_SPEED);
                enemy.LinkMesh(gameManager.models["zombie"]);
                enemies.push_back(enemy);

            }


            //Render enemies


            for (int i = 0; i < enemies.size(); i++) {
                glm::vec3  enemyCurrentPosition = enemies[i].Move(deltaTime, glm::vec3(gameManager.player.objectPosition.x, 0.0f, gameManager.player.objectPosition.z) , terrain);
                gameManager.shaders["animationShader"].setMat4("model", enemies[i].GetModelMatrix(enemyCurrentPosition, settings.ENEMY_YAW_OFFSET));


                auto transforms = zombieAnimator.GetFinalBoneMatrices();
                for (int i = 0; i < transforms.size(); ++i)
                    gameManager.shaders["animationShader"].setMat4("finalBonesMatrices[" + std::to_string(i) + "]", transforms[i]);



                enemies[i].Draw(gameManager.shaders["animationShader"]);
            }

            //Environment

            gameManager.shaders["shader"].use();
            gameManager.shaders["shader"].setMat4("projection", gameManager.perspectiveProjection);
            gameManager.shaders["shader"].setMat4("view", view);
            gameManager.shaders["shader"].setVec3("viewPos", camPos);
            gameManager.configureLighting(gameManager.shaders["shader"] , camPos);
         
            gameManager.shaders["shader"].setBool("useTex", true);
            gameManager.shaders["shader"].setMat4("model", gameManager.floorObject->GetModelMatrix());

            gameManager.floorObject->Draw(gameManager.shaders["shader"]);

          /* for (int i = 0; i < 1;i++) {
               gameManager.shaders["shader"].setMat4("model", terrainObjects[i].GetModelMatrix());
                terrainObjects[i].Draw(gameManager.shaders["shader"]);

            }*/

           gameManager.level.Draw(gameManager.shaders["shader"]);

           if (mainWindow.mouseLeftClick) {
               glm::vec3 bulletCurrentPos = glm::vec3(gameManager.player.objectPosition.x, 0.55f + gameManager.player.objectPosition.y, gameManager.player.objectPosition.z);
               Bullet bullet(bulletCurrentPos, settings.BULLET_SCALE, settings.BULLET_SPEED, gameManager.player.yaw);
               bullet.LinkMesh(gameManager.models["bullet"]);
               bullets.push_back(bullet);
               mainWindow.mouseLeftClick = false;

           }


            //Check collisions

            gameManager.bulletHandler(bullets, enemies, gameManager.shaders["shader"], deltaTime);


            
            // 2D stuff

            gameManager.shaders["spriteShader"].use();
            gameManager.shaders["spriteShader"].setMat4("projection", gameManager.orthoProjection);
            glm::vec2 currentCursorPos = glm::vec2(gameManager.player.objectPosition.x + gameManager.player.objectFront.x * settings.CROSSHAIR_RADIUS + (float)settings.SCR_WIDTH / 2, gameManager.player.objectPosition.z + gameManager.player.objectFront.z * settings.CROSSHAIR_RADIUS + (float)settings.SCR_HEIGHT / 2);
            glm::vec4 mod = view * glm::vec4(currentCursorPos.x, 0.0f, currentCursorPos.y, 1.0f);
            spriteRenderer.DrawSprite(gameManager.textures["crosshairTexture"], glm::vec2(mod.x, mod.z), glm::vec2(30.0f, 30.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

             //std::cout << "x1:" << player.objectPosition.x<<"\n";
             //std::cout << "x2:" << player.objectposition.x - glm::abs(player.boundingbox[1]) << "\n";

             //std::cout << "z1:" << player.objectPosition.z<<"\n";
             //std::cout << "z2:" << player.objectposition.z - glm::abs(player.boundingbox[5]) << "\n";

            // std::cout << "y1:" << gameManager.player.objectPosition.y<<"\n";
            // std::cout << "y2:" << gameManager.player.objectposition.y - glm::abs(player.boundingbox[3]) << "\n";




        }
        // -------------------------------------------------------------------------------
        mainWindow.swapBuffers();
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
