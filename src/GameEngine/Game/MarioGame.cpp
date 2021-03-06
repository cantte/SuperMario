#include "MarioGame.h"
#include "Mario.h"
#include "MarioState.h"
#include "MarioGUI.h"
#include "Text.h"
#include "GameScene.h"
#include "Background.h"
#include "GameEngine.h"

namespace GameEngine
{
	MarioGame::MarioGame() : Game("SuperMario", { 1280, 720 })
	{
		// Load textures
		const std::string textures_dir = MARIO_RES_PATH + "Textures/";
		for (auto texture : { "Mario", "Tiles", "AnimTiles", "Enemies", "Bowser", "Items", "Logo" })
			TextureManager().LoadFromFile(texture, textures_dir + texture + ".png");

		for (auto texture : { "Sky", "Night", "Underground", "Castle", "Water" })
		{
			TextureManager().LoadFromFile(texture, textures_dir + "Backgrounds/" + texture + ".png");
			TextureManager().Get(texture)->setRepeated(true);
		}

		// Load fonts
		const std::string fonts_dir = MARIO_RES_PATH + "Fonts/";
		for (auto font : { "arial", "menu_font", "main_font", "score_font", "some_font" })
			FontManager().LoadFromFile(font, fonts_dir + font + ".ttf");

		// Load sounds
		const std::string sounds_dir = MARIO_RES_PATH + "Sounds/";
		for (auto sound : { "breakblock", "bump", "coin", "fireball", "jump_super", "kick", "stomp","powerup_appears",
		 "powerup", "pipe","flagpole", "bowser_falls", "bowser_fire", "mario_die","stage_clear",
		 "game_over","1-up","warning", "world_clear","pause","beep","fireworks" })
			SoundManager().LoadFromFile(sound, sounds_dir + sound + ".wav");

		// Load music
		const std::string music_dir = MARIO_RES_PATH + "Music/";
		for (auto music : { "overworld", "underworld", "bowsercastle", "underwater", "invincibility" })
			MusicManager().LoadFromFile(music, music_dir + music + ".ogg");

		// Configure input
		std::vector<std::pair<std::string, std::vector<std::string>>> inputs = {
			{ "Fire",			{ "X", "[0]" } },
			{ "Jump",			{ "Space",  "[1]" } },
			{ "Pause",			{ "Enter", "[7]"} },
			{ "Horizontal+",	{ "Right" } },
			{ "Horizontal-",	{ "Left" } },
			{ "Vertical-",		{ "Up" } },
			{ "Vertical+",		{ "Down" } }
		};

		for (auto input : inputs)
			InputManager().SetupButton(input.first, input.second);
	}

	MarioGame::~MarioGame()
	{

	}

	void MarioGame::ShowStatus()
	{
		SetState(GameState::Status);
	}

	Timer& MarioGame::GetTimer()
	{
		return timer;
	}

	void MarioGame::AddScore(int value, const Vector& vector)
	{
		score += value;
		GUI()->SetScore(score);

		if (vector != Vector::Zero)
		{
			FlowText* flow_text = GUI()->CreateFlowText();
			flow_text->Splash(vector, ToString(value));
			current_scene->AddObject(flow_text);
		}
	}

	void MarioGame::AddCoin()
	{
		++coins;
		if (coins >= 100)
		{
			AddLive();
			coins = 0;
			PlaySound("1-up");
		}

		GUI()->SetCoins(coins);
	}

	void MarioGame::AddLive()
	{
		GUI()->SetLives(++lives);

		Vector vector = current_scene->FindObjectByType<Creatures::Mario>()->GetBounds().Center();
		if (vector != Vector::Zero)
		{
			FlowText* flow_text = GUI()->CreateFlowText();
			flow_text->Splash(vector, "1 up");
			current_scene->AddObject(flow_text);
		}
	}

	void MarioGame::SetScore(int score)
	{
		this->score = score;
		GUI()->SetScore(this->score);
	}

	void MarioGame::SetEndLevelStatus()
	{
		game_state = GameState::LevelOver;
	}

	int MarioGame::GetScore() const
	{
		return score;
	}

	int MarioGame::GetGameTime() const
	{
		return game_time;
	}

	void MarioGame::InvicibleMode(bool value)
	{
		invicible_mode = value;

		if (!invicible_mode)
			UpdateMusic();
		else
		{
			current_scene->FindObjectByType<Creatures::Mario>()->SetInvicibleMode(true);
			StopMusic();
			PlayMusic("invicibility");
		}
	}

	void MarioGame::LoadLevel(const std::string& _level_name)
	{
		std::string level_name = _level_name;

		if (_level_name == "[NEXT_LEVEL]")
			level_name = NextLevelName();
		SetScene(new GameScene(MARIO_RES_PATH + "Levels/" + level_name + ".tmx"));
		GUI()->MoveToFront();
		current_stage_name = level_name;
	}

	void MarioGame::LoadSubLevel(const std::string& sublevel_name)
	{
		PushScene(new GameScene(MARIO_RES_PATH + "Levels/" + sublevel_name + ".tmx"));
		gui_object->MoveToFront();
	}

	void MarioGame::UnloadSubLevel()
	{
		PopScene();
	}

	Label* MarioGame::CreateText(const std::string& text, const Vector& pos)
	{
		Label* label = GUI()->CreateLabel();
		label->SetString(text);
		label->SetPosition(pos);
		return label;
	}

	void MarioGame::MarioDied()
	{
		LoadLevel(current_stage_name);
		--lives;
		if (lives <= 0)
			SetState(GameState::GameOver);
		else
			SetState(GameState::Status);
	}

	MarioGame* MarioGame::s_instance = nullptr;
	MarioGame* MarioGame::instance()
	{
		if (s_instance == nullptr)
			s_instance = new MarioGame();
		return s_instance;
	}

	void MarioGame::SyncMarioRank(GameObjects::GameObject* from_scene, GameObjects::GameObject* to_scene)
	{
		auto mario_rank = from_scene->FindObjectByName<Creatures::Mario>("Mario")->GetRank();
		to_scene->FindObjectByName<Creatures::Mario>("Mario")->SetRank(mario_rank);
	}

	void MarioGame::UpdateGUI()
	{
		GUI()->SetGameTime(game_time / 1000);
		GUI()->SetLevelName(level_name);
		GUI()->SetLives(lives);
		GUI()->SetCoins(coins);
		GUI()->SetScore(score);
		GUI()->SetMarioRank(current_scene->FindObjectByType<Creatures::Mario>()->GetRank());
	}

	void MarioGame::ClearScenes()
	{
		if (!scene_stack.empty())
		{
			for (auto scene : scene_stack)
				GetRootObject()->RemoveObject(scene);
			scene_stack.clear();
		}

		current_scene = nullptr;
	}

	void MarioGame::Init()
	{
		GetRootObject()->AddObject(gui_object = new MarioGUI());
		SetState(GameState::MainMenu);
	}

	void MarioGame::Update(int delta_time)
	{
		Game::Update(delta_time);
		timer.Update(delta_time);
		
		switch (game_state)
		{
			case GameState::MainMenu:
			{
				if (InputManager().IsButtonDown("Pause"))
					SetState(GameState::Status);

				break;
			}
			case GameState::Status:
			{
				delay_timer -= delta_time;
				if (delay_timer < 0)
					SetState(GameState::Playing);

				break;
			}
			case GameState::Playing:
			{
				if (game_state != GameState::MainMenu)
				{
					if (InputManager().IsButtonDown("Pause"))
					{
						PlaySound("pause");

						if (current_scene->IsEnabled())
						{
							current_scene->Disable();
							MusicManager().Pause();
							GUI()->Pause(true);
							GetTimer().Pause();
							break;
						}
						else
						{
							current_scene->Enable();
							MusicManager().Play();
							GUI()->Pause(false);
							GetTimer().Play();
						}
					}
				}

				if (!current_scene->IsEnabled())
					break;

				game_time -= delta_time;

				switch (time_out_state)
				{
					case TimeOutState::None:
					{
						if (game_time < 100000)
						{
							time_out_state = TimeOutState::StartWarning;
							StopMusic();
							PlaySound("warning");
						}

						break;
					}
					case TimeOutState::StartWarning:
					{
						if (game_time < 97000)
						{
							MusicManager().SetPitch(1.35f);
							UpdateMusic();
							time_out_state = TimeOutState::Warning;
						}
						break;
					}
					case TimeOutState::Warning:
					{
						if (game_time < 0)
						{
							current_scene->FindObjectByType<Creatures::Mario>()->SetState(Creatures::MarioState::Died);
							SetState(GameState::TimeOut);
						}
						break;
					}
					default:
						break;
				}

				GUI()->SetGameTime(game_time / 1000);

				break;
			}
			case GameState::LevelOver:
			{
				if (game_time > 0)
				{
					delay_timer -= delta_time;
					if (delay_timer < 0)
					{
						delay_timer = 12;
						game_time -= 1000;
						AddScore(50);

						static int i = 0; ++i;
						if (!(i % 4))
							PlaySound("beep");
						if (game_time < 0)
							game_time = 0;

						GUI()->SetGameTime(game_time / 1000);
					}
				}
				break;
			}
			case GameState::GameOver:
			{
				delay_timer -= delta_time;
				if (delay_timer < 0)
					SetState(GameState::MainMenu);
				break;
			}
			default:
				break;
		}
	}

	MarioGUI* MarioGame::GUI()
	{
		return gui_object;
	}

	void MarioGame::SetScene(GameObjects::GameObject* game_object)
	{
		if (current_scene)
			SyncMarioRank(current_scene, game_object);
		ClearScenes();
		current_scene = game_object;
		game_object->TurnOff();
		GetRootObject()->AddObject(game_object);
		scene_stack.push_back(game_object);
		level_name = game_object->CastTo<GameScene>()->GetLevelName();
	}

	void MarioGame::PushScene(GameObjects::GameObject* game_object)
	{
		if (current_scene)
		{
			SyncMarioRank(current_scene, game_object);
			current_scene->TurnOff();
		}

		current_scene = game_object;
		scene_stack.push_back(current_scene);
		GetRootObject()->AddObject(current_scene);

		UpdateMusic();
	}

	void MarioGame::PopScene()
	{
		if (scene_stack.empty())
			return;

		scene_stack.pop_back();
		GameObjects::GameObject* old_scene = current_scene;
		current_scene = scene_stack.back();
		SyncMarioRank(current_scene, old_scene);
		GetRootObject()->RemoveObject(old_scene);
		current_scene->TurnOn();

		UpdateMusic();
	}

	void MarioGame::Reset()
	{
		game_time = 300000;
		lives = 3;
		score = 0;
		coins = 0;
		delay_timer = 0;

		UpdateGUI();
	}

	void MarioGame::SetState(GameState state)
	{
		game_state = state;

		switch (state)
		{
			case GameState::MainMenu:
			{
				LoadLevel(first_stage_name);
				Reset();

				GUI()->SetState(GUIState::Menu);

				current_scene->Enable();
				current_scene->Update(0);
				current_scene->Disable();
				current_scene->Show();

				break;
			}
			case GameState::Status:
			{
				current_scene->TurnOff();
				GUI()->SetState(GUIState::Status);
				UpdateGUI();

				game_time = 300000;
				time_out_state = TimeOutState::None;
				MusicManager().SetPitch(1.f);
				delay_timer = 2500;
				StopMusic();

				break;
			}
			case GameState::Playing:
			{
				current_scene->Update(500);
				current_scene->TurnOn();
				GUI()->SetState(GUIState::Normal);
				UpdateMusic();

				break;
			}
			case GameState::GameOver:
			{
				sMarioGame->PlaySound("game_over");
				current_scene->TurnOff();
				GUI()->SetState(GUIState::GameOver);
				delay_timer = 5000;

				break;
			}
			default:
				break;
		}
	}

	void MarioGame::UpdateMusic()
	{
		if (invicible_mode)
			return;

		std::string back_picture = current_scene->FindObjectByType<GameObjects::Background>()->GetProperty("Picture").AsString();
		std::string music = "overworld";
		static std::map<std::string, std::string> backgroundToMusic =
		{
			{"Underground","underworld"},
			{"Water","underwater"},
			{"Castle","bowsercastle"}
		};
		if (backgroundToMusic.find(back_picture) != backgroundToMusic.end())
			music = backgroundToMusic[back_picture];
		StopMusic();
		PlayMusic(music);
	}

	std::string MarioGame::NextLevelName() const
	{
		int world = ToInt(current_stage_name[6] + std::string());
		int level = ToInt(current_stage_name[8] + std::string());

		if (level < 4)
			level++;
		else
		{
			assert(world < 8);
			world++;
			level = 1;
		}

		return "WORLD " + ToString(world) + "-" + ToString(level);
	}
}

