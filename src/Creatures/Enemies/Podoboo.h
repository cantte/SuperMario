#ifndef PODOBOO_H
#define PODOBOO_H

#include "Enemy.h"
#include "Animator.h"

namespace Creatures
{
	class Podoboo : public Enemy
	{
	public:
		Podoboo();

		virtual void KickFromTop(Mario* mario) override;
		virtual void KickFromBottom(Mario* mario) override;
		virtual void TouchSide(Mario* mario) override;
		virtual void Fired(Mario* mario) override;
		virtual bool IsAlive() const override;

		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;
		void OnActivated() override;

	private:
		float timer = 0;
		const float period_time = 3000.f;
		const float amplitude = 400.f;
		float acceleration = 0;
		float max_speed;
		Vector center;
		Vector speed;
		const Vector size = { 32,32 };
		Animator animator;
		sf::CircleShape shape;
	};
}

#endif // !PODOBOO_H
