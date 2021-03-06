#ifndef FIREBALL_H
#define FIREBALL_H

#include "GameObject.h"
#include "Animator.h"

namespace Creatures
{
	class Mario;

	using namespace GameEngine;

	class FireBall : public GameObjects::GameObject
	{
	public:
		FireBall(const Vector& position, const Vector speed);

		virtual void Draw(sf::RenderWindow* render_window) override;
		virtual void Update(int delta_time) override;
		virtual void Init() override;

	private:
		int life_timer = 10000;
		Mario* mario = nullptr;
		Vector speed;
		Animator animator;
	};
}

#endif // !FIREBALL_H
