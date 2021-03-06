#include "FireBar.h"
#include "MarioGame.h"
#include "Mario.h"

namespace GameObjects
{
	FireBar::FireBar()
	{
		animator.Create("fly", *sMarioGame->TextureManager().Get("Mario"), { { 0,0,16,16 }, { 16,0,16,16 }, { 16,0,-16,16 }, { 16,16,16,-16 } }, 0.01f);
	}

	void FireBar::Draw(sf::RenderWindow* render_window)
	{
		for (auto fire_position : fire_pos)
		{
			animator.SetPosition(fire_position);
			animator.Draw(render_window);
		}
	}

	void FireBar::Update(int delta_time)
	{
		animator.Update(delta_time);

		timer += delta_time;
		float angle = -speed * timer / 400;
		Vector rot = { std::cos(angle), std::sin(angle) };

		for (size_t i = 0; i < fire_pos.size(); ++i)
			fire_pos[i] = GetPosition() + rot * 16 * i - Vector(4, 4);

		Rect own_bounds = Rect(fire_pos.front(), fire_pos.back() - fire_pos.front());
		own_bounds.Normalize();
		Rect mario_bounds = mario->GetBounds();
		if (own_bounds.IsIntersect(mario_bounds))
		{
			for (auto fire_position : fire_pos)
			{
				if (mario_bounds.IsContain(fire_position + Vector(8, 8)))
				{
					mario->ReciveDamage();
					break;
				}
			}
		}

	}

	void FireBar::OnActivated()
	{
		int fires = GetProperty("Height").AsFloat() / 16.f;
		fire_pos.resize(fires);
	}

	void FireBar::Init()
	{
		speed = GetProperty("Speed").AsFloat();
		mario = GetParent()->FindObjectByName<Creatures::Mario>("Mario");
	}
}