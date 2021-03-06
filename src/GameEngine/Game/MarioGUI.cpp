#include "MarioGUI.h"
#include "MarioState.h"
#include "Text.h"
#include "MarioGame.h"
#include "Animator.h"
#include "GameEngine.h"

#include <sstream>
#include <iomanip>

namespace GameEngine
{
	MarioGUI::MarioGUI()
	{
		const int y_gui_pos = 5;

		score = new Label();
		score->SetFontName(*sMarioGame->FontManager().Get("some_font"));
		score->SetPosition({ 70, y_gui_pos });
		score->SetFontStyle(sf::Text::Bold);
		score->SetFontColor({ 255, 255, 220 });
		score->SetFontSize(40);
		score->SetTextAlign(Label::TextAling::Left);
		AddObject(score);

		coin_counter = CreateLabel();
		coin_counter->SetPosition({ 490, y_gui_pos });
		AddObject(coin_counter);

		world = CreateLabel();
		world->SetPosition({ 720, y_gui_pos });
		AddObject(world);

		timer_lab = CreateLabel();
		timer_lab->SetPosition({ 1080, y_gui_pos });
		AddObject(timer_lab);

		level_name = CreateLabel();
		level_name->SetPosition(sMarioGame->ScreenSize() / 2.f + Vector::Up * 100.f);
		level_name->SetTextAlign(Label::TextAling::Center);
		AddObject(level_name);

		lives = CreateLabel();
		lives->SetPosition(sMarioGame->ScreenSize() / 2.f + Vector(-15, -18));
		AddObject(lives);

		sf::Texture& texture = *sMarioGame->TextureManager().Get("Mario");
		mario_pix = new Animator();
		mario_pix->Create("small", texture, { 0, 96, 32, 32 });
		mario_pix->SetSpriteOffset("small", 0, Vector::Down * 22.f);
		mario_pix->Create("big", texture, { 0, 32, 32, 64 });
		mario_pix->SetPosition(sMarioGame->ScreenSize() / 2.f + Vector(-64, -44));
		mario_pix->Play("big");
		mario_pix->Scale(1.3f, 1.3f);
		mario_pix->Hide();
		AddObject(mario_pix);
		fire_pallete.Create({ sf::Color(202, 77, 62), sf::Color(132, 133, 30) }, { sf::Color(255, 255, 255), sf::Color(202, 77, 62) });

		flow_text = new FlowText(*sMarioGame->FontManager().Get("main_font"));
		flow_text->SetTextColor(sf::Color::Red);
		flow_text->SetSplashVector({ 0, -3 });
		flow_text->SetTextSize(14);
		AddObject(flow_text);

		game_logo = new Label(sf::Sprite(*sMarioGame->TextureManager().Get("Logo"), { 0,0, 750,300 }));
		game_logo->SetPosition(280, 70);
		AddObject(game_logo);

		menu_selector = new Label(sf::Sprite(*sMarioGame->TextureManager().Get("Items"), { 128, 150, 32, 32 }));
		menu_selector->SetPosition(450, 435);
		AddObject(menu_selector);

		one_player = CreateLabel();
		one_player->SetPosition(sMarioGame->ScreenSize() / 2.f + Vector::Down * 80.f);
		one_player->SetTextAlign(Label::TextAling::Center);
		one_player->SetString("1 PLAYER GAME");
		one_player->SetFontColor(sf::Color(22, 85, 122));
		AddObject(one_player);

		two_player = one_player->Clone();
		two_player->SetString("2 PLAYER GAME");
		two_player->SetPosition(sMarioGame->ScreenSize() / 2.f + Vector::Down * 135.f);
		AddObject(two_player);

		one_player->SetFontColor({ 0, 0, 0 });

		coin = new Animator();
		coin->Create("twist", *sMarioGame->TextureManager().Get("Items"), Vector(0, 84), Vector(32, 32), 4, 1, 0.01f);
		coin->Play("twist");
		coin->SetPosition(458, 15);
		AddObject(coin);

		game_over = CreateLabel();
		game_over->SetPosition(sMarioGame->ScreenSize() / 2.f);
		game_over->SetTextAlign(Label::TextAling::Center);
		game_over->SetString("GAME OVER");
		AddObject(game_over);

		paused = CreateLabel();
		paused->SetPosition(sMarioGame->ScreenSize() / 2.f);
		paused->SetTextAlign(Label::TextAling::Center);
		paused->SetString("PAUSED");
		AddObject(paused);
		paused->Hide();
	}

	void MarioGUI::SetScore(int score)
	{
		std::stringstream str_stream;
		str_stream << "MARIO: " << std::setw(6) << std::setfill('0') << score;
		this->score->SetString(str_stream.str());
	}

	void MarioGUI::SetCoins(int coins)
	{
		std::stringstream str_stream;
		str_stream << "x" << std::setw(2) << std::setfill('0') << coins;
		coin_counter->SetString(str_stream.str());
	}

	void MarioGUI::SetGameTime(int time)
	{
		std::stringstream str_stream;
		str_stream << "TIME: " << std::setw(3) << std::setfill('0') << time;
		timer_lab->SetString(str_stream.str());
	}

	void MarioGUI::SetLevelName(const std::string& str)
	{
		level_name->SetString(str);
		world->SetString(str);
	}

	void MarioGUI::SetLives(int lives)
	{
		this->lives->SetString("  x  " + GameEngine::ToString(lives));
	}

	void MarioGUI::SetMarioRank(Creatures::MarioRank rank)
	{
		if (rank == Creatures::MarioRank::Small)
			mario_pix->Play("small");
		else
			mario_pix->Play("big");

		if (rank == Creatures::MarioRank::Fire)
			mario_pix->SetPallete(&fire_pallete);
		else
			mario_pix->SetPallete(nullptr);
	}

	void MarioGUI::SetState(GUIState state)
	{
		this->state = state;

		GameObject* states_labs[3][5] = {
			{level_name, lives, mario_pix, nullptr, nullptr},
			{game_logo, one_player, menu_selector, two_player, nullptr},
			{game_over, nullptr, nullptr, nullptr, nullptr}
		};

		for (auto state_labs : states_labs)
			for (int i = 0; state_labs[i]; ++i)
				state_labs[i]->TurnOff();

		if (state == GUIState::Normal)
			return;

		auto state_labs = states_labs[(int)state - 1];
		for (int i = 0; state_labs[i]; ++i)
			state_labs[i]->TurnOn();
	}

	FlowText* MarioGUI::CreateFlowText()
	{
		return flow_text->Clone();
	}

	Label* MarioGUI::CreateLabel()
	{
		return score->Clone();
	}

	void MarioGUI::Update(int delta_time)
	{
		GameObject::Update(delta_time);

		if (state == GUIState::Menu)
		{
			timer += delta_time;
			float x = 0.85f + 0.15f * std::cos(timer * 0.01f);
			menu_selector->SetPosition(450 + 16 * (1 - x), 435 + 16 * (1 - x));
			menu_selector->GetSprite().setScale(x, x);
		}
	}

	void MarioGUI::Pause(bool is_paused)
	{
		if (is_paused)
			paused->Show();
		else
			paused->Hide();
	}

	void MarioGUI::PostDraw(sf::RenderWindow* render_window)
	{

	}

	void MarioGUI::Draw(sf::RenderWindow* render_window)
	{
		GameObject::Draw(render_window);
	}
}