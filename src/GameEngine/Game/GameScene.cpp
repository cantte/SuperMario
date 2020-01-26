#include "GameScene.h"
#include "MarioGame.h"
#include "Blocks.h"

#include <tinyxml2.h>

namespace GameEngine
{
	GameScene::GameScene(const std::string& filepath)
	{
		LoadFromFile(filepath);
		Init();
	}

	GameScene::~GameScene()
	{
		sMarioGame->EventManager().UnSubcribe(this);
	}

	const std::string& GameScene::GetLevelName() const
	{
		return level_name;
	}

	void GameScene::LoadFromFile(const std::string& filepath)
	{
		SetName("GameScene");
		level_name = filepath;

		auto it1 = --level_name.end();
		while (*it1 != '.') it1--;
		auto it2 = it1;
		while (*it2 != '/' || it2 == level_name.begin()) it2--;
		level_name = std::string(++it2, it1);

		Clear();

		tinyxml2::XMLDocument document;
		bool status = document.LoadFile(filepath.c_str());
		assert(status == tinyxml2::XML_SUCCESS);
		tinyxml2::XMLElement* root_element = document.FirstChildElement();

		// Load tilemap
		//blocks = ParseBlocks(root_element);
	}

	Rect GameScene::CameraRect() const
	{

	}

	void GameScene::SetCameraOnTarget()
	{

	}
}