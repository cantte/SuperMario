#ifndef LAKITY_H
#define LAKITY_H

#include "Enemy.h"
#include "Animator.h"

namespace Creatures
{
	class Lakity : public Enemy
	{
	public:
		Lakity();

		virtual void Draw(sf::RenderWindow* render_window) override;
		virtual void Update(int delta_time) override;
		virtual void KickFromTop(Mario* mario) override;
		virtual void KickFromBottom(Mario* mario) override;
		virtual void TouchSide(Mario* mario) override;
		virtual bool IsAlive() const override;

		void Escape(const Vector& escape_direction);
		void Init() override;

	private:
		const int fire_rate = 2500;
		float fire_timer = 0;
		float died_timer = 0;

		enum State
		{
			Normal,
			Died,
			RunAway
		} state = State::Normal;

		Animator animator;

		void SetState(State state);
	};

	class LakitySpawner : public GameObjects::GameObject
	{
	public:
		LakitySpawner() { }

		void Update(int delta_time) override;

	protected:
		void OnActivated() override;
		void Init() override;

	private:
		const int check_interval = 5000;
		Lakity* lakity = nullptr;
		Mario* mario = nullptr;
		float lakity_checker_timer = 0;
	};
}

#endif // !LAKITY_H
