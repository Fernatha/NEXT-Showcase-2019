#pragma once
#ifndef _PLAYER_H
#define _PLAYER_H
#include "stdafx.h"
#include "App/app.h"
#include "SimpleTileMap.h"

enum EDirection
{
	STOPPED = -1,
	UP,
	DOWN,
	LEFT,
	RIGHT,
};

class CPlayer
{
public:
	CPlayer()
	{
		Create();
	};
	~CPlayer()
	{
		
	};
	void Create();
	void Render(const float tileW, const float tileH)const;
	//setters
	void SetPlayerX(float x);
	void SetPlayerY(float y);
	void SetScore(int score);
	void SetDirection(EDirection dir);
	void SetLives(int l);
	//getters
	float GetPlayerX();
	float GetPlayerY();
	int GetScore();
	EDirection GetDir();
	int GetLives();

	void Move(float m);

private:

	float p_positionX;
	float p_positionY;
	int p_score;
	int p_lives;
	EDirection p_dir;
};
#endif