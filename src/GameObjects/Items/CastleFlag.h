#ifndef CASTLE_FLAG_H
#define CASTLE_FLAG_H

#include "GameObject.h"
#include "Animator.h"

namespace GameObjects
{
	class CastleFlag : public GameObject
	{
	public:
		CastleFlag();

		void LiftUp();

		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;

	private:
		int pos_y;
		GameEngine::Animator animator;

		void Init() override;
	};
}

#endif // !CASTLE_FLAG_H
