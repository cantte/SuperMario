#ifndef END_LEVEL_KEY_H
#define END_LEVEL_KEY_H

#include "GameObject.h"

namespace Creatures
{
	class Bowser;
	class Mario;
}

namespace GameObjects
{
	class Blocks;

	class EndLevelKey : public GameObject
	{
	public:
		EndLevelKey();

		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;

	private:
		enum class State
		{
			Play,
			BridgeHiding,
			BowserRun,
			BowserFall,
			MarioGoToPrincess,
			TextShow,
			GotToNextLevel
		} state = State::Play;

		sf::Sprite sprite;
		int delay_timer = 0;
		std::vector<Vector> bridge_blocks;
		Creatures::Mario* mario = nullptr;
		Creatures::Bowser* bowser = nullptr;
		Blocks* blocks = nullptr;

		void SetState(State state);
		void Init() override;
	};
}

#endif // !END_LEVEL_KEY_H
