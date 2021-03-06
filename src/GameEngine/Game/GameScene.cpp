#include "GameScene.h"
#include "MarioGame.h"
#include "Blocks.h"
#include "Block.h"
#include "BricksBlock.h"
#include "MoneyBlock.h"
#include "QuiestionBlock.h"
#include "StaticBlock.h"
#include "TwistedCoin.h"
#include "Mushroom.h"
#include "Ladder.h"
#include "Start.h"
#include "GameEngine.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopa.h"
#include "HammerBro.h"
#include "Bowser.h"
#include "BuzzyBeetle.h"
#include "Lakity.h"
#include "CheepCheep.h"
#include "Blooper.h"
#include "BulletBill.h"
#include "PiranhaPlant.h"
#include "Podoboo.h"
#include "Coin.h"
#include "Jumper.h"
#include "FireBar.h"
#include "Platform.h"
#include "PlatformSytem.h"
#include "Background.h"
#include "LevelPortal.h"
#include "EndLevelFlag.h"
#include "EndLevelKey.h"
#include "CastleFlag.h"
#include "Pricess.h"
#include "Trigger.h"
#include "Text.h"

#include <tinyxml2.h>

using namespace GameObjects;

namespace GameEngine
{
	Blocks* ParseBlocks(tinyxml2::XMLElement* element)
	{
		tinyxml2::XMLElement* data = element->FirstChildElement("layer")->FirstChildElement("data");

		//convert cvs format to string, where each char definite type of block
		std::string dirty_string = data->GetText();
		std::string new_string, buf;

		for (size_t i = 0; i < dirty_string.size(); ++i)
		{
			if (std::isdigit(dirty_string[i]))
				buf += dirty_string[i];
			else if (!buf.empty())
			{
				new_string += (char)ToInt(buf);
				buf.clear();
			}
		}

		static auto blocks_fabric = [](char c) -> Block*
		{
			static const bool INVIZ_STYLE = true,
				NOT_INVIZ_STYLE = false,
				BRICK_STYLE = true;

			switch (c)
			{
			case 0: return nullptr;
			case BlockTitleCode::Brick:
				return new BricksBlock();
			case BlockTitleCode::Money:
				return new MoneyBlock();
			case BlockTitleCode::CoinTitle:
				return new QuestionBlock<TwistedCoin>(BlockTitleCode::CoinTitle);
			case BlockTitleCode::MushroomTitle:
				return new QuestionBlock<Mushroom>(BlockTitleCode::MushroomTitle);
			case BlockTitleCode::BrickMushroom:
				return new QuestionBlock<Mushroom>(BlockTitleCode::BrickMushroom, NOT_INVIZ_STYLE, BRICK_STYLE);
			case BlockTitleCode::LadderTitle:
				return new QuestionBlock<Ladder>(BlockTitleCode::LadderTitle);
			case BlockTitleCode::InvizLadder:
				return new QuestionBlock<Ladder>(BlockTitleCode::InvizLadder, INVIZ_STYLE);
			case BlockTitleCode::BrickLadder:
				return new QuestionBlock<Ladder>(BlockTitleCode::BrickLadder, NOT_INVIZ_STYLE, BRICK_STYLE);
			case BlockTitleCode::InizUP:
				return new QuestionBlock<OneUpMushroom>(BlockTitleCode::InizUP, INVIZ_STYLE);
			case BlockTitleCode::InizCoin:
				return new QuestionBlock<TwistedCoin>(BlockTitleCode::InizCoin, INVIZ_STYLE);
			case BlockTitleCode::LiveUp:
				return new QuestionBlock<OneUpMushroom>(BlockTitleCode::LiveUp, NOT_INVIZ_STYLE, BRICK_STYLE);
			case BlockTitleCode::StartTile:
				return new QuestionBlock<Start>(BlockTitleCode::StartTile, NOT_INVIZ_STYLE, BRICK_STYLE);
			default:
				return new StaticBlock(BlockTitleCode((int)c));
			}
		};

		int cols, rows, block_size;

		cols = ToInt(element->Attribute("width"));
		rows = ToInt(element->Attribute("height"));
		block_size = ToInt(element->Attribute("tilewidth"));

		Blocks* blocks = new Blocks(cols, rows, block_size);

		blocks->LoadFromString(new_string, blocks_fabric);

		return blocks;
	}

	template <typename T>
	GameObject* GoFabric()
	{
		return new T;
	}

	GameObject* TextFabric()
	{
		Label* label = new Label();
		label->SetFontName(*sMarioGame->FontManager().Get("some_font"));
		label->SetFontStyle(sf::Text::Bold);
		label->SetFontColor({ 255,255,220 });
		label->SetFontSize(36);
		label->SetTextAlign(Label::TextAling::Center);
		label->SetFillColor(sf::Color::Transparent);
		return label;
	}

	std::map<std::string, Property> ParseProperties(tinyxml2::XMLElement* object)
	{
		std::map<std::string, Property> parsed;
		//Common properties
		parsed["X"] = Property(ToFloat(object->Attribute("x")));
		parsed["Y"] = Property(ToFloat(object->Attribute("y")));
		parsed["Width"] = Property(ToFloat(object->Attribute("width")));
		parsed["Height"] = Property(ToFloat(object->Attribute("height")));
		parsed["Name"] = Property(ToString(object->Attribute("name")));

		//Specific properties
		if (tinyxml2::XMLElement* properties = object->FirstChildElement("properties"))
		{
			for (auto property = properties->FirstChildElement("property"); property != nullptr; property = property->NextSiblingElement())
			{
				std::string type("string");

				if (const char* type_att = property->Attribute("type"))
					type = type_att;

				std::string name = property->Attribute("name");
				std::string value = property->Attribute("value");

				if (type == "int")
					parsed[name] = Property(ToInt(value));
				else if (type == "float")
					parsed[name] = Property(ToFloat(value));
				else if (type == "bool")
					parsed[name] = Property(ToBool(value));
				else
					parsed[name] = Property(ToString(value));
			}
		}

		//Parse text element
		if (tinyxml2::XMLElement* text_properties = object->FirstChildElement("text"))
			parsed["text"] = std::string(text_properties->FirstChild()->Value());

		return parsed;
	}

	GameObject* ParseGameObject(tinyxml2::XMLElement* element)
	{
		std::string obj_type = element->Attribute("type");

		static std::unordered_map<std::string, GameObject * (*)()> fabrics = {
			{"Mario",				GoFabric<Creatures::Mario>},
			{"Goomba",				GoFabric<Creatures::Goomba>},
			{"Koopa",				GoFabric<Creatures::Koopa>},
			{ "HammerBro",			GoFabric<Creatures::HammerBro> },
			{ "Bowser",				GoFabric<Creatures::Bowser> },
			{ "BuzzyBeetle",		GoFabric<Creatures::BuzzyBeetle> },
			{ "LakitySpawner",		GoFabric<Creatures::LakitySpawner> },
			{ "CheepCheep",			GoFabric<Creatures::CheepCheep> },
			{ "Blooper",			GoFabric<Creatures::Blooper> },
			{ "CheepCheepSpawner",	GoFabric<Creatures::CheepCheepSpawner>},
			{ "BulletBillSpawner",	GoFabric<Creatures::BulletBillSpawner> },
			{ "PiranhaPlant",		GoFabric<Creatures::PiranhaPlant> },
			{ "Podoboo",			GoFabric<Creatures::Podoboo> },
			{ "Coin",				GoFabric<GameObjects::Coin> },
			{ "Jumper",				GoFabric<GameObjects::Jumper> },
			{ "FireBar",			GoFabric<GameObjects::FireBar> },
			{ "MoveablePlatform",	GoFabric<GameObjects::MoveablePlatform> },
			{ "FallingPlatform",	GoFabric<GameObjects::FallingPlatform> },
			{ "PlatformSystem",		GoFabric<GameObjects::PlatformSystem> },
			{ "Background",			GoFabric<GameObjects::Background> },
			{ "LevelPortal",		GoFabric<GameObjects::LevelPortal> },
			{ "EndLevelFlag",		GoFabric<GameObjects::EndLevelFlag> },
			{ "EndLevelKey",		GoFabric<GameObjects::EndLevelKey> },
			{ "CastleFlag",			GoFabric<GameObjects::CastleFlag> },
			{ "Princess",			GoFabric<GameObjects::Princess> },
			{ "Trigger",			GoFabric<GameObjects::Trigger> },
			{ "Text",				TextFabric }
		};

		auto object_fabric = fabrics[obj_type];

		assert(object_fabric);

		GameObject* object = object_fabric();
		auto properties = ParseProperties(element);
		for (auto& property : properties)
			object->SetProperty(property.first, property.second);

		return object;
	}

	GameScene::GameScene(const std::string& filepath)
	{
		LoadFromFile(filepath);
		Init();
	}

	GameScene::GameScene()
	{
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
		blocks = ParseBlocks(root_element);
		AddObject(blocks);

		//Load objects
		tinyxml2::XMLElement* objects = root_element->FirstChildElement("objectgroup");
		for (auto obj = objects->FirstChildElement("object"); obj != nullptr; obj = obj->NextSiblingElement())
		{
			if (GameObject* object = ParseGameObject(obj))
				AddObject(object);
		}

		mario = FindObjectByType<Creatures::Mario>();

		assert(mario);

		mario->MoveToFront();
		SetCameraOnTarget();
	}

	Rect GameScene::CameraRect() const
	{
		return Rect(Vector(view.getCenter()) - Vector(view.getSize()) * 0.5, view.getSize());
	}

	void GameScene::SetCameraOnTarget()
	{
		view.setCenter(mario->GetBounds().Center());
	}

	Vector GameScene::PointToScreen(const Vector& vector)
	{
		return (vector - (Vector(view.getCenter()) - Vector(view.getSize()) / 2)) * scale_factor;
	}

	Vector GameScene::ScreenToPoint(const Vector& vector)
	{
		return vector / scale_factor + (Vector(view.getCenter()) - Vector(view.getSize()) / 2);
	}

	void GameScene::PlaySoundAtPoint(const std::string& name, const Vector& pos)
	{
		if (CameraRect().IsContain(pos))
			sMarioGame->PlaySound(name);
	}

	void GameScene::Update(int delta_time)
	{
		GameObject::Update(delta_time);

		Vector camera_pos = view.getCenter();
		const Vector delta = (mario->GetBounds().Center() - camera_pos) * delta_time;

		camera_pos.X += delta.X * 0.0075f;
		camera_pos.Y += delta.Y * 0.0005f;
		camera_pos = mario->GetBounds().Center();

		camera_pos.X = math::clamp(camera_pos.X, screen_size.X / 2.f, blocks->Width() - screen_size.X / 2.f);
		camera_pos.Y = math::clamp(camera_pos.X, screen_size.Y / 2.f, blocks->Height() - screen_size.Y / 2.f);
		view.setCenter(camera_pos);
	}

	void GameScene::Draw(sf::RenderWindow* render_window)
	{
		render_window->setView(view);
		GameObject::Draw(render_window);
		render_window->setView(render_window->getDefaultView());
	}

	void GameScene::Events(const sf::Event& event)
	{
		if (event.type == sf::Event::Resized)
		{
			//screen_size = Vector(event.size.width / scale_factor, event.size.height / scale_factor);
			//m_view.setSize(screen_size);
		}
	}

	void GameScene::Init()
	{
		SetName("GameScene");
		view.setSize(screen_size);
		sMarioGame->EventManager().Subscribe(this);
	}
}