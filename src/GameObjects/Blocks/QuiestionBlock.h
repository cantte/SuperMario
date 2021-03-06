#ifndef QUESTION_BLOCK_H
#define QUESTION_BLOCK_H

#include "Block.h"
#include "Blocks.h"
#include "MarioGame.h"

namespace GameObjects
{
	template <class T>
	class QuestionBlock : public Block
	{
	public:
		QuestionBlock(BlockTitleCode id, bool isInvisible = false, bool isBrickStyle = false) : Block(id)
		{
			SetInvisible(isInvisible);
			
			current_sprite = (isBrickStyle) ? &bricket_sprite : &question_block_sprite;
		}

		void Draw(sf::RenderWindow* render_window)
		{
			if (!IsInvisible())
			{
				current_sprite->SetPosition(position * block_size + Vector::Up * kicked_value);
				current_sprite->Draw(render_window);
			}
		}

		void Kick(Creatures::Mario* mario)
		{
			if (!kicked)
			{
				kicked_value = 1;
				kicked_dir = 0;
				kicked = true;

				Vector pos = (position + Vector::Up) * block_size;
				GameObject* obj = (GameObject*)GetObject(pos, mario);
				blocks->GetParent()->AddObject(obj);
				KillEnemiesAbove(mario);
			}

			sMarioGame->PlaySound("bump");
		}

		void Update(int delta_time)
		{
			if (kicked_value != 0)
			{
				if (kicked_dir == 0)
				{
					kicked_value += delta_time / 10;
					if (kicked_value > 15)
					{
						current_sprite = &kicked_sprite;
						kicked_dir = 1;
					}
				}
				if (kicked_dir == 1)
				{
					kicked_value -= delta_time / 10;
					if (kicked_value < 0)
						kicked_value = 0;
				}
			}
		}

	protected:
		float kicked_value = 0;
		bool kicked = false;
		int kicked_dir = 0;

		T* GetObject(const Vector& pos, Creatures::Mario* mario)
		{
			return new T(pos, mario);
		}

		friend class Blocks;
	};
}

#endif // !QUESTION_BLOCK_H