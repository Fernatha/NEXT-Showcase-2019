#include "stdafx.h"
#include "Enemy.h"
static float e_enemyDef [4][4]=
{
	{0.7f,0.0f,1.0f, 1.0f}, //Purple
	{0.0f,0.6f,0.0f, 1.0f}, //Green
	{1.0f,0.0f,0.0f, 1.0f}, //Red
	{0.0f,1.0f,1.0f, 1.0f} //cyan
};


void CEnemy::Create() 
{
	e_enemyX = (float)APP_VIRTUAL_WIDTH / 2;
	e_enemyY = (float)APP_VIRTUAL_HEIGHT / 2;
	e_dir = EDirection::STOPPED;
	e_logic = ELogic::SIMPLE;
	e_ability = false;
}
//Setters
void CEnemy::SetX(float x)
{
	e_enemyX = x;
}

void CEnemy::SetY(float y)
{
	e_enemyY = y;
}

void CEnemy::SetLogic(ELogic t)
{
	e_logic = t;
	if (e_logic == ELogic::HOUND)
	{
		e_ability = true;
	}
}

void CEnemy::GetPlayerDir(EDirection v)
{
	targetDir = v;
}

void CEnemy::SetDir(EDirection d)
{
	e_dir = d;
}

void CEnemy::SetAbility(bool a)
{
	e_ability = a;
}
//Getters
float CEnemy::GetX()
{
	return e_enemyX;
}

float CEnemy::GetY()
{
	return e_enemyY;
}

ELogic CEnemy::GetLogic()
{
	return e_logic;
}

EDirection CEnemy::GetDir()
{
	return e_dir;
}

bool CEnemy::GetAbility()
{
	return e_ability;
}


bool CEnemy::CanMove(CSimpleTileMap* map, float tileH, float tileW)
{
	//temp will be used to make sure that the sprite cant go back the way it came
	EDirection temp = EDirection::STOPPED;
	if (e_dir == EDirection::LEFT)
	{
		temp = EDirection::RIGHT;
	}
	else if(e_dir == EDirection::RIGHT)
	{
		temp = EDirection::LEFT;
	}
	else if (e_dir == EDirection::UP)
	{
		temp = EDirection::DOWN;
	}
	else if (e_dir == EDirection::DOWN)
	{
		temp = EDirection::UP;
	}
	//validation
	if (this->e_dir == LEFT)
	{
		if (map->GetTileMapValue(e_enemyX - tileW, e_enemyY) == EMapValue::PILL ||
			map->GetTileMapValue(e_enemyX - tileW, e_enemyY) == EMapValue::PIP ||
			map->GetTileMapValue(e_enemyX - tileW, e_enemyY) == EMapValue::FLOOR)
		{
			return true;
		}
	}
	else if (this->e_dir == RIGHT)
	{
		if (map->GetTileMapValue(e_enemyX + tileW, e_enemyY) == EMapValue::PILL ||
			map->GetTileMapValue(e_enemyX + tileW, e_enemyY) == EMapValue::PIP ||
			map->GetTileMapValue(e_enemyX + tileW, e_enemyY) == EMapValue::FLOOR)
		{
			return true;
		}
	}
	else if (this->e_dir == UP)
	{
		if (map->GetTileMapValue(e_enemyX, e_enemyY + tileH) == EMapValue::PILL ||
			map->GetTileMapValue(e_enemyX, e_enemyY + tileH) == EMapValue::PIP ||
			map->GetTileMapValue(e_enemyX, e_enemyY + tileH) == EMapValue::FLOOR)
		{
			return true;
		}
	}
	else if (this->e_dir == DOWN)
	{
		if (map->GetTileMapValue(e_enemyX, e_enemyY - tileH) == EMapValue::PILL ||
			map->GetTileMapValue(e_enemyX, e_enemyY - tileH) == EMapValue::PIP ||
			map->GetTileMapValue(e_enemyX, e_enemyY - tileH) == EMapValue::FLOOR)
		{
			return true;
		}
	}

	e_dir = EDirection(rand() % 4);
	while (EDirection(rand() % 4) == temp)
	{
		e_dir = EDirection(rand() % 4);
	}
	return false;
}

void CEnemy::Move(float m)
{
	if (this->e_dir == LEFT)
	{
		e_enemyX -= m;
	}
	else if (this->e_dir == RIGHT)
	{
		e_enemyX += m;
	}
	else if (this->e_dir == UP)
	{
		e_enemyY += m;
	}
	else if (this->e_dir == DOWN)
	{
		e_enemyY -= m;
	}
}

EDirection CEnemy::CheckTargetDir()
{
	return targetDir;
}

//checking which ability the sprites have and the calling their logic functions
void CEnemy::PowerUp(CSimpleTileMap* map, CPlayer* p, float tileH, float tileW) 
{
	if (e_logic == ELogic::TELE)
	{
		TeleLogic(map,p,tileH,tileW);
		e_ability = false;
	}
	else if (e_logic == ELogic::BOOST)
	{
		BoostLogic(map, tileH, tileW);
		e_ability = false;
	}
	else if (e_logic == ELogic::HOUND)
	{
		HoundLogic(map, p, tileH, tileW);
	}
	else if (e_logic == ELogic::BASIC)
	{
		e_ability = false;
	}


}

void CEnemy::BoostLogic(CSimpleTileMap* map, float tileH, float tileW) 
{
	for (int i = 0; i < 10; i++)
	{
		if (CanMove(map, tileH*2, tileW*2))
		{
			if (GetDir() == EDirection::LEFT || GetDir() == EDirection::RIGHT)
			{
				Move(tileW*2);
			}
			else if (GetDir() == EDirection::UP || GetDir() == EDirection::DOWN)
			{
				Move(tileH*2);
			}
		}
	}
}

void CEnemy::HoundLogic(CSimpleTileMap* map, CPlayer* p, float tileH, float tileW)
{
	if ((int)p->GetPlayerX() == (int)e_enemyX)
	{
		if (p->GetPlayerY() > e_enemyY)
		{
			e_dir = EDirection::UP;
		}
		else 
		{
			e_dir = EDirection::DOWN;
		}
		
	}
	else if ((int)p->GetPlayerY() == (int)e_enemyY)
	{
		if (p->GetPlayerX() > e_enemyX)
		{
			e_dir = EDirection::RIGHT;
		}
		else
		{
			e_dir = EDirection::LEFT;
		}
		
	}
	if (CanMove(map, tileH, tileW))
	{
		if (GetDir() == EDirection::LEFT || GetDir() == EDirection::RIGHT)
		{
			Move(tileW);
		}
		else if (GetDir() == EDirection::UP || GetDir() == EDirection::DOWN)
		{
			Move(tileH);
		}
	}


}

void CEnemy::TeleLogic(CSimpleTileMap* map, CPlayer* p, float tileH, float tileW)
{
	//get the players curent heading
	GetPlayerDir(p->GetDir());
	//check x tiles behind and see if the space is available
	if (targetDir == EDirection::LEFT || targetDir == EDirection::RIGHT)
	{
		if (CanTele(map, targetDir, p, tileW) && e_ability)
		{
			//teleport there and chase the player(on a timer)
			Teleport(p, tileW);
			
		}
	}
	else if (targetDir == EDirection::UP || targetDir == EDirection::DOWN)
	{
		if (CanTele(map, targetDir, p, tileH) && e_ability)
		{
			//teleport there and chase the player(on a timer)
			Teleport(p, tileH);
			
		}
	}
	else {
		//if the player has stopped, go back to simple logic style
		//but keep a true teleport value for next movement
	}

	
}

bool CEnemy::CanTele(CSimpleTileMap* map, EDirection d, CPlayer* p,float tile)
{
	if (d == EDirection::LEFT)
	{
		if (map->GetTileMapValue(p->GetPlayerX() + 3 * tile, p->GetPlayerY()) == EMapValue::OUTOFBOUNDS ||
			map->GetTileMapValue(p->GetPlayerX() + 3 * tile, p->GetPlayerY()) == EMapValue::WALL ||
			map->GetTileMapValue(p->GetPlayerX() + 3 * tile, p->GetPlayerY()) == EMapValue::BORDER)
		{
			return false;
		}
		return true;
	}
	else if (d == EDirection::RIGHT)
	{
		if (map->GetTileMapValue(p->GetPlayerX() - 3 * tile, p->GetPlayerY()) == EMapValue::OUTOFBOUNDS ||
			map->GetTileMapValue(p->GetPlayerX() - 3 * tile, p->GetPlayerY()) == EMapValue::WALL ||
			map->GetTileMapValue(p->GetPlayerX() - 3 * tile, p->GetPlayerY()) == EMapValue::BORDER)
		{
			return false;
		}
		return true;
	}
	else if (d == EDirection::UP)
	{
		if (map->GetTileMapValue(p->GetPlayerX(), p->GetPlayerY() + 3 * tile) == EMapValue::OUTOFBOUNDS ||
			map->GetTileMapValue(p->GetPlayerX(), p->GetPlayerY() + 3 * tile) == EMapValue::WALL ||
			map->GetTileMapValue(p->GetPlayerX(), p->GetPlayerY() + 3 * tile) == EMapValue::BORDER)
		{
			return false;
		}
		return true;
	}
	else if (d == EDirection::DOWN)
	{
		if (map->GetTileMapValue(p->GetPlayerX(), p->GetPlayerY() - 3 * tile) == EMapValue::OUTOFBOUNDS ||
			map->GetTileMapValue(p->GetPlayerX(), p->GetPlayerY() - 3 * tile) == EMapValue::WALL ||
			map->GetTileMapValue(p->GetPlayerX(), p->GetPlayerY() - 3 * tile) == EMapValue::BORDER)
		{
			return false;
		}
		return true;
	}
	return false;
}

void CEnemy::Teleport(CPlayer* p, float tile)
{
	if (targetDir == EDirection::LEFT)
	{
		SetX(p->GetPlayerX() + (3 * tile));
		SetY(p->GetPlayerY());
		e_dir = EDirection::LEFT;
	}
	else if (targetDir == EDirection::RIGHT)
	{
		SetX(p->GetPlayerX() - (3 * tile));
		SetY(p->GetPlayerY());
		e_dir = EDirection::RIGHT;
	}
	else if (targetDir == EDirection::UP)
	{
		SetY(p->GetPlayerY() - (3 * tile));
		SetX(p->GetPlayerX());
		e_dir = EDirection::UP;
	}
	else if (targetDir == EDirection::DOWN)
	{
		SetY(p->GetPlayerY() + (3 * tile));
		SetX(p->GetPlayerX());
		e_dir = EDirection::DOWN;
	}
}

void CEnemy::Render(const float tileW, const float tileH, int g) const
{
	App::DrawQuad(e_enemyX - tileW / 2, e_enemyY - tileH / 2,
		e_enemyX + tileW / 2, e_enemyY + tileH / 2,
		e_enemyDef[g][0], e_enemyDef[g][1], e_enemyDef[g][2]);
}