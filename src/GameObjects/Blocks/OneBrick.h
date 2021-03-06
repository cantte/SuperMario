#ifndef ONE_BRICK_H
#define ONE_BRICK_H

#include "GameObject.h"
#include "SpriteSheet.h"

namespace GameObjects
{
	class OneBrick : public GameObject
	{
	public:
		OneBrick(const Vector& position, const Vector& speed_vector);

		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;

	private:
		Vector speed;
		GameEngine::SpriteSheet sprite_sheet;
		int timer;
	};
}

#endif // !ONE_BRICK_H
