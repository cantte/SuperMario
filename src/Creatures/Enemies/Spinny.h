#ifndef SPINNY_H
#define SPINNY_H

#include "Enemy.h"
#include "Animator.h"

namespace Creatures
{
	class Spinny : public Enemy
	{
	public:
		Spinny(const Vector& position, const Vector& speed, const Vector& walk_direction);

		virtual void Draw(sf::RenderWindow* render_window) override;
		virtual void Update(int delta_time) override;
		virtual void KickFromTop(Mario* mario) override;
		virtual void KickFromBottom(Mario* mario) override;
		virtual void TouchSide(Mario* mario) override;
		virtual bool IsAlive() const override;

	private:
		enum State
		{
			Egg,
			Normal,
			Died
		} state = Egg;

		Vector walk_direction;
		Animator animator;

		void SetState(State state);
	};
}

#endif // !SPINNY_H
