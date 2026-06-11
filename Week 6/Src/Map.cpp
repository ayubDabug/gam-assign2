#include "Map.h"

#include <cstdlib>
#include <ctime>

Map::Map()
{
    rows = 22;

    columns = 40;

    tileSize = 32;

    std::srand(
        static_cast<unsigned int>(
            std::time(nullptr)));
}

int Map::Index(
    int row,
    int column) const
{
    return
        row * columns +
        column;
}

//--------------------------------------------------
// Generation
//--------------------------------------------------

void Map::Clear()
{
    tiles.clear();
}

void Map::Generate()
{
    Clear();

    tiles.resize(
        rows *
        columns);

    for (int row = 0;
        row < rows;
        row++)
    {
        for (int column = 0;
            column < columns;
            column++)
        {
            Tile tile;

            tile.position =
            {
                static_cast<float>(
                    column * tileSize),

                static_cast<float>(
                    row * tileSize)
            };

            tile.type =
                TileType::Empty;

            tile.health = 0;

            int random =
                std::rand() % 100;

            if (random < 20)
            {
                tile.type =
                    TileType::Breakable;

                tile.health = 1;
            }
            else if (random < 30)
            {
                tile.type =
                    TileType::Steel;

                tile.health = -1;
            }

            tiles[
                Index(
                    row,
                    column)] =
                tile;
        }
    }

    //--------------------------------------------------
    // Player Spawn Zone
    //--------------------------------------------------

    for (int row = 0;
        row < 5;
        row++)
    {
        for (int column = 0;
            column < 5;
            column++)
        {
            Tile& tile =
                tiles[
                    Index(
                        row,
                        column)];

            tile.type =
                TileType::Empty;

            tile.health = 0;
        }
    }

    //--------------------------------------------------
    // Enemy Spawn Zone
    //--------------------------------------------------

    for (int row = rows - 5;
        row < rows;
        row++)
    {
        for (int column = columns - 5;
            column < columns;
            column++)
        {
            Tile& tile =
                tiles[
                    Index(
                        row,
                        column)];

            tile.type =
                TileType::Empty;

            tile.health = 0;
        }
    }
}

//--------------------------------------------------
// Grid
//--------------------------------------------------

int Map::GetRows() const
{
    return rows;
}

int Map::GetColumns() const
{
    return columns;
}

int Map::GetTileSize() const
{
    return tileSize;
}

//--------------------------------------------------
// Tile Access
//--------------------------------------------------

Tile& Map::GetTile(
    int row,
    int column)
{
    return
        tiles[
            Index(
                row,
                column)];
}

const Tile& Map::GetTile(
    int row,
    int column) const
{
    return
        tiles[
            Index(
                row,
                column)];
}

//--------------------------------------------------
// Queries
//--------------------------------------------------

bool Map::IsInsideMap(
    const glm::vec2& position) const
{
    return
        position.x >= 0.0f &&
        position.y >= 0.0f &&
        position.x <
        columns * tileSize &&
        position.y <
        rows * tileSize;
}

bool Map::IsBlocked(
    const glm::vec2& position) const
{
    if (!IsInsideMap(position))
    {
        return true;
    }

    int column =
        static_cast<int>(
            position.x /
            tileSize);

    int row =
        static_cast<int>(
            position.y /
            tileSize);

    const Tile& tile =
        GetTile(
            row,
            column);

    return
        tile.type !=
        TileType::Empty;
}

bool Map::DamageTile(
    const glm::vec2& position)
{
    if (!IsInsideMap(position))
    {
        return false;
    }

    int column =
        static_cast<int>(
            position.x /
            tileSize);

    int row =
        static_cast<int>(
            position.y /
            tileSize);

    Tile& tile =
        GetTile(
            row,
            column);

    if (tile.type ==
        TileType::Steel)
    {
        return false;
    }

    if (tile.type ==
        TileType::Breakable)
    {
        tile.health--;

        if (tile.health <= 0)
        {
            tile.type =
                TileType::Empty;

            tile.health = 0;
        }

        return true;
    }

    return false;
}

//--------------------------------------------------
// Spawn
//--------------------------------------------------

glm::vec2 Map::GetPlayerSpawn() const
{
    return
    {
        64.0f,
        64.0f
    };
}

glm::vec2 Map::GetEnemySpawn(
    int index) const
{
    switch (index)
    {
    case 0:
        return { 1180.0f,650.0f };

    case 1:
        return { 1100.0f,650.0f };

    case 2:
        return { 1180.0f,580.0f };

    default:
        return { 1120.0f,620.0f };
    }
}

//--------------------------------------------------
// Collision
//--------------------------------------------------

bool Map::CheckTankCollision(
    const glm::vec2& position,
    float width,
    float height) const
{
    float left =
        position.x -
        width * 0.5f;

    float right =
        position.x +
        width * 0.5f;

    float bottom =
        position.y -
        height * 0.5f;

    float top =
        position.y +
        height * 0.5f;

    glm::vec2 corners[4] =
    {
        {left,bottom},
        {right,bottom},
        {left,top},
        {right,top}
    };

    for (int i = 0;
        i < 4;
        i++)
    {
        if (
            IsBlocked(
                corners[i]))
        {
            return true;
        }
    }

    return false;
}

//--------------------------------------------------
// Tiles
//--------------------------------------------------

const std::vector<Tile>&
Map::GetTiles() const
{
    return tiles;
}

//--------------------------------------------------
// Tile Helpers
//--------------------------------------------------

bool Tile::IsSolid() const
{
    return
        type ==
        TileType::Breakable ||

        type ==
        TileType::Steel;
}

bool Tile::IsBreakable() const
{
    return
        type ==
        TileType::Breakable;
}

//--------------------------------------------------
// Destructor
//--------------------------------------------------

Map::~Map()
{
}