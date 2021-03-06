#ifndef MARIO_BULLET_H
#define MARIO_BULLET_H

#include "GameObject.h"
#include "Animator.h"

namespace GameObjects
{
	class Blocks;
}

namespace Creatures
{
	class MarioBullet : public GameObjects::GameObject
	{
	public:
		MarioBullet(const Vector& pos, const Vector& direction);
		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;

	private:
		enum class State
		{
			Fly,
			Splash
		} state = State::Fly;

		const float bullet_speed = 0.33f;
		const float gravity_force = 0.0015f;

		Vector direction;
		float timer;
		Vector speed;
		GameObjects::Blocks* blocks;
		GameEngine::Animator animator;

		void SetState(State state);
		void Init() override;
	};
}

#endif // !MARIO_BULLET_H
