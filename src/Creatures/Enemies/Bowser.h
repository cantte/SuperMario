#ifndef BOWSER_H
#define BOWSER_H

#include "Enemy.h"
#include "Animator.h"

namespace Creatures
{
	class Bowser : public Enemy
	{
	public:
		Bowser();

		virtual void Draw(sf::RenderWindow* render_window) override;
		virtual void Update(int delta_time) override;
		virtual void OnActivated() override;
		virtual void KickFromTop(Mario* mario) override;
		virtual void KickFromBottom(Mario* mario) override;
		virtual void TouchSide(Mario* mario) override;
		virtual bool IsAlive() const override;

		void NoBridge();

		const int walk_amplitude = 100;
		const int run_speed = 100;
		const int jump_period = 4000;

	private:
		const float anim_speed = 0.008f;

		enum class State
		{
			Walk,
			Turn,
			Jump,
			PreJump,
			MiddleFire,
			LandFire,
			NoBridge,
			Fall,
			Died
		} state = State::Walk;

		int lives = 5;
		int delay_timer = 0;
		Animator animator;
		float center_x;
		Vector old_speed;

		void SetState(State state);
	};
}

#endif // !BOWSER_H
