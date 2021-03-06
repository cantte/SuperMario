#ifndef JUMPER_H
#define JUMPER_H

#include "Item.h"
#include "SpriteSheet.h"

namespace GameObjects
{
	class Jumper : public Item
	{
	public:
		Jumper();

		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;

		virtual void CollisionResponse(Creatures::Mario* mario, CollisionTag& collisionTag, int delta_time) override;

	private:
		SpriteSheet* sprite_sheet;
		Creatures::Mario* mario = nullptr;
		bool zipped = false;
		float buttom = 0.f;
		int state = 0;
		float timer = 0;

		void OnActivated() override;
	};
}

#endif // !JUMPER_H
