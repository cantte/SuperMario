#include "Blocks.h"
#include "Block.h"
#include "Mario.h"
#include "MarioGame.h"
#include "GameScene.h"
#include "StaticBlock.h"
#include "TileMap.h"
#include "GameEngine.h"

#include <unordered_set>

using namespace GameEngine;

namespace GameObjects
{
	Blocks::Blocks(int cols, int rows, int block_size) : block_size(block_size)
	{
		SetName("Blocks");
		Block::block_size = block_size;
		shape.setSize(BlockSize());
		shape.setFillColor(sf::Color::Red);

		map = new TileMap<Block*>(cols, rows);
		map->Clear(nullptr);

		sf::Texture& texture = *sMarioGame->TextureManager().Get("AnimTiles");
		Block::question_block_sprite.Load(texture, { 0,0 }, { 32,32 }, 4, 1);
		Block::question_block_sprite.SetAnimType(AnimType::Manual);
		Block::water_sprite.Load(texture, { 0,32 }, { 32,32 }, 4, 1);
		Block::water_sprite.SetAnimType(AnimType::Manual);
		Block::lava_sprite.Load(texture, { 0,64 }, { 32,32 }, 4, 1);
		Block::lava_sprite.SetAnimType(AnimType::Manual);
		Block::kicked_sprite.Load(*sMarioGame->TextureManager().Get("Tiles"), { { 0,32,32,32 } });
		Block::bricket_sprite.Load(*sMarioGame->TextureManager().Get("Tiles"), { { 32,0,32,32 } });

		static const sf::String frag_shader =
			"#version 120\n"\
			"uniform sampler2D texture;\n"\
			"void main()"\
			"{"\
			"vec4 color = texture2D(texture, gl_TexCoord[0].xy);"\
			"float middle = (color.r + color.g)/2.f;"\
			"gl_FragColor = vec4(color.g,middle,middle,color.a);"\
			"}";
		night_view_filter_shader.loadFromMemory(frag_shader, sf::Shader::Fragment);
		night_view_filter_shader.setUniform("texture", sf::Shader::CurrentTexture);
	}

	Blocks::~Blocks()
	{
		for (int x = 0; x < map->GetWidth(); ++x)
			for (int y = 0; y < map->GetHeight(); ++y)
				delete GetBlock(x, y);
		delete map;
	}

	Vector Blocks::ToBlockCoordinates(const Vector& pixel, bool rounded) const
	{
		if (rounded)
			return Round(pixel / block_size);
		return pixel / block_size;
	}

	Vector Blocks::ToPixelCoordinates(const Vector& block) const
	{
		return block * block_size;
	}

	Rect Blocks::GetBlockBounds(int x, int y) const
	{
		return Rect(x * block_size, y * block_size, block_size, block_size);
	}

	Rect Blocks::GetBlockBounds(const Vector& pos) const
	{
		return GetBlockBounds(pos.X, pos.Y);
	}

	Vector Blocks::TraceLine(const Vector& start_cell, const Vector direction)
	{
		return map->TraceLine(start_cell, direction,
			[](Block* block) 
			{
				return !(block && block->IsColliable());
			});
	}

	bool Blocks::IsBlockInBounds(int x, int y) const
	{
		return (x >= 0 && y >= 0 && x < Cols() && y < Rows());
	}

	bool Blocks::IsBlockInBounds(const Vector& block) const
	{
		return IsBlockInBounds(block.X, block.Y);
	}

	std::vector<Vector> Blocks::GetBridgeBlocks()
	{
		std::vector<Vector> bridge_cells;

		for (int x = 0; x < Cols(); ++x)
			for (int y = 0; y < Rows(); ++y)
				if (GetBlock(x, y) &&
					(GetBlock(x, y)->GetCode() == BlockTitleCode::Bridge ||
						GetBlock(x, y)->GetCode() == BlockTitleCode::Chain))
					bridge_cells.emplace_back(x, y);

		return bridge_cells;
	}

	void Blocks::Draw(sf::RenderWindow* render_window)
	{
		static std::unordered_set<int> filter_except = { 82, 83, 50, 58, 62, 64 };

		if (night_view_filter)
			sf::Shader::bind(&night_view_filter_shader);

		Rect cameraRect = GetParent()->CastTo<GameScene>()->CameraRect();
		const float block_size = BlockSize().X;
		Vector center = render_window->getView().getCenter();
		Vector size = render_window->getView().getSize();
		view_rect = Rect(ToBlockCoordinates(center - size / 2), ToBlockCoordinates(size)).GetIntersection(Rect(0, 0, Cols(), Rows()));

		for (int x = view_rect.Left; x < view_rect.Right(); ++x)
		{
			for (int y = view_rect.Top; y < view_rect.Bottom(); ++y)
			{
				if (Block* block = GetBlock(x, y))
				{
					bool skip_filter = night_view_filter && filter_except.count(block->id);

					if (skip_filter)
						sf::Shader::bind(nullptr);

					block->Draw(render_window);

					if (skip_filter)
						sf::Shader::bind(&night_view_filter_shader);
				}
			}
		}

		GameObject::Draw(render_window);

		if (night_view_filter)
			sf::Shader::bind(nullptr);
	}

	void Blocks::Update(int delta_time)
	{
		if (!remove_later_list.empty())
		{
			for (auto object : remove_later_list)
				delete object;
			remove_later_list.clear();
		}

		GameObject::Update(delta_time);

		timer += delta_time / 200.f;
		Block::timer = timer;
		Block::question_block_sprite.SetSpriteIndex(int(timer) % 4);

		StaticBlock::sprite_sheet[49] = Block::water_sprite[int(timer) % 4];
		StaticBlock::sprite_sheet[81] = Block::lava_sprite[int(timer) % 4];

		for (int x = view_rect.Left; x < view_rect.Right(); ++x)
		{
			for (int y = view_rect.Top; y < view_rect.Bottom(); ++y)
			{
				if (Block* block = GetBlock(x, y))
					block->Update(delta_time);
			}
		}
	}

	void Blocks::ClearBlock(int x, int y)
	{
		remove_later_list.push_back(GetBlock(x, y));
		map->SetCell(x, y, nullptr);
	}

	void Blocks::KickBlock(int x, int y, Creatures::Mario* mario)
	{
		if (Block* block = GetBlock(x, y))
		{
			block->Kick(mario);

			if (block->IsInvisible())
				block->SetInvisible(false);
		}
	}

	void Blocks::EnableNightViewFilter()
	{
		night_view_filter = true;
	}

	void Blocks::LoadFromString(const std::string& data, std::function<Block * (char)> fabric)
	{
		map->LoadFromString(fabric, data);

		for (int x = 0; x < Cols(); ++x)
		{
			for (int y = 0; y < Rows(); ++y)
			{
				if (Block* block = GetBlock(x, y))
				{
					block->SetPosition({ x, y });
					block->SetParent(this);
				}
			}
		}
	}

	bool Blocks::IsCollidableBlock(int x, int y) const
	{
		if (Block* block = map->GetCell(x, y))
			return block->IsColliable();
		return false;
	}

	bool Blocks::IsCollidableBlock(const Vector& block) const
	{
		return IsCollidableBlock(block.X, block.Y);
	}

	bool Blocks::IsInvizibleBlock(const Vector& block) const
	{
		if (Block* _block = map->GetCell(block.X, block.Y))
			return _block->IsInvisible();
		return false;
	}

	Vector Blocks::CollisionResponse(const Rect& body_rect, const Vector& body_speed, float delta_time, CollisionTag& collision_tag)
	{
		Vector own_size = body_rect.Size();
		float tile_size = BlockSize().X;
		Vector new_pos = body_rect.LeftTop();

		new_pos.X -= body_speed.X * delta_time;

		for (int x = new_pos.X / tile_size; x < (new_pos.X + own_size.X) / tile_size; ++x)
		{
			for (int y = new_pos.Y / tile_size; y < (new_pos.Y + own_size.Y) / tile_size; ++y)
			{
				if (IsBlockInBounds(x, y) && (IsCollidableBlock(x, y) || (IsInvizibleBlock({ x, y }) && body_speed.Y < 0)))
				{
					if (body_speed.Y == 0 && body_rect.Bottom() > (y + 1)* tile_size)
					{
						collision_tag |= CollisionTag::Cell;
						collision_tag |= CollisionTag::Floor;

						return new_pos + Vector::Right * 2;
					}
					if (body_speed.Y >= 0)
					{
						new_pos.Y = y * tile_size - own_size.Y;
						collision_tag |= CollisionTag::Floor;
					}
					else if (body_speed.Y < 0)
					{
						new_pos.Y = y * tile_size + tile_size;
						collision_tag |= CollisionTag::Cell;
					}

					goto ExitYLoop;
				}
			}
		}

		ExitYLoop:;
		new_pos.X += body_speed.X * delta_time;

		for (int x = new_pos.X / tile_size; x < (new_pos.X + own_size.X) / tile_size; ++x)
		{
			for (int y = new_pos.Y / tile_size; y < (new_pos.Y + own_size.Y) / tile_size; ++y)
			{
				if (IsBlockInBounds(x, y) && IsCollidableBlock(x, y))
				{
					if (body_speed.X > 0)
					{
						new_pos.X = x * tile_size - own_size.X;
						collision_tag |= CollisionTag::Left;
					}
					else if (body_speed.X < 0)
					{
						new_pos.X = x * tile_size + tile_size;
						collision_tag |= CollisionTag::Right;
					}

					goto ExitXLoop;
				}
			}
		}

		ExitXLoop:;
		return new_pos;
	}
}