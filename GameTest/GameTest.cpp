//------------------------------------------------------------------------
// GameTest.cpp
//------------------------------------------------------------------------
#include "stdafx.h"
//------------------------------------------------------------------------
#include <windows.h> 
#include <math.h>  
#include "dos.h"
//------------------------------------------------------------------------
#include "app\app.h"
#include "SimpleTileMap.h"
#include "Player.h"
#include "Enemy.h"



void PlayerInput();
void MovePlayer();
bool CanMove();
void WhatTile();
void SetGhostSpawn(CEnemy* e);
void MoveEnemies();
void ResetPlayer();
bool DistPlayer(float fx, float fy);
bool CheckCol();
void ResetMap();
void EndGame(); //avengers plug

int timer = 0;
static const int MAP_SIZE = 21;
static const int TUNNEL_LEN = 19;
static const float TUNNEL_FILL_PERCENT = 80;


CSimpleTileMap g_map(MAP_SIZE);
CPlayer g_player;
CEnemy g_enemy[4];

static const float TILE_H = (float)APP_VIRTUAL_HEIGHT / (float)g_map.GetMapSize();
static const float TILE_W = (float)APP_VIRTUAL_WIDTH / (float)g_map.GetMapSize();


//------------------------------------------------------------------------
// Called before first update. Do any initial setup here.
//------------------------------------------------------------------------
void Init()
{
	srand(time(NULL));	
    g_map.RandomMap(TUNNEL_FILL_PERCENT, TUNNEL_LEN);
	for (int i = 0; i < 4; i++)
	{
		SetGhostSpawn(&g_enemy[i]);
		g_enemy[i].SetLogic(ELogic(i));
	}
	timer = 0;
}

//------------------------------------------------------------------------
// Update your simulation here. deltaTime is the elapsed time since the last update in ms.
// This will be called at no greater frequency than the value of APP_MAX_FRAME_RATE
//------------------------------------------------------------------------
void Update(float deltaTime)
{
	
	timer++;
    static const float MIN_FRAMERATE = (1000 / 20);
    if (deltaTime > MIN_FRAMERATE)
        deltaTime = MIN_FRAMERATE;

	PlayerInput();
	//wanted the enemies to and players
	if (timer % 25 == 0)
		MovePlayer();
	if(timer % 20 ==0)
		MoveEnemies();


	if (CheckCol())
	{
		g_player.SetLives(g_player.GetLives() - 1);
		ResetMap();
	}
	if (g_map.GetPillCount() == 0)
	{
		ResetMap();
	}
	
}

//------------------------------------------------------------------------
// Add your display calls here (DrawLine or Print) 
// See App.h 
//------------------------------------------------------------------------
void Render()
{	 
	WhatTile();
    g_map.Render();
	g_player.Render(TILE_W,	TILE_H);
	for (int i = 0; i < 4; i++)
	{
		g_enemy[i].Render(TILE_W, TILE_H, i);
	}
	
	App::Print((TILE_W * (MAP_SIZE-1)) - 110, TILE_H * MAP_SIZE - 25, "Score");
	App::Print((TILE_W * (MAP_SIZE-1)) - 50, TILE_H * MAP_SIZE - 25, std::to_string(g_player.GetScore()).c_str());

	App::Print((TILE_W * (MAP_SIZE - 1)) - 210, TILE_H * MAP_SIZE - 25, "Lives");
	App::Print((TILE_W * (MAP_SIZE - 1)) - 160, TILE_H * MAP_SIZE - 25, std::to_string(g_player.GetLives()).c_str());

	if (g_player.GetLives() < 0)
	{
		App::DrawQuad(0.0f, 0.0f, (float)APP_VIRTUAL_WIDTH, (float)APP_VIRTUAL_HEIGHT, 1.0f, 1.0f, 1.0f);
		App::Print((float)APP_VIRTUAL_WIDTH / 2, (float)APP_VIRTUAL_HEIGHT / 2, "Game Over", 0.0f, 0.0f, 0.0f );
		App::Print((float)APP_VIRTUAL_WIDTH / 2 - 100, (float)APP_VIRTUAL_HEIGHT / 2 - 100, "Final Score: ", 0.0f, 0.0f, 0.0f);
		App::Print((float)APP_VIRTUAL_WIDTH / 2, (float)APP_VIRTUAL_HEIGHT / 2 - 100, std::to_string(g_player.GetScore()).c_str(), 0.0f, 0.0f, 0.0f);
	}

}

/*
---Functions here for handling gametime evets
*/

void SetGhostSpawn(CEnemy* g)
{
	bool valid = false;
	float x = 0;
	float y = 0;
	
	while (!valid)
	{
		x = (float)(rand() % g_map.GetMapSize() + 1) * TILE_W;
		y = (float)(rand() % g_map.GetMapSize() + 1) * TILE_H;
		for (int i = 0; i < 4; i++)
		{
			if (g_enemy[i].GetX() == x && g_enemy[i].GetY() == x ||
				g_map.GetTileMapValue(x, y) == EMapValue::WALL ||
				g_map.GetTileMapValue(x, y) == EMapValue::BORDER ||
				g_map.GetTileMapValue(x, y) == EMapValue::OUTOFBOUNDS ||
				DistPlayer(x,y))
			{
				valid = false;
				break;
			}
			valid = true;
		}	
	}
	g->SetX(x  + TILE_W / 2);	
	g->SetY(y  + TILE_H / 2);
}

bool DistPlayer(float fx, float fy)
{
	bool tooClose = false;
	for (float i = fx; i <= fx + 2 * TILE_W; i+= TILE_W)
	{
		for (float v = fy; v <= fy + 2 * TILE_H; v += TILE_H)
		{
			if (g_player.GetPlayerX() == i && g_player.GetPlayerY() == v)
			{
				tooClose = true;
				break;
			}
		}
	}
	return tooClose;
}

void PlayerInput() {
	if (App::GetController().CheckButton(VK_PAD_START, true))
	{
		ResetMap();
	}

	if (App::IsKeyPressed('A'))
	{
		g_player.SetDirection(LEFT);
	}
	if (App::IsKeyPressed('D'))
	{
		g_player.SetDirection(RIGHT);
	}
	if (App::IsKeyPressed('W'))
	{
		g_player.SetDirection(UP);
	}
	if (App::IsKeyPressed('S'))
	{
		g_player.SetDirection(DOWN);
	}
}

void ResetPlayer()
{
	float cx = (float)APP_VIRTUAL_WIDTH / 2;
	float cy = (float)APP_VIRTUAL_HEIGHT / 2;

	g_player.SetPlayerX(cx);
	g_player.SetPlayerY(cy);

	for (float i = cx - 1 * TILE_W; i < cx + 1 * TILE_W; i+= TILE_W)
	{
		for (float n = cy - 1 * TILE_H; n < cy + 1 * TILE_H; n+= TILE_H)
		{
			g_map.SetTileMapValue((int)i, (int)n, EMapValue::FLOOR);
		}
	}
	g_player.SetDirection(EDirection::STOPPED);
}

void MovePlayer()
{
	if (CanMove())
	{
		if (g_player.GetDir() == EDirection::LEFT || g_player.GetDir() == EDirection::RIGHT)
		{
			g_player.Move(TILE_W);
		}
		else if (g_player.GetDir() == EDirection::UP || g_player.GetDir() == EDirection::DOWN)
		{
			g_player.Move(TILE_H);
		}
	}
}

bool CanMove()
{
	if (g_player.GetDir() == LEFT)
	{
		if (g_map.GetTileMapValue(g_player.GetPlayerX() - TILE_W, g_player.GetPlayerY()) == EMapValue::PILL ||
			g_map.GetTileMapValue(g_player.GetPlayerX() - TILE_W, g_player.GetPlayerY()) == EMapValue::PIP ||
			g_map.GetTileMapValue(g_player.GetPlayerX() - TILE_W, g_player.GetPlayerY()) == EMapValue::FLOOR)
		{
			return true;
		}
	}
	else if (g_player.GetDir() == RIGHT)
	{
		if (g_map.GetTileMapValue(g_player.GetPlayerX() + TILE_W, g_player.GetPlayerY()) == EMapValue::PILL ||
			g_map.GetTileMapValue(g_player.GetPlayerX() + TILE_W, g_player.GetPlayerY()) == EMapValue::PIP ||
			g_map.GetTileMapValue(g_player.GetPlayerX() + TILE_W, g_player.GetPlayerY()) == EMapValue::FLOOR)
		{
			return true;
		}
	}
	else if (g_player.GetDir() == UP)
	{
		if (g_map.GetTileMapValue(g_player.GetPlayerX(), g_player.GetPlayerY() + TILE_H) == EMapValue::PILL ||
			g_map.GetTileMapValue(g_player.GetPlayerX(), g_player.GetPlayerY() + TILE_H) == EMapValue::PIP ||
			g_map.GetTileMapValue(g_player.GetPlayerX(), g_player.GetPlayerY() + TILE_H) == EMapValue::FLOOR)
		{
			return true;
		}
	}
	else if (g_player.GetDir() == DOWN)
	{
		if (g_map.GetTileMapValue(g_player.GetPlayerX(), g_player.GetPlayerY() - TILE_H) == EMapValue::PILL ||
			g_map.GetTileMapValue(g_player.GetPlayerX(), g_player.GetPlayerY() - TILE_H) == EMapValue::PIP ||
			g_map.GetTileMapValue(g_player.GetPlayerX(), g_player.GetPlayerY() - TILE_H) == EMapValue::FLOOR)
		{
			return true;
		}
	}
		g_player.SetDirection(STOPPED);
		return false;


}

void MoveEnemies()
{
	
	for (int i = 0; i < 4; i++)
	{
		if (timer % 400 == 0)
		{
			g_enemy[i].SetAbility(true);
		}

		if (g_enemy[i].GetAbility())
		{
			g_enemy[i].PowerUp(&g_map, &g_player, TILE_H, TILE_W);
		}
		else if (g_enemy[i].CanMove(&g_map, TILE_H, TILE_W))
		{
			if (g_enemy[i].GetDir() == EDirection::LEFT || g_enemy[i].GetDir() == EDirection::RIGHT)
			{
				g_enemy[i].Move(TILE_W);
			}
			else if (g_enemy[i].GetDir() == EDirection::UP || g_enemy[i].GetDir() == EDirection::DOWN)
			{
				g_enemy[i].Move(TILE_H);
			}
		}
	}
}

void WhatTile()
{
	if (g_map.GetTileMapValue(g_player.GetPlayerX(), g_player.GetPlayerY()) == EMapValue::PILL)
	{
		g_player.SetScore(g_player.GetScore() + 10);
		g_map.SetTileMapValue((int)g_player.GetPlayerX() / (int)TILE_W, (int)g_player.GetPlayerY() / (int)TILE_H, EMapValue::FLOOR);
		g_map.SetPillCount(g_map.GetPillCount() - 1);
		
	}
	else if (g_map.GetTileMapValue(g_player.GetPlayerX(), g_player.GetPlayerY()) == EMapValue::PIP)
	{
		g_player.SetScore(g_player.GetScore() + 100);
		g_map.SetTileMapValue((int)g_player.GetPlayerX() / (int)TILE_W, (int)g_player.GetPlayerY() / (int)TILE_H, EMapValue::FLOOR);
		g_map.SetPillCount(g_map.GetPillCount() - 1);
	}
}

bool CheckCol()
{
	for (int i = 0; i < 4; i++)
	{
		if ((int)g_player.GetPlayerX() == (int)g_enemy[i].GetX() && (int)g_player.GetPlayerY() == (int)g_enemy[i].GetY())
		{
			return true;
		}
	}
	return false;
}

void ResetMap()
{
	
		g_map.RandomMap(TUNNEL_FILL_PERCENT, TUNNEL_LEN);
		g_map.SetTileMapValue((float)APP_VIRTUAL_HEIGHT / 2, (float)APP_VIRTUAL_WIDTH / 2, EMapValue::FLOOR);
		for (int i = 0; i < 4; i++)
		{
			ResetPlayer();
			SetGhostSpawn(&g_enemy[i]);
			timer = 0;
		}	
}

//------------------------------------------------------------------------
// Add your shutdown code here. Called when the APP_QUIT_KEY is pressed.
// Just before the app exits.
//------------------------------------------------------------------------
void Shutdown()
{
	
}