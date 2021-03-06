#ifndef MARIO_GAME_H
#define MARIO_GAME_H

#include "Game.h"
#include "Timer.h"

#include <string>

namespace GameEngine
{
	const std::string MARIO_RES_PATH = "res/";

	class GameScene;
	class MarioGUI;
	class Label;

	class MarioGame : public Game
	{
	public:
		void ShowStatus();

		~MarioGame();

		Timer& GetTimer();

		void AddScore(int value, const Vector& vector = Vector::Zero);
		void AddCoin();
		void AddLive();
		
		void SetScore(int score);
		void SetEndLevelStatus();

		int GetScore() const;
		int GetGameTime() const;

		void InvicibleMode(bool value = true);
		void LoadLevel(const std::string& level_name);
		void LoadSubLevel(const std::string& sublevel_name);
		void UnloadSubLevel();

		Label* CreateText(const std::string& text, const Vector& pos);
		void MarioDied();

		static MarioGame* instance();

	private:
		MarioGame();

		static MarioGame* s_instance;

		Timer timer;
		MarioGUI* gui_object = nullptr;
		GameObjects::GameObject* current_scene = nullptr;
		std::vector<GameObjects::GameObject*> scene_stack;

		enum class GameState
		{
			MainMenu,
			Status,
			Playing,
			LevelOver,
			GameOver,
			TimeOut
		} game_state = GameState::MainMenu;

		enum class TimeOutState
		{
			None,
			StartWarning,
			Warning
		} time_out_state = TimeOutState::None;

		int delay_timer = 0;
		int game_time = 300000;
		int lives = 3;
		int score = 0;
		int coins = 0;

		std::string level_name;
		bool invicible_mode = false;

		const std::string first_stage_name = "WORLD 1-1";
		std::string current_stage_name;

		void SyncMarioRank(GameObjects::GameObject* from_scene, GameObjects::GameObject* to_scene);
		void UpdateGUI();
		void ClearScenes();

		virtual void Init() override;
		void Update(int delta_time) override;
		MarioGUI* GUI();

		void SetScene(GameObjects::GameObject* game_object);
		void PushScene(GameObjects::GameObject* game_object);
		void PopScene();
		void Reset();
		void SetState(GameState state);
		void UpdateMusic();
		std::string NextLevelName() const;

	};

	#define sMarioGame MarioGame::instance()
}

#endif // !MARIO_GAME_H
