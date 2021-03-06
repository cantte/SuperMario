#ifndef TWISTED_COIN_H
#define TWISTED_COIN_H

#include "GameObject.h"
#include "Animator.h"

namespace Creatures
{
	class Mario;
}

namespace GameObjects
{
	class TwistedCoin : public GameObject
	{
	public:
		TwistedCoin(const Vector& pos, Creatures::Mario* mario);

		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;

	private:
		Vector speed;
		GameEngine::Animator animator;
		int timer = 0;
	};
}

#endif // !TWISTED_COIN_H
