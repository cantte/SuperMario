#include "OneBrick.h"
#include "MarioGame.h"

using namespace GameEngine;

namespace GameObjects
{
	OneBrick::OneBrick(const Vector& pos, const Vector& speed)
	{
		SetPosition(pos);
		this->speed = speed;
		
		sprite_sheet.Load(*sMarioGame->TextureManager().Get("Items"), { { 96,0,16,16 }, { 96,16,16,-16 } });
		sprite_sheet.SetAnimType(AnimType::ForwardCycle);
		sprite_sheet.SetSpeed(0.005f);
	}

	void OneBrick::Draw(sf::RenderWindow* render_window)
	{
		sprite_sheet.SetPosition(GetPosition());
		sprite_sheet.Draw(render_window);
	}

	void OneBrick::Update(int delta_time)
	{
		sprite_sheet.Update(delta_time);
		speed.Y += delta_time * 0.0005f;
		Move(speed * delta_time);
		timer += delta_time;
		
		if (timer > 3000)
			GetParent()->RemoveObject(this);
	}
}