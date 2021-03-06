#ifndef CHEP_CHEP_H
#define CHEP_CHEP_H

#include "Enemy.h"
#include "Animator.h"

namespace Creatures
{
	class CheepCheep : public Enemy
	{
	public:
		CheepCheep();
		CheepCheep(const Vector& initial_pos, const Vector& initial_speed);

		virtual void Draw(sf::RenderWindow* render_window) override;
		virtual void Update(int delta_time) override;
		virtual void KickFromTop(Mario* mario) override;
		virtual void KickFromBottom(Mario* mario) override;
		virtual void TouchSide(Mario* mario) override;
		virtual bool IsAlive() const override;

	private:
		enum State
		{
			Normal,
			Underwater,
			Died
		} state = State::Normal;

		Animator animator;

		void SetState(State state);
	};

	class CheepCheepSpawner : public GameObjects::GameObject
	{
	public:
		CheepCheepSpawner();

		void Update(int delta_time) override;
		void OnActivated() override;
		void Init() override;

	private:
		const int spawn_interval = 1000;
		const Vector spawn_speed = { 0.32f, -0.65f };

		float spawn_timer = 0;
		int map_height;
		Mario* mario = nullptr;
	};
}

#endif // !CHEP_CHEP_H
