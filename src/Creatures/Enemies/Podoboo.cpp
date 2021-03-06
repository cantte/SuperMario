#include "Podoboo.h"
#include "Mario.h"
#include "MarioGame.h"

namespace Creatures
{
	Podoboo::Podoboo()
	{
		shape.setRadius(16);
		shape.setFillColor(sf::Color::Red);
		SetSize({ 32, 32 });

		const sf::Texture& texture = *sMarioGame->TextureManager().Get("Enemies");
		animator.Create("up", texture, Vector(192, 80), Vector(32, 32), 3, 1, 0.005f);
		animator.Create("down", texture, Vector(192, 112), Vector(32, -32), 3, 1, 0.005f);
	}

	void Podoboo::KickFromTop(Mario* mario)
	{
		mario->ReciveDamage();
	}

	void Podoboo::KickFromBottom(Mario* mario)
	{

	}

	void Podoboo::TouchSide(Mario* mario)
	{
		mario->ReciveDamage();
	}

	void Podoboo::Fired(Mario* mario)
	{

	}

	bool Podoboo::IsAlive() const
	{
		return true;
	}

	void Podoboo::Draw(sf::RenderWindow* render_window)
	{
		animator.SetPosition(GetPosition());
		animator.Draw(render_window);
	}

	void Podoboo::Update(int delta_time)
	{
		Enemy::Update(delta_time);

		timer += delta_time;

		if (GetPosition().Y > center.Y)
			speed += Vector::Up * acceleration * delta_time;
		else
			speed += Vector::Down * acceleration * delta_time;

		if (timer > period_time)
		{
			SetPosition(center);
			speed = Vector::Up * acceleration * period_time * 0.25;
			timer = 0;
		}

		animator.Update(delta_time);
		Move(speed * delta_time);
		animator.Play((speed.Y < 0) ? "down" : "up");
	}

	void Podoboo::OnActivated()
	{
		center = GetPosition();
		acceleration = amplitude / (period_time * period_time * 0.0625f);
		speed = Vector::Up * acceleration * period_time * 0.25f;
	}
}