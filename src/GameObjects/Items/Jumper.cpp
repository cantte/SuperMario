#include "Jumper.h"
#include "MarioGame.h"
#include "Mario.h"
#include "SpriteSheet.h"

namespace GameObjects
{
	Jumper::Jumper()
	{
		sprite_sheet = new SpriteSheet();
		sprite_sheet->Load(*sMarioGame->TextureManager().Get("Items"), { { 0,20,32,64 },{ 32,36,32,48 },{ 64,52,32,32 } });
		AddObject(sprite_sheet);
	}

	void Jumper::Draw(sf::RenderWindow* render_window)
	{
		sprite_sheet->SetPosition(GetPosition());
		sprite_sheet->Draw(render_window);
	}

	void Jumper::Update(int delta_time)
	{
		if (zipped)
		{
			timer += delta_time;

			int new_state = int(timer / 100) % 4 + 1;
			if (new_state == 4)
			{
				new_state = 0;
				zipped = false;
			}
			if (new_state != state)
			{
				state = new_state;

				int index = state;
				if (index > 2) index = 1;

				Rect old_bounds = GetBounds();
				old_bounds.SetTop(buttom - 64 + 16 * index);
				SetBounds(old_bounds);
				sprite_sheet->SetSpriteIndex(index);

				if (state == 2)
					mario->AddImpulse(Vector::Up * 0.5);
			}
		}
	}

	void Jumper::CollisionResponse(Creatures::Mario* mario, CollisionTag& collisionTag, int delta_time)
	{
		if (mario->GetSpeed().Y >= 0)
		{
			mario->SetPosition(GameEngine::CollsionResponse(mario->GetBounds(), mario->GetSpeed(), GetBounds(), Vector::Zero, delta_time, collisionTag));
			mario->SetSpeed({ mario->GetSpeed().X, 0.f });
		}

		if (collisionTag == CollisionTag::Floor)
		{
			this->mario = mario;

			if (state == 0)
			{
				timer = 0;
				zipped = true;
			}
		}
	}

	void Jumper::OnActivated()
	{
		SetSize({ 32, 64 });
		buttom = GetBounds().Bottom();
	}
}