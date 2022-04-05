#include "Game.h"




Player* player;
GameObject* floorObject;
Camera *camera;


Game::Game():state(GAME_MENU)
{
    floorObject = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(settings.TERRAIN_SCALE));

    player = new Player();
    player->objectSize = glm::vec3(settings.PLAYER_SCALE);

    glm::vec3 cameraOffset = glm::vec3(0.0f, 3.0f, 3.0f);
    camera = new Camera((cameraOffset));

    Init();


}

Game::~Game()
{
    delete player;
    delete floorObject;
    delete camera;
}




void Game::Init()
{
    InitShaders();
    InitModels();


    //projection matrices
    glm::mat4 projection = glm::perspective(glm::radians(camera->Zoom), (float)settings.SCR_WIDTH / (float)settings.SCR_HEIGHT, 0.1f, 100.0f);
    glm::mat4 orthoProjection = glm::ortho(0.0f, (float)settings.SCR_WIDTH, (float)settings.SCR_HEIGHT, 0.0f, -1.0f, 1.0f);


}

void InitAnimations(Model zombieModel, Model playerModel)
{
    Animation zombieRunAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/Mutant/Mutant.fbx", &zombieModel);
    Animation zombieMeleeAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/Mutant/MeeleCombo.fbx", &zombieModel);
    Animator zombieAnimator(&zombieRunAnimation);

    Animation playerRunAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/RunForward.fbx", &playerModel);
    Animation playerRunBackAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/RunBackward.fbx", &playerModel);
    Animation playerIdleAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/Idle.fbx", &playerModel);
    Animation playerRunLeftAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/RunLeft.fbx", &playerModel);
    Animation playerRunRightAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/RunRight.fbx", &playerModel);
    Animator playerAnimator(&playerIdleAnimation);

}

void Game::InitModels()
{
    Model floorModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/floor/floor.obj"), settings.TERRAIN_SCALE * 2);
    Model boxModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/box/box.obj"));
    Model bulletModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/bullets/bullet.obj"));
    Model zombieModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/Mutant/Mutant.fbx"));
    Model playerModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/Player.fbx"));

    InitAnimations(zombieModel,playerModel);

    floorObject->LinkMesh(floorModel);
    player->LinkMesh(playerModel);
}






void Game::InitShaders()
{
    // build and compile shaders
    // -------------------------
    Shader basicShader("Shaders/basic_vertex_shader.vs", "Shaders/basic_fragment_shader.fs");
    Shader shader("Shaders/vertex_shader.vs", "Shaders/fragment_shader.fs");
    Shader animationShader("Shaders/vertex_shader_animation.vs", "Shaders/fragment_shader_animation.fs");

    Shader spriteShader("Shaders/sprite_vertex_shader.vs", "Shaders/sprite_fragment_shader.fs");
    Texture2D crosshair("cross2.png", "C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/sprites", false);
    SpriteRenderer spriteRenderer(spriteShader);
}


void Game::bulletHandler(vector<Bullet>& bullets, vector<Enemy>& objects, Shader shader, float deltaTime) {

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


void Game::playerCollisionCheck(Player player, vector<Enemy>& objects) {


    for (int j = 0; j < (int)objects.size(); j++) {

        if (player.CheckCollision(objects[j])) {
            //std::cout << "Game over\n";
            break;
        }
    }

}

glm::vec3 Game::spawnPosition(glm::vec3 playerPosition) {

    srand(time(0));


    float randomX = (rand() % static_cast<int>(2 * settings.SPAWN_RADIUS_MAX)) + playerPosition.x;
    float randomZ = (rand() % static_cast<int>(2 * settings.SPAWN_RADIUS_MAX)) + playerPosition.z;
    float sign = -1 + (rand() % static_cast<int>(3));
    return glm::vec3(sign * randomX + playerPosition.x + settings.SPAWN_RADIUS_MIN, playerPosition.y, sign * randomZ + playerPosition.z + settings.SPAWN_RADIUS_MIN);

}
