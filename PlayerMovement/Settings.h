#pragma once

#include <json.hpp>
#include <iostream>
#include <fstream>
#include<string.h>
using json = nlohmann::json;



class Settings {

public:

	//Window
	 unsigned int SCR_WIDTH = 800;
	 unsigned int SCR_HEIGHT = 600;

	 //Bullet
	 float BULLET_SPEED = 20.0f;
	 float BULLET_SCALE = 0.05f;
	 float BULLET_DIE_DISTANCE = 10.0f;

	 //Player
	 float PLAYER_SCALE = 0.005f;
	 float PLAYER_YAW_OFFSET = 90.0f;
	 //float PLAYER_YAW = 0.0f;
	 float PLAYER_SPEED = 2.5f;
	 float PLAYER_JUMP_SPEED = 5.0f;
	 float PLAYER_TURNSPEED = 2.5f;
	 float PLAYER_SENSITIVITY = 0.1f;

	 //Enemy
	 float ENEMY_SCALE = 0.007f;
	 float ENEMY_SPEED = 2.0f;
	 float SPAWN_RADIUS_MAX = 10.0f;
	 float SPAWN_RADIUS_MIN = 2.0f;
	 float SPAWN_DELAY = 5.0f;
	 float ENEMY_YAW_OFFSET = 90.0f;

	 //Terrain
	 float TERRAIN_HEIGHT = 0.0f;
	 float TERRAIN_SCALE = 40.0f;

	 json settings;



	 Settings() {
		 try {
			 std::ifstream settingsFile("settings.json");
			 settingsFile>>settings;
			 parseSettings();
		 }
		 catch (const std::ifstream::failure& e) {
			 std::cout << "Exception opening/reading file";
		 }
	 }

	 void parseSettings() {

		 SCR_WIDTH = settings["SCR_WIDTH"].get<int>();
		 SCR_HEIGHT = settings["SCR_HEIGHT"].get<int>();

		 BULLET_SPEED = settings["BULLET_SPEED"].get<float>();
		 BULLET_SCALE = settings["BULLET_SCALE"].get<float>();
		 BULLET_DIE_DISTANCE = settings["BULLET_DIE_DISTANCE"].get<float>();

		 PLAYER_SCALE = settings["PLAYER_SCALE"].get<float>();
		 PLAYER_YAW_OFFSET = settings["PLAYER_YAW_OFFSET"].get<float>();
		 //PLAYER_YAW = settings["PLAYER_YAW"].get<float>();
		 PLAYER_SPEED = settings["PLAYER_SPEED"].get<float>();
		 PLAYER_JUMP_SPEED = settings["PLAYER_JUMP_SPEED"].get<float>();
		 PLAYER_TURNSPEED = settings["PLAYER_TURNSPEED"].get<float>();
		 PLAYER_SENSITIVITY = settings["PLAYER_SENSITIVITY"].get<float>();

		 ENEMY_SCALE = settings["ENEMY_SCALE"].get<float>();
		 ENEMY_SPEED = settings["ENEMY_SPEED"].get<float>();
		 SPAWN_RADIUS_MAX = settings["SPAWN_RADIUS_MAX"].get<float>();
		 SPAWN_RADIUS_MIN = settings["SPAWN_RADIUS_MIN"].get<float>();
		 SPAWN_DELAY = settings["SPAWN_DELAY"].get<float>();
		 ENEMY_YAW_OFFSET = settings["ENEMY_YAW_OFFSET"].get<float>();

		 TERRAIN_HEIGHT = settings["TERRAIN_HEIGHT"].get<float>();
		 TERRAIN_SCALE = settings["TERRAIN_SCALE"].get<float>();



	 }

};
