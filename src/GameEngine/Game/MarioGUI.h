#ifndef MARIO_GUI_H
#define MARIO_GUI_H

#include "GameObject.h"
#include "Pallete.h"

namespace Creatures
{
	enum class MarioRank;
}

namespace GameEngine
{
	enum class GUIState
	{
		Normal,
		Status,
		Menu,
		GameOver
	};

	class Animator;
	class FlowText;
	class Label;

	class MarioGUI : public GameObjects::GameObject
	{
	public:
		MarioGUI();

		void SetScore(int score);
		void SetCoins(int coins);
		void SetGameTime(int time);
		void SetLevelName(const std::string& str);
		void SetLives(int lives);
		void SetMarioRank(Creatures::MarioRank rank);
		void SetState(GUIState state);

		FlowText* CreateFlowText();
		Label* CreateLabel();

		void Update(int delta_time) override;
		void Pause(bool is_paused);

	protected:
		GUIState state;

		void PostDraw(sf::RenderWindow* render_window) override;
		void Draw(sf::RenderWindow* render_window) override;

	private:
		int timer = 0;
		Animator* mario_pix = nullptr;
		Animator* coin = nullptr;
		Pallete fire_pallete;

		Label* score, *coin_counter, *world, *timer_lab, *level_name,
			*lives, *game_logo, *one_player, *two_player, *menu_selector,
			*game_over, *paused;

		FlowText* flow_text;
	};
}

#endif // !MARIO_GUI_H
