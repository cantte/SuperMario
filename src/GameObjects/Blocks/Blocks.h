#ifndef BLOCKS_H
#define BLOCKS_H

#include "GameObject.h"
#include "TileMap.h"

namespace Creatures
{
	class Mario;
}

namespace GameEngine
{
	enum class CollisionTag : int;
}

namespace GameObjects
{
	class Block;
	
	class Blocks : public GameObject
	{
	public:
		Blocks(int cols, int rows, int block_size);
		~Blocks();

		Vector BlockSize() const { return { block_size, block_size }; }
		Block* GetBlock(int x, int y) { return map->GetCell(x, y); }

		int Rows() const { return map->GetHeight(); }
		int Cols() const { return map->GetWidth(); }
		int Width() const { return Cols() * block_size; }
		int Height() const { return Rows() * block_size; }

		Vector ToBlockCoordinates(const Vector& pixel, bool rounded = false) const;
		Vector ToPixelCoordinates(const Vector& block) const;
		Rect GetBlockBounds(int x, int y) const;
		Rect GetBlockBounds(const Vector& pos) const;

		Vector TraceLine(const Vector& start_cell, const Vector direction);

		bool IsBlockInBounds(int x, int y) const;
		bool IsBlockInBounds(const Vector& block) const;

		std::vector<Vector> GetBridgeBlocks();

		void Draw(sf::RenderWindow* render_window) override;
		void Update(int delta_time) override;

		void ClearBlock(int x, int y);
		void KickBlock(int x, int y, Creatures::Mario* mario);

		void EnableNightViewFilter();

		void LoadFromString(const std::string& data, std::function<Block * (char)> fabric);

		bool IsCollidableBlock(int x, int y) const;
		bool IsCollidableBlock(const Vector& block) const;
		bool IsInvizibleBlock(const Vector& block) const;

		Vector CollisionResponse(const Rect& body_rect, const Vector& body_speed, float delta_time, GameEngine::CollisionTag& collision_tag);

	private:
		Vector kicked_block;
		Rect view_rect;
		bool night_view_filter = false;
		GameEngine::TileMap<Block*>* map;
		sf::RectangleShape shape;
		float timer;
		float block_size = 32.0f;
		sf::Shader night_view_filter_shader;
		std::vector<Block*> remove_later_list;
	};
}

#endif // !BLOCKS_H