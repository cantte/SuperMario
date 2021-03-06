#include "Platform.h"
#include "Mario.h"
#include "MarioGame.h"
#include "Blocks.h"

namespace GameObjects
{
	void Platform::CollisionResponse(Creatures::Mario* mario, CollisionTag& collision_tag, int delta_time)
	{
		mario->SetPosition(GameEngine::CollsionResponse(mario->GetBounds(), mario->GetSpeed(), GetBounds(), GetSpeedVector(), delta_time, collision_tag));
	}

	MoveablePlatform::MoveablePlatform()
	{
		platform_type = PlatformType::NoInit;
		amplitude = 0;
	}

	void MoveablePlatform::Draw(sf::RenderWindow* render_window)
	{
		sprite.setPosition(GetPosition());
		render_window->draw(sprite);
	}

	void MoveablePlatform::Update(int delta_time)
	{
		switch (platform_type)
		{
			case PlatformType::Vertical:
			case PlatformType::Horizontal:
			{
				int k = 1;

				if ((platform_type == PlatformType::Horizontal && GetPosition().X > center.X) ||
					(platform_type == PlatformType::Vertical && GetPosition().Y > center.Y))
					k = -1;

				timer += delta_time;
				speed += k * orientation * acceleration * delta_time;

				break;
			}
			case PlatformType::Elevator:
			{
				static const int button = GetParent()->FindObjectByType<Blocks>()->Height();

				if (speed.Y > 0 && GetPosition().Y > buttom)
					SetPosition({ center.X, -16.f });
				if (speed.Y < 0 && GetPosition().Y < 0)
					SetPosition({ center.X, buttom + 16.f });

				break;
			}
			default:
				break;
		}

		Move(speed * delta_time);
	}

	Vector MoveablePlatform::GetSpeedVector()
	{
		return 2 * speed;
	}

	void MoveablePlatform::CollisionResponse(Creatures::Mario* mario, CollisionTag& collision_tag, int delta_time)
	{
		Platform::CollisionResponse(mario, collision_tag, delta_time);

		if (collision_tag & CollisionTag::Floor)
		{
			if (platform_type == PlatformType::Elevator)
			{
				Vector mario_pos = mario->GetPosition();

				mario->SetPosition(Vector(mario_pos.X, GetPosition().Y - mario->GetBounds().Size().Y) +
					delta_time * GetSpeedVector() +
					(speed.Y < 0 ? 2 * Vector::Down : 3 * Vector::Up));
			}
			else if (platform_type == PlatformType::Skate)
			{
				if (speed == Vector::Zero)
					speed = Vector::Right * SKATE_SPEED;
			}
			else
			{
				if (speed.Y < 0)
					mario->Move({ 0.f, -GetPosition().Y + mario->GetBounds().Bottom() - 2 });
			}
		}
	}

	void MoveablePlatform::Init()
	{
		const int height = 16;
		mario = GetParent()->FindObjectByName<Creatures::Mario>("Mario");

		platform_type = (PlatformType)GetProperty("Orientation").AsInt();
		amplitude = GetProperty("Amplitude").AsInt();
		period_time = GetProperty("Period").AsFloat() * 1000;
		size = GetProperty("Width").AsFloat();
		center = GetPosition();

		assert(platform_type != PlatformType::NoInit);

		sprite.setTexture(*sMarioGame->TextureManager().Get("Items"));
		sprite.setTextureRect({ 0, 0, size, height });
		
		SetSize({ size, height });
		timer = 0;

		switch (platform_type)
		{
			case PlatformType::Elevator:
			{
				buttom = GetParent()->FindObjectByType<Blocks>()->Height();
				speed.Y = buttom / period_time;

				break;
			}
			case PlatformType::Horizontal:
			case PlatformType::Vertical:
			{
				orientation = (platform_type == PlatformType::Horizontal) ? Vector::Right : Vector::Down;
				acceleration = 2 * (amplitude) / std::pow(period_time * 0.25f, 2);
				speed = acceleration * (period_time * 0.25f) * orientation;

				SetPosition(center);

				break;
			}
			case PlatformType::Skate:
			{
				sprite.setTextureRect({ 112, 0, size, height });
				orientation = Vector::Right;
				speed = Vector::Zero;

				break;
			}
			default:
				break;
		}

		if (GetProperty("Phase").AsInt() != 0)
			speed = -speed;
	}

	FallingPlatform::FallingPlatform()
	{

	}

	void FallingPlatform::Draw(sf::RenderWindow* render_window)
	{
		sprite.setPosition(GetPosition());
		render_window->draw(sprite);
	}

	void FallingPlatform::Update(int delta_time)
	{
		if (stayed)
		{
			speed += Vector::Down * delta_time * 0.008f;
			stayed = false;

			if (moving_callback)
				moving_callback();
		}
		else
		{
			speed += Vector::Up * math::sign(speed.Y) * 0.001f * delta_time;
			if (std::abs(speed.Y < 0.05f))
				speed = Vector::Zero;
		}
	}

	Vector FallingPlatform::GetSpeedVector()
	{
		return speed;
	}

	void FallingPlatform::CollisionResponse(Creatures::Mario* mario, CollisionTag& collision_tag, int delta_time)
	{
		Platform::CollisionResponse(mario, collision_tag, delta_time);

		if (collision_tag & CollisionTag::Floor)
		{
			stayed = true;
			this->mario = mario;
		}
	}

	void FallingPlatform::AddImpulse(const Vector& speed)
	{
		this->speed += speed;
	}

	void FallingPlatform::SetSpeed(const Vector& speed)
	{
		this->speed = speed;
	}

	void FallingPlatform::SetMovingCallback(const std::function<void()>& func)
	{
		moving_callback = func;
	}

	void FallingPlatform::OnActivated()
	{
		int size = GetProperty("With").AsFloat();
		Vector center = GetPosition();

		SetBounds(Rect(center - Vector(size, 16) / 2, Vector(size, 16)));

		sprite.setTexture(*sMarioGame->TextureManager().Get("Items"));
		sprite.setTextureRect({ 0, 0, size, 16 });
	}

	void FallingPlatform::Init()
	{

	}
}