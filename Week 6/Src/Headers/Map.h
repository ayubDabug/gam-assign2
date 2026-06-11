#ifndef MAP_H
#define MAP_H

#include <vector>

#include <glm.hpp>

enum class TileType
{
    Empty,
    Breakable,
    Steel
};

struct Tile
{
    TileType type;

    int health;

    glm::vec2 position;

    bool IsSolid() const;

    bool IsBreakable() const;
};

class Map
{
private:

    //--------------------------------------------------
    // Grid
    //--------------------------------------------------

    int rows;

    int columns;

    int tileSize;

    //--------------------------------------------------
    // Tiles
    //--------------------------------------------------

    std::vector<Tile> tiles;

private:

    int Index(
        int row,
        int column) const;

public:

    //--------------------------------------------------
    // Constructor
    //--------------------------------------------------

    Map();

    //--------------------------------------------------
    // Generation
    //--------------------------------------------------

    void Generate();

    void Clear();

    //--------------------------------------------------
    // Grid Info
    //--------------------------------------------------

    int GetRows() const;

    int GetColumns() const;

    int GetTileSize() const;

    //--------------------------------------------------
    // Tile Access
    //--------------------------------------------------

    Tile& GetTile(
        int row,
        int column);

    const Tile& GetTile(
        int row,
        int column) const;

    //--------------------------------------------------
    // World Queries
    //--------------------------------------------------

    bool IsInsideMap(
        const glm::vec2& position) const;

    bool IsBlocked(
        const glm::vec2& position) const;

    bool DamageTile(
        const glm::vec2& position);

    //--------------------------------------------------
    // Spawn Points
    //--------------------------------------------------

    glm::vec2 GetPlayerSpawn() const;

    glm::vec2 GetEnemySpawn(
        int index) const;

    //--------------------------------------------------
    // Collision
    //--------------------------------------------------

    bool CheckTankCollision(
        const glm::vec2& position,
        float width,
        float height) const;

    //--------------------------------------------------
    // Tiles
    //--------------------------------------------------

    const std::vector<Tile>&
        GetTiles() const;

    //--------------------------------------------------
    // Destructor
    //--------------------------------------------------

    ~Map();
};

#endif