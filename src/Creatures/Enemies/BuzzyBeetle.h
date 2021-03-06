#ifndef BUZZY_BEETLE_H
#define BUZZY_BEETLE_H

#include "Enemy.h"
#include "Animator.h"

namespace Creatures
{
	class BuzzyBeetle : public Enemy
	{
	public:
		BuzzyBeetle();

		virtual void KickFromTop(Mario* mario) override;
		virtual void KickFromBottom(Mario* mario) override;
		virtual void Fired(Mario* mario) override { }
		virtual bool IsAlive() const override;
		virtual void TouchSide(Mario* mario) override;
		virtual void Draw(sf::RenderWindow* render_window) override;
		virtual void Update(int delta_time) override;
		virtual bool IsInBulletState() const override;

	private:
		enum State
		{
			Deactivated,
			Normal,
			Hidden,
			Bullet,
			Died
		} state = Deactivated;

		Animator animator;
		bool is_flying = false;
		float timer;

		void SetState(State state);
	};
}

#endif // !BUZZY_BEETLE_H
