#ifndef HAMMER_H
#define HAMMER_H

#include "GameObject.h"
#include "Animator.h"

namespace Creatures
{
	class Mario;

	class Hammer : public GameObjects::GameObject
	{
	public:
		Hammer(Mario* target);

		void Draw(sf::RenderWindow* render_window);
		virtual void Update(int delta_time) override;

		void ThrowAway(const Vector& speed);

	private:
		enum State
		{
			InHand,
			Fly
		} state = State::InHand;

		GameObject* parent;
		Vector direction;
		const float gravity_force = 0.0015f;
		Mario* target;
		GameEngine::Animator animator;
		Vector speed;
	};
}

#endif // !HAMMER_H
