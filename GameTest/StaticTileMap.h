#ifndef _STATIC_TILE_MAP_H
#define _SIMPLE_TILE_MAP_H
#include "stdafx.h"
/*-----------------------------------------------
Default map for the game, uses alot of the same code as SimpleTileMap
-------------------------------------------------*/
enum EMapValue
{
	OUTOFBOUNDS = -1,   // If a GetTileMapValue() call is ourside the bounds of the map it will return this.
	BORDER,
	FLOOR,
	WALL,
	PIP,
};

class CStaticTileMap
{
public:
	CStaticTileMap(const int mapSize) : m_mapSize(mapSize)
	{
		Create();
	}
	~CStaticTileMap()
	{
	}

	void SetMap(const float targetFloorPercentage, const int maxTunnelLength);

	void Clear(EMapValue = EMapValue::WALL);

	EMapValue GetTileMapValue(const int x, const int y) const;
	EMapValue GetTileMapValue(const float fx, const float fy) const;

	bool SetTileMapValue(const int x, const int y, EMapValue v);
	void Render() const;
	int GetMapSize() const { return m_mapSize; }
private:
	void Create();
	int GetNewDirection(const int currentRow, const int currentColumn, int currentDir) const;
	int m_mapSize;
	float m_tileWidth;
	float m_tileHeight;
	std::vector<std::vector<EMapValue>>m_tileMap;
};

#endif

