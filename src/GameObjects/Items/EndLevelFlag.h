#ifndef END_LEVEL_FLAG_H
#define END_LEVEL_FLAG_H

#include "GameObject.h"
#include "Animator.h"

namespace Creatures
{
	class Mario;
}

namespace GameObjects
{
	class EndLevelFlag : public GameObject
	{
	public:
		EndLevelFlag();

		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;

	private:
		int cell_y = 0;
		GameEngine::Animator animator;
		Creatures::Mario* mario;
		bool touched = false;

		void Init() override;
	};
}

#endif // !END_LEVEL_FLAG_H
