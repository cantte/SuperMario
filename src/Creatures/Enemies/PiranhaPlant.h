#ifndef PIRANHA_PLANT_H
#define PIRANHE_PLANT_H

#include "Enemy.h"

namespace Creatures
{
	class PiranhaPlant : public Enemy
	{
	public:
		PiranhaPlant();

		virtual void KickFromTop(Mario* mario) override;
		virtual void KickFromBottom(Mario* mario) override;
		virtual void TouchSide(Mario* mario) override;
		virtual void Fired(Mario* mario) override;
		virtual bool IsAlive() const override;
		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;
		void OnActivated() override;
		void HideInTube();

	private:
		const Vector size = { 32,46 };
		const float period_time = 2000;
		sf::Sprite sprite;
		float timer = 0;
		float buttom;
		bool dead_zone = false;
	};
}

#endif // !PIRANHA_PLANT_H
