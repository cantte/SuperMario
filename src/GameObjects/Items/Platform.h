#ifndef PLATFORM_H
#define PLATFORM_H

#include "Item.h"

namespace GameObjects
{
	class Platform : public Item
	{
	public:
		virtual Vector GetSpeedVector() = 0;

		void CollisionResponse(Creatures::Mario* mario, CollisionTag& collision_tag, int delta_time) override;
	};

	class MoveablePlatform : public Platform
	{
	public:
		MoveablePlatform();

		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;
		Vector GetSpeedVector() override;

		virtual void CollisionResponse(Creatures::Mario* mario, CollisionTag& collision_tag, int delta_time) override;

	protected:
		void Init() override;

	private:
		const float SKATE_SPEED = 0.17f;

		PlatformType platform_type;
		Vector center;
		int size;
		int amplitude;
		float period_time = 0;
		int buttom = 0;
		float timer = 0;
		sf::Sprite sprite;
		Vector speed;
		float last_delta;
		float acceleration = 0;
		Vector orientation;
		Creatures::Mario* mario = nullptr;
	};

	class FallingPlatform : public Platform
	{
	public:
		FallingPlatform();

		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;
		Vector GetSpeedVector() override;
		void CollisionResponse(Creatures::Mario* mario, CollisionTag& collision_tag, int delta_time) override;

		void AddImpulse(const Vector& speed);
		void SetSpeed(const Vector& speed);
		void SetMovingCallback(const std::function<void()>& func);

	protected:
		void OnActivated() override;
		void Init() override;

	private:
		std::function<void()> moving_callback;
		sf::Sprite sprite;
		Vector speed;
		bool stayed = false;
		Creatures::Mario* mario;

	};
}

#endif // !PLATFORM_H
