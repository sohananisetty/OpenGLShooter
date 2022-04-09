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
    GAME_INST,
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
float score = 0.0f;

int main()
{
    GameLevel level;
    mainWindow = Window(settings.SCR_WIDTH, settings.SCR_HEIGHT);
    mainWindow.intitialise();
//    stbi_set_flip_vertically_on_load(true);


    // build and compile shaders
    // -------------------------
    Shader basicShader("Shaders/vertex_shader.vs", "Shaders/basic_fragment_shader.fs");
    Shader shader("Shaders/vertex_shader.vs", "Shaders/fragment_shader.fs");
    Shader animationShader("Shaders/vertex_shader_animation.vs", "Shaders/fragment_shader_animation.fs");
    Shader textShader("Shaders/vertex_shader_text.vs", "Shaders/fragment_shader_text.fs");

    Shader spriteShader("Shaders/sprite_vertex_shader.vs", "Shaders/sprite_fragment_shader.fs");
    Texture2D crosshair("cross2.png", "C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/sprites", true);
    Texture2D menue("menue.jpg", "C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/sprites", true);
    Texture2D controls("controls2.png", "C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/sprites", false);

    SpriteRenderer spriteRenderer(spriteShader);


    // load models
    // -----------
    Model floorModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/floor/floor.obj"), settings.TERRAIN_SCALE * 2);
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

    vector<GameObject> terrainObjects;
    Texture2D floorTexture("Floor D2.psd", "C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/floor", false);
    vector <Texture2D> terrainTexures = { floorTexture };

    //for (int i = 0; i < 1;i++) {
                            

        Terrain terrain(0,0, "height.png", "C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/sprites", terrainTexures,settings.TERRAIN_SCALE,settings.TERRAIN_HEIGHT,settings.TERRAIN_TILLING);

        vector<Mesh> meshes; meshes.push_back(terrain.mesh);
        Model terrainModel; terrainModel.meshes = meshes;

        GameObject terrainObject(glm::vec3(terrain.x, 0.0f, terrain.z));
        terrainObject.LinkMesh(terrainModel);

        terrainObjects.push_back(terrainObject);
    //}

  


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

        glClearColor(0.9215f, 0.96078f, 0.9843f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        if (score == 50) {
            state = GameState::GAME_WIN;
            score = 0;
        }



        if (state == GameState::GAME_MENU) {

            spriteShader.use();
            spriteShader.setMat4("projection", orthoProjection);
            spriteRenderer.DrawSprite(menue, glm::vec2(0.0f,0.0f), glm::vec2(settings.SCR_WIDTH, settings.SCR_HEIGHT),180.0f, glm::vec3(1.0f, 1.0f, 1.0f));
            
            Text.RenderText("Demon Slayer", 60.0f, 50.0f, 1.0f);
            Text.RenderText("Press enter to start!", 200.0f, settings.SCR_HEIGHT-100.0f, 0.25f);

            if (mainWindow.getsKeys()[GLFW_KEY_ENTER]) {

                state = GameState::GAME_INST;
                mainWindow.keys[GLFW_KEY_ENTER] = false;

            }
        }

        else if (state == GameState::GAME_INST) {

            spriteRenderer.DrawSprite(controls, glm::vec2(50.0f, 300.0f), glm::vec2(settings.SCR_WIDTH-100.0f, settings.SCR_HEIGHT/600 *250.0f ), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

            Text.RenderText("Instructions", 300.0f, 50.0f, 0.5f , glm::vec3(0.0f));
            Text.RenderText("Defeat monster to get points. Get 100 points to win!", 100.0f, 150.0f, 0.25f, glm::vec3(0.0f));

            if (mainWindow.getsKeys()[GLFW_KEY_ENTER] ) {
                state = GameState::GAME_ACTIVE;
                mainWindow.keys[GLFW_KEY_ENTER] = false;
            }

        }

        else if (state == GameState::GAME_WIN) {

            spriteShader.use();
            spriteShader.setMat4("projection", orthoProjection);
            spriteRenderer.DrawSprite(menue, glm::vec2(0.0f, 0.0f), glm::vec2(settings.SCR_WIDTH, settings.SCR_HEIGHT), 180.0f, glm::vec3(1.0f, 1.0f, 1.0f));

            Text.RenderText("You Won!!!", 60.0f, 50.0f, 1.0f);


            if (mainWindow.getsKeys()[GLFW_KEY_ENTER]) {
                state = GameState::GAME_MENU;
                mainWindow.keys[GLFW_KEY_ENTER] = false;

            }
        }

        else if (state == GameState::GAME_ACTIVE) {


            float currentFrame = static_cast<float>(glfwGetTime());
            deltaTime = currentFrame - lastFrame;
            lastFrame = currentFrame;

            std::stringstream ss; ss << score;
            Text.RenderText("Score:" + ss.str(), 350.0f, 10.0f, 0.3f);

            // input
            // -----

            player.ProcessKeyboard(mainWindow.getsKeys(), deltaTime , terrain);
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

            shader.setVec3("viewPos", camPos);
            shader.setInt("numPointLights" , 0);
           
            // directional light
            shader.setVec3("dirLight.direction", -1.0f, -1.0f, 1.f);
            shader.setVec3("dirLight.ambient", glm::vec3(0.2f));
            shader.setVec3("dirLight.diffuse", glm::vec3(0.2f));
            shader.setVec3("dirLight.specular", glm::vec3(0.2f));

            // point light 1
            shader.setVec3("pointLights[0].position", glm::vec3(3.0, 0.5, -3.0));
            shader.setVec3("pointLights[0].ambient", glm::vec3(0.2f));
            shader.setVec3("pointLights[0].diffuse", glm::vec3(0.8f));
            shader.setVec3("pointLights[0].specular", glm::vec3(1.0f));
            shader.setFloat("pointLights[0].constant", 1.0f);
            shader.setFloat("pointLights[0].linear", 0.09f);
            shader.setFloat("pointLights[0].quadratic", 0.032f);

            // spotLight
            shader.setVec3("spotLight.position", camPos);
            shader.setVec3("spotLight.direction", player.objectPosition - camPos);
            shader.setVec3("spotLight.ambient", glm::vec3(0.1f));
            shader.setVec3("spotLight.diffuse", glm::vec3(0.8f));
            shader.setVec3("spotLight.specular", glm::vec3(1.0f));
            shader.setFloat("spotLight.constant", 1.0f);
            shader.setFloat("spotLight.linear", 0.09f);
            shader.setFloat("spotLight.quadratic", 0.032f);
            shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.5f)));
            shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(100.5f)));

            shader.setBool("useTex", true);
            //shader.setMat4("model", floor.GetModelMatrix());
            //floor.Draw(shader);
           for (int i = 0; i < 1;i++) {
                shader.setMat4("model", terrainObjects[i].GetModelMatrix());
                terrainObjects[i].Draw(shader);

            }
           
            level.Draw(shader);


            //Check collisions

            bulletHandler(bullets, enemies, shader, deltaTime);


            if (mainWindow.mouseLeftClick) {
                glm::vec3 bulletCurrentPos = glm::vec3(player.objectPosition.x, 0.55f + player.objectPosition.y, player.objectPosition.z);
                Bullet bullet(bulletCurrentPos, settings.BULLET_SCALE, settings.BULLET_SPEED, player.yaw);
                bullet.LinkMesh(bulletModel);
                bullets.push_back(bullet);
                mainWindow.mouseLeftClick = false;

            }
            // 2D stuff

            spriteShader.use();
            spriteShader.setMat4("projection", orthoProjection);
            glm::vec2 currentCursorPos = glm::vec2(player.objectPosition.x + player.objectFront.x * crosshairRadius + (float)settings.SCR_WIDTH / 2, player.objectPosition.z + player.objectFront.z * crosshairRadius + (float)settings.SCR_HEIGHT / 2);
            glm::vec4 mod = view * glm::vec4(currentCursorPos.x, 0.0f, currentCursorPos.y, 1.0f);
            spriteRenderer.DrawSprite(crosshair, glm::vec2(mod.x, mod.z), glm::vec2(30.0f, 30.0f), 0.0f, glm::vec3(1.0f, 1.0f, 1.0f));

             std::cout << "x1:" << player.objectPosition.x<<"\n";
             //std::cout << "x2:" << player.objectposition.x - glm::abs(player.boundingbox[1]) << "\n";

             std::cout << "z1:" << player.objectPosition.z<<"\n";
             //std::cout << "z2:" << player.objectposition.z - glm::abs(player.boundingbox[5]) << "\n";

             std::cout << "y1:" << player.objectPosition.y<<"\n";
             //std::cout << "y2:" << player.objectposition.y - glm::abs(player.boundingbox[3]) << "\n";




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
                score += 10;
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
            std::cout << "enemy collision\n";
            score = 0.0f;
            //state = GameState::GAME_MENU;
            break;
        }
    }

}

void playerCollisionCheck(Player& player, vector<GameObject>& objects, float deltaTime) {


    for (int j = 0; j < (int)objects.size(); j++) {

        if (player.CheckCollision(objects[j])) {
            //std::cout << "world collision\n";
            float distance = player.objectSpeed * deltaTime;

            if(player.objectPosition.y - glm::abs(player.boundingBox[3]) <= objects[j].objectPosition.y + glm::abs(objects[j].boundingBox[2]) && player.objectPosition.y - glm::abs(player.boundingBox[3]) + 0.1 >= objects[j].objectPosition.y + glm::abs(objects[j].boundingBox[2])) {
                std::cout << "z collision\n";
                
                player.objectPosition.y -= player.currentUpSpeed * deltaTime;
                player.currentUpSpeed = 0.0f;

            }

            else if (player.lastMovement == Object_Movement::FORWARD)
                player.objectPosition -= player.objectFront * distance;

            else if (player.lastMovement == Object_Movement::BACKWARD)
                player.objectPosition += player.objectFront * distance;

            else if (player.lastMovement == Object_Movement::LEFT)
                player.objectPosition += player.objectRight * distance;

            else if (player.lastMovement == Object_Movement::RIGHT)
                player.objectPosition -= player.objectRight * distance;

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
