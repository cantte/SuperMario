#ifndef PRINCESS_H
#define PRINCESS_H

#include "GameObject.h"
#include "Animator.h"

namespace GameObjects
{
	class Princess : public GameObject
	{
	public:
		Princess();

		virtual void Draw(sf::RenderWindow* render_window) override;

	private:
		GameEngine::Animator animator;
	};
}

#endif // !PRINCESS_H
