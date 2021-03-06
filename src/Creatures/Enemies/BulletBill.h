#ifndef BULLET_BILL_H
#define BULLET_BILL_H

#include "Enemy.h"
#include "Animator.h"

namespace Creatures
{
	class BulletBill : public Enemy
	{
	public:
		BulletBill(const Vector& position, const Vector& speed);

		virtual void Draw(sf::RenderWindow* render_window) override;
		virtual void Update(int delta_time) override;
		virtual void KickFromTop(Mario* mario) override;
		virtual void KickFromBottom(Mario* mario) override;
		virtual void TouchSide(Mario* mario) override;
		virtual bool IsAlive() const override;
		virtual void Init() override;

	private:
		enum State
		{
			Normal,
			Died
		} state = State::Normal;
		
		Animator animator;

		void SetState(State state);
	};

	class BulletBillSpawner : public GameObjects::GameObject
	{
	public:
		BulletBillSpawner();

		void Update(int delta_time) override;
		void OnActivated() override;
		void Init() override;

	private:
		Mario* mario;
		int blocks_width = 0;
		float spawn_timer;

		const int spawn_interval = 4000;
		const float bullet_speed = 0.15f;

		bool IsBulletBillBeyondTiledMap() const;
	};
}

#endif // !BULLET_BILL_H