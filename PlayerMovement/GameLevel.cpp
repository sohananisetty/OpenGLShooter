#include "GameLevel.h"
#include <typeinfo>

void GameLevel::Load(const char* file)
{
	try {
		std::ifstream levelFile(file);
		levelFile >> level;
		parseLevel(level);
	}
	catch (const std::ifstream::failure& e) {
		std::cout << "Exception opening/reading file";
	}
}

void GameLevel::Draw(Shader& shader)
{
	for (int i = 0; i < objects.size();i++) {
	
		shader.setVec3("objectColor" , objects[i].objectColor);
		shader.setBool("useTex", objects[i].useTex);
		shader.setMat4("model", objects[i].GetModelMatrix());
		objects[i].Draw(shader);
	}
}

void GameLevel::parseLevel(json level)
{
	std::cout << level.size() << "\n";
	for (json::iterator it = level.begin(); it != level.end(); ++it) {
		
		auto obj = it.value();

		std::vector<double> position = (obj["position"].get<std::vector<double>>());
		std::vector<double> scale = (obj["scale"].get<std::vector<double>>());
		std::vector<double> rotation = (obj["rotation"].get<std::vector<double>>());
		std::vector<double> color = (obj["color"].get<std::vector<double>>());
		std::string path = obj["path"].get<std::string>();
		bool useTex = obj["useTex"].get<bool>();
		bool useCollision = obj["collision"].get<bool>();


		Model model(path);
		GameObject gameobject(glm::vec3(position[0] , position[1] , position[2])  , glm::vec3(scale[0], scale[1], scale[2]));
		gameobject.objectColor = glm::vec3(color[0], color[1], color[2]);
		gameobject.LinkMesh(model);
		gameobject.useTex = useTex;
		gameobject.yaw = rotation[0];
		gameobject.pitch = rotation[1];
		gameobject.computeCollision = useCollision;
		objects.push_back(gameobject);

	}

}
