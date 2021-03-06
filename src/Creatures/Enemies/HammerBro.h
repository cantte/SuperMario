#ifndef HAMMER_BRO_H
#define HAMMER_BRO_H

#include "Enemy.h"
#include "Animator.h"

namespace Creatures
{
	class Hammer;

	class HammerBro : public Enemy
	{
	public:
		HammerBro();

		virtual void Draw(sf::RenderWindow* render_window) override;
		virtual void Update(int delta_time) override;
		virtual void OnActivated() override;
		virtual void KickFromTop(Mario* mario) override;
		virtual void KickFromBottom(Mario* mario) override;
		virtual void TouchSide(Mario* mario) override;
		virtual bool IsAlive() const override;

	private:
		const int fire_rate = 1200;
		const int jump_rate = 5000;
		const int walk_amplitude = 25;

		bool collision_on = true;

		enum State
		{
			Normal,
			Died
		}state = State::Normal;

		Vector jump_direction = Vector::Up;
		Hammer* hammer = nullptr;
		Animator animator;
		int center_x = 0;
		float jump_timer = 0;
		float fire_timer = 0;
		float drop_off_height = 0;

		bool IsCanJumpUp() const;
		bool IsCanJumpDown() const;
		void SetState(State state);
	};
}

#endif // !HAMMER_BRO_H
