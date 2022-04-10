#include "Game.h"




//Player* player;
//GameObject* floorObject;
//Camera *camera;


Game::Game():state(GameState::GAME_MENU)
{
    floorObject = new GameObject(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(settings.TERRAIN_SCALE * 10));

    //player = new Player();
    player.objectSize = glm::vec3(settings.PLAYER_SCALE);

    cameraOffset = glm::vec3(0.0f, 3.0f, 3.0f);
    camera = new Camera((cameraOffset));

    score = 0;

    Init();


}

Game::~Game()
{
    //delete player;
    delete floorObject;
    delete camera;
}



void Game::Init()
{
    InitShaders();
    InitModels();
    InitTextures();
    InitGameObjects();

    //projection matrices
    perspectiveProjection = glm::perspective(glm::radians(camera->Zoom), (float)settings.SCR_WIDTH / (float)settings.SCR_HEIGHT, 0.1f, 100.0f);
    orthoProjection = glm::ortho(0.0f, (float)settings.SCR_WIDTH, (float)settings.SCR_HEIGHT, 0.0f, -1.0f, 1.0f);

    //SpriteRenderer spriteRenderer(shaders["spriteShader"]);

    //TextRenderer Text(shaders["textShader"], orthoProjection);
    //Text.Load("Fonts/arialbd.ttf", 96);

    //level.Load("level1.json");

}

void Game::InitAnimations()
{
    Animation zombieRunAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/Mutant/Mutant.fbx", &models["zombie"]);
    Animation zombieMeleeAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/Mutant/MeeleCombo.fbx", &models["zombie"]);
    //Animator zombieAnimator(&zombieRunAnimation);
    zombieAnimator = new Animator(&zombieRunAnimation);

    Animation playerRunAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/RunForward.fbx", &models["player"]);
    Animation playerRunBackAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/RunBackward.fbx", &models["player"]);
    Animation playerIdleAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/Idle.fbx", &models["player"]);
    Animation playerRunLeftAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/RunLeft.fbx", &models["player"]);
    Animation playerRunRightAnimation("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/RunRight.fbx", &models["player"]);
    Animator playerAnimator(&playerIdleAnimation);

    this->animations.insert(std::pair<string, Animation>("zombieRunAnimation", zombieRunAnimation));
    this->animations.insert(std::pair<string, Animation>("zombieMeleeAnimation", zombieMeleeAnimation));


    this->animations.insert(std::pair<string, Animation>("playerRunAnimation", playerRunAnimation));
    this->animations.insert(std::pair<string, Animation>("playerIdleAnimation", playerIdleAnimation));
    this->animations.insert(std::pair<string, Animation>("playerRunLeftAnimation", playerRunLeftAnimation));
    this->animations.insert(std::pair<string, Animation>("playerRunBackAnimation", playerRunBackAnimation));
    this->animations.insert(std::pair<string, Animation>("playerRunRightAnimation", playerRunRightAnimation));

    //this->animators.insert(std::pair<string, Animator>("zombieAnimator", zombieAnimator));
    //this->animators.insert(std::pair<string, Animator>("playerAnimator", playerAnimator));




}

void Game::InitModels()
{
    Model floorModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/floor/floor.obj"), settings.TERRAIN_TILLING);
    Model boxModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/box/box.obj"));
    Model bulletModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/bullets/bullet.obj"));
    Model zombieModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/Mutant/Mutant.fbx"));
    Model playerModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/MainPlayer/Player.fbx"));
    //Model dropshipModel(("C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/dropship/dropship.fbx"));

    this->models.insert(std::pair<string, Model>("floor", floorModel));
    this->models.insert(std::pair<string, Model>("box", boxModel));
    this->models.insert(std::pair<string, Model>("bullet", bulletModel));
    this->models.insert(std::pair<string, Model>("zombie", zombieModel));
    this->models.insert(std::pair<string, Model>("player", playerModel));
    //this->models.insert(std::pair<string, Model>("dropship", dropshipModel));

    InitAnimations();

}

void Game::InitShaders()
{
    // build and compile shaders
    // -------------------------
    Shader basicShader("Shaders/basic_vertex_shader.vs", "Shaders/basic_fragment_shader.fs");
    Shader shader("Shaders/vertex_shader.vs", "Shaders/fragment_shader.fs");
    Shader animationShader("Shaders/vertex_shader_animation.vs", "Shaders/fragment_shader_animation.fs");
    Shader textShader("Shaders/vertex_shader_text.vs", "Shaders/fragment_shader_text.fs");
    Shader spriteShader("Shaders/sprite_vertex_shader.vs", "Shaders/sprite_fragment_shader.fs");

    this->shaders.insert(std::pair<string, Shader>("basicShader", basicShader));
    this->shaders.insert(std::pair<string, Shader>("shader", shader));
    this->shaders.insert(std::pair<string, Shader>("animationShader", animationShader));
    this->shaders.insert(std::pair<string, Shader>("textShader", textShader));
    this->shaders.insert(std::pair<string, Shader>("spriteShader", spriteShader));


}

void Game::InitTextures(){

    Texture2D crosshair("cross2.png", "C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/sprites", false);
    Texture2D floorTexture("Floor D2.psd", "C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/floor", false);
    Texture2D menue("menue.jpg", "C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/sprites", true);
    Texture2D controls("controls2.png", "C:/users/abc/Documents/Visual Studio 2022/Projects/PlayerMovement/Assets/sprites", false);

    this->textures.insert(std::pair<string, Texture2D>("crosshairTexture", crosshair));
    this->textures.insert(std::pair<string, Texture2D>("floorTexture", floorTexture));
    this->textures.insert(std::pair<string, Texture2D>("menueTexture", menue));
    this->textures.insert(std::pair<string, Texture2D>("controlsTexture", controls));



}

void Game::InitGameObjects(){

    
    floorObject->LinkMesh(models["floor"]);
    player.objectSize = glm::vec3(settings.PLAYER_SCALE);
    player.LinkMesh(models["player"]);


}
void Game::configureLighting(Shader &shader , glm::vec3 spotPos){

    shader.use();
    shader.setInt("numPointLights", settings.POINTLIGHT_NUMBER);

    // directional light
    shader.setVec3("dirLight.direction", settings.DIRECTIONAL_LIGHT);
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
    shader.setVec3("spotLight.position", spotPos);
    shader.setVec3("spotLight.direction", player.objectPosition - spotPos);
    shader.setVec3("spotLight.ambient", glm::vec3(0.2f));
    shader.setVec3("spotLight.diffuse", glm::vec3(0.8f));
    shader.setVec3("spotLight.specular", glm::vec3(1.0f));
    shader.setFloat("spotLight.constant", 1.0f);
    shader.setFloat("spotLight.linear", 0.09f);
    shader.setFloat("spotLight.quadratic", 0.032f);
    shader.setFloat("spotLight.cutOff", glm::cos(glm::radians(10.5f)));
    shader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(100.5f)));

}





void Game::bulletHandler(vector<Bullet>& bullets, vector<Enemy>& objects, Shader shader, float deltaTime) {

    for (int i = 0; i < (int)bullets.size(); i++) {

        for (int j = 0; j < (int)objects.size(); j++) {

            if (bullets[i].CheckCollision(objects[j])) {
                std::cout << "bullet collision\n";
                this->score += 10;
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
            std::cout << "enemy collision\n";
            this->score = 0.0f;
            //state = GameState::GAME_MENU;
            break;
        }
    }

}

void Game::playerCollisionCheck(Player& player, vector<GameObject>& objects, float deltaTime) {


    for (int j = 0; j < (int)objects.size(); j++) {

        if (objects[j].computeCollision) {
            if (player.CheckCollision(objects[j])) {
                //std::cout << "world collision\n";
                float distance = player.objectSpeed * deltaTime;

                if (player.objectPosition.y - glm::abs(player.boundingBox[3]) <= objects[j].objectPosition.y + glm::abs(objects[j].boundingBox[2]) && player.objectPosition.y - glm::abs(player.boundingBox[3]) + 0.1 >= objects[j].objectPosition.y + glm::abs(objects[j].boundingBox[2])) {
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

}

glm::vec3 Game::spawnPosition(glm::vec3 playerPosition) {

    srand(time(0));


    float randomX = (rand() % static_cast<int>(2 * settings.SPAWN_RADIUS_MAX)) + playerPosition.x;
    float randomZ = (rand() % static_cast<int>(2 * settings.SPAWN_RADIUS_MAX)) + playerPosition.z;
    float sign = -1 + (rand() % static_cast<int>(3));
    return glm::vec3(sign * randomX + playerPosition.x + settings.SPAWN_RADIUS_MIN, playerPosition.y, sign * randomZ + playerPosition.z + settings.SPAWN_RADIUS_MIN);

}
