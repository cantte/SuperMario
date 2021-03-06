#ifndef GOOMBA_H
#define GOOMBA_h

#include "Enemy.h"
#include "Animator.h"

namespace Creatures
{
	class Goomba : public Enemy
	{
	public: 
		Goomba();

		virtual void Draw(sf::RenderWindow* render_window) override;
		virtual void Update(int delta_time) override;
		virtual void KickFromTop(Mario* mario) override;
		virtual void KickFromBottom(Mario* mario) override;
		virtual void TouchSide(Mario* mario) override;
		virtual bool IsAlive() const override;

	private:
		enum State
		{
			Deactivated,
			Normal,
			Cramped,
			Died
		} state = Deactivated;
		Animator animator;
		float timer;

		void SetState(State state);
	};
}

#endif // !GOOMBA_H
