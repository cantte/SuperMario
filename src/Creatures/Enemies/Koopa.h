#ifndef KOOPA_H
#define KOOPA_H

#include "Enemy.h"
#include "Animator.h"

namespace Creatures
{
	class Koopa : public Enemy
	{
	public:
		Koopa();

		virtual void KickFromTop(Mario* mario) override;
		virtual void KickFromBottom(Mario* mario) override;
		virtual bool IsAlive() const override;
		virtual void TouchSide(Mario* mario) override;
		virtual void Draw(sf::RenderWindow* render_window) override;
		virtual void Update(int delta_time) override;
		virtual bool IsInBulletState() const override;

	private:
		const Vector full_size = { 32, 48 };
		const Vector hidden_size = { 32, 32 };

		enum State
		{
			Deactivated,
			Normal,
			Jumping,
			Levitating,
			Hidden,
			Climb, 
			Bullet,
			Died
		} state = Deactivated;

		Animator animator;
		int flying_mode = 0;
		float timer = 0;
		Vector initial_pos;

		void OnActivated() override;
		void SetState(State state);
	};
}

#endif // !KOOPA_H
