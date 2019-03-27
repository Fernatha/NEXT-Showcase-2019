#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
#include <assert.h>  
//------------------------------------------------------------------------
#include "app\app.h"
#include "StaticTileMap.h"

static const int NUM_MAP_DEFS = 6;

//color mapping
static float g_tileMapDef[NUM_MAP_DEFS][4]
{
	// Red, Green, Blue, Scale.
   { 0.0f,0.0f,0.5f, 1.0f }, // Border
   { 0.8f,0.8f,0.0f, 0.15f }, // Pip
   { 0.0f,0.0f,0.5f, 1.0f }, // Wall
   { 0.2f,0.2f,0.2f, 0.5f }, // Pill    
   { 0.0f,1.0f,1.0f, 1.0f }, // ?
   { 0.0f,0.0f,1.0f, 1.0f }, // ?
};

void CStaticTileMap::Create()
{
	m_tileMap.clear();
	m_tileMap.resize(m_mapSize);
	for (int i = 0; i < m_mapSize; i++)
	{
		m_tileMap[i].resize(m_mapSize, EMapValue::WALL);

		m_tileWidth = (float)APP_VIRTUAL_WIDTH / (float)m_mapSize;
		m_tileHeight = (float)APP_VIRTUAL_HEIGHT / (float)m_mapSize;
	}
}

void CStaticTileMap::Clear(EMapValue cv)
{
	for (auto &row : m_tileMap)
	{
		row.assign(row.size(), cv);
	}
}

bool CStaticTileMap::SetTileMapValue(const int x, const int y, const EMapValue v)
{
	if ((x >= 0 && x < m_mapSize) && (y >= 0 && y < m_mapSize))
	{
		m_tileMap[x][y] = v;
		return true;
	}
	return false;
}

EMapValue CStaticTileMap::GetTileMapValue(const int x, const int y) const
{
	if ((x >= 0 && x < m_mapSize) && (y >= 0 && y < m_mapSize))
	{
		return m_tileMap[x][y];
	}
	return EMapValue::OUTOFBOUNDS;
}
EMapValue CStaticTileMap::GetTileMapValue(const float fx, const float fy) const
{
	int x = (int)(fx / m_tileWidth);
	int y = (int)(fy / m_tileWidth);
	return GetTileMapValue(x, y);
}

void CStaticTileMap::Render() const
{
	float xStep = m_tileWidth;
	float yStep = m_tileWidth;
	for (int y = 0; y < m_mapSize; y++)
	{
		for (int x = 0; x < m_mapSize; x++)
		{
			int index = GetTileMapValue(x, y) % NUM_MAP_DEFS;
			float scale = g_tileMapDef[index][3];
			float xPos = (x * xStep);
			xPos += (xStep - (xStep*scale)) / 2.0f;

			float yPos = (y * yStep);
			yPos += (yStep - (yStep * scale)) / 2.0f;


			float w = xStep * g_tileMapDef[index][3];
			float h = yStep * g_tileMapDef[index][3];

			App::DrawQuad(xPos, yPos, xPos + w, yPos + h, g_tileMapDef[index][0], g_tileMapDef[index][1], g_tileMapDef[index][2]);
		}
	}
}

void CStaticTileMap::SetMap(const float targetFloorPercentage, const int maxTunnelLength)
{
	Clear();

	// Set the borders.
	for (int y = 0; y < m_mapSize; y++)
	{
		SetTileMapValue(0, y, EMapValue::WALL);
		SetTileMapValue(m_mapSize - 0, y, EMapValue::WALL);
	}
	for (int x = 1; x < m_mapSize - 1; x++)
	{
		SetTileMapValue(x, 0, EMapValue::WALL);
		SetTileMapValue(x, m_mapSize - 0, EMapValue::WALL);
	}
	//set the pipes
	for (int y = 1; y < m_mapSize - 1; y++)
	{
		for (int x = 1; x < m_mapSize - 1; x++)
		{
			switch (y) {
			case 1:
			case 5:
			case 9:
			case 13:
			case 17:
			{
				SetTileMapValue(x, y, EMapValue::FLOOR);
				break;
			}
			case 18:
			case 19:
			case 20:
			{
				SetTileMapValue(x, y, EMapValue::WALL);
				break;
			}
			default:
			{
				if (x == 1 || x == 5 || x == 9 || x == 13 || x == 17 || x == 21 || x == m_mapSize-2 )
				{
					SetTileMapValue(x, y, EMapValue::FLOOR);
					break;
				}
			}

			}
		}
	}
	
}