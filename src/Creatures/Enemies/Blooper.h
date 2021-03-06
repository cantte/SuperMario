#ifndef BLOOPER_H
#define BLOOPER_H

#include "Enemy.h"
#include "Animator.h"

namespace Creatures
{
	class Blooper : public Enemy
	{
	public:
		Blooper();

		virtual void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;
		virtual void KickFromTop(Mario* mario) override;
		virtual void KickFromBottom(Mario* mario) override;
		virtual void TouchSide(Mario* mario) override;
		virtual bool IsAlive() const override;

	private:
		enum State
		{
			Zig,
			Zag,
			Died
		} state = State::Zig;

		int delay_time = 0;
		Vector speed;
		Animator animator;

		void SetState(State state);
	};
}

#endif // !BLOOPER_H
