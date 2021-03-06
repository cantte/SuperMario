#include "FireBall.h"
#include "Mario.h"
#include "MarioGame.h"

namespace Creatures
{
	FireBall::FireBall(const Vector& position, const Vector speed)
	{
		auto texture = sMarioGame->TextureManager().Get("Bowser");
		animator.Create("fire", *texture, { 0,364 }, { 32,36 }, 4, 1, 0.01f, AnimType::ForwardBackwardCycle);

		this->speed = speed;
		SetPosition(position);

		animator.FlipX(speed.X < 0);
		animator.Get("fire")->SetOrigin({ 16, 18 });
	}

	void FireBall::Draw(sf::RenderWindow* render_window)
	{
		animator.SetPosition(GetPosition());
		animator.Draw(render_window);
	}

	void FireBall::Update(int delta_time)
	{
		animator.Update(delta_time);
		life_timer -= delta_time;

		if (life_timer < 0)
			GetParent()->RemoveObject(this);
		
		Move(delta_time * speed);

		if (mario->GetBounds().IsContain(GetPosition()))
			mario->ReciveDamage();
	}

	void FireBall::Init()
	{
		mario = GetParent()->FindObjectByName<Mario>("Mario");
	}
}