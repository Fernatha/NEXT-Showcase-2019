#pragma once
#ifndef _ENEMY_H
#define _ENEMY_H

#include "stdafx.h"
#include "Player.h"
#include "SimpleTileMap.h"

enum ELogic
{
	SIMPLE = -1,
	TELE,
	BOOST,
	HOUND,
	BASIC,
};
class CEnemy
{
public:
	CEnemy()
	{
		Create();
	}
	~CEnemy()
	{

	}

	void Create();
	void Render(const float tileW, const float tileH, int g) const;
	//setters
	void SetX(float x);
	void SetY(float y);
	void SetLogic(ELogic t);
	void SetDir(EDirection d);
	void SetAbility(bool a);
	//getters
	float GetX();
	float GetY();
	bool GetAbility();
	ELogic GetLogic();
	EDirection GetDir();
	//uses the player directions
	void GetPlayerDir(EDirection v);
	EDirection CheckTargetDir();
	//cheks the powerp that the enemy is using
	void PowerUp(CSimpleTileMap* map, CPlayer* p, float tileH, float tileW);
	//teleport logic
	void TeleLogic(CSimpleTileMap* map, CPlayer* p, float tileH, float tileW);
	bool CanTele(CSimpleTileMap* map, EDirection d, CPlayer* p, float tile);
	void Teleport(CPlayer* p, float tile);
	//hound logic
	void HoundLogic(CSimpleTileMap* map, CPlayer* p, float tileH, float tileW);
	//boost Logic
	void BoostLogic(CSimpleTileMap* map, float tileH, float tileW);
	//movement logic
	bool CanMove(CSimpleTileMap* map, float tileH, float tileW);	
	void Move(float m);



private:
	float e_enemyX;
	float e_enemyY;
	bool e_ability;

	ELogic e_logic;
	EDirection e_dir;
	EDirection targetDir;

};


#endif