#ifndef BLOCK_H
#define BLOCK_H

#include "SpriteSheet.h"

namespace Creatures
{
	class Mario;
}

using namespace GameEngine;
using namespace GameEngine::Geometry;

namespace GameObjects
{
	enum class BlockType
	{
		Empty,
		Ground,
		Armor,
		Brick,
		Quiestion
	};

	enum BlockTitleCode
	{
		Brick = 2,
		Money = 3,
		CoinTitle = 11,
		MushroomTitle = 12,
		StartTile = 13,
		LadderTitle = 17,
		Bridge = 18,
		Chain = 81,
		InizUP = 20,
		InizCoin = 19,
		LiveUp = 21,
		BrickLadder = 26,
		InvizLadder = 27,
		BrickMushroom = 28
	};

	class Blocks;

	class Block
	{
	public:
		Block(BlockTitleCode id);
		virtual ~Block() { }

		virtual void Draw(sf::RenderWindow* renderWindow) { }
		virtual void Update(int delta_time) { }
		virtual void Kick(Creatures::Mario* mario) { }

		void SetPosition(const Vector& pos) { position = pos; }

		bool IsColliable() const { return colliable; }
		bool IsInvisible() const { return invisible; }

		void SetParent(Blocks* blocks) { this->blocks = blocks; }

		int GetCode() const { return id; }

	protected:
		bool colliable = true;
		bool invisible = false;
		friend class Blocks;
		static int block_size;
		static float timer;
		Blocks* blocks = nullptr;
		Vector position;
		static SpriteSheet question_block_sprite;
		static SpriteSheet water_sprite;
		static SpriteSheet lava_sprite;
		static SpriteSheet kicked_sprite;
		static SpriteSheet bricket_sprite;
		SpriteSheet* current_sprite = nullptr;
		int id;

		void SetInvisible(bool value);
		void KillEnemiesAbove(Creatures::Mario *mario);
	};
}
#endif // !BLOCK_H