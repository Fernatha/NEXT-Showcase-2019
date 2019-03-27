#include "stdafx.h"
#include "Player.h"

static float p_playerDef[4] =
{
	1.0f,1.0f,0.0f, 1.0f
};

void CPlayer::Create()
{
	p_positionX = (float)APP_VIRTUAL_WIDTH / 2;
	p_positionY = (float)APP_VIRTUAL_HEIGHT / 2;
	p_score = 0;
	p_lives = 3;
	p_dir = EDirection::STOPPED;
}
//setters
void CPlayer::SetPlayerX(float x)
{
	p_positionX = x;
}

void CPlayer::SetPlayerY(float y)
{
	p_positionY = y;
}

void CPlayer::SetScore(int score)
{
	p_score = score;
}

void CPlayer::SetDirection(EDirection dir)
{
	p_dir = dir;
}

void CPlayer::SetLives(int l)
{
	p_lives = l;
}
//getters
float CPlayer::GetPlayerX()
{
	return p_positionX;
}

float CPlayer::GetPlayerY()
{
	return p_positionY;
}

int CPlayer::GetScore()
{
	return p_score;
}

int CPlayer::GetLives()
{
	return p_lives;
}

EDirection CPlayer::GetDir()
{
	return p_dir;
}

void CPlayer::Move(float m)
{
	if (this->p_dir == LEFT)
	{
		p_positionX -= m;
	}
	else if (this->p_dir == RIGHT)
	{
		p_positionX += m;
	}
	else if (this->p_dir == UP)
	{
		p_positionY += m;
	}
	else if (this->p_dir == DOWN)
	{
		p_positionY -= m;
	}
}

void CPlayer::Render(const float tileW, const float tileH) const
{
	App::DrawQuad(p_positionX - tileW / 2, p_positionY - tileH / 2,
		p_positionX + tileW / 2, p_positionY + tileH / 2,
		p_playerDef[0], p_playerDef[1], p_playerDef[2]);
}