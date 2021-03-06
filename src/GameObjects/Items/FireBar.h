#ifndef FIRE_BAR_H
#define FIRE_BAR_H

#include "GameObject.h"
#include "Animator.h"

namespace Creatures
{
	class Mario;
}

namespace GameObjects
{
	class FireBar : public GameObject
	{
	public:
		FireBar();

		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;

	protected:
		void OnActivated() override;
		void Init() override;

	private:
		GameEngine::Animator animator;
		Creatures::Mario* mario;
		sf::CircleShape shape;
		std::vector<Vector> fire_pos;
		float timer = 0;
		float speed = -1;
	};
}

#endif // !FIRE_BAR_H
