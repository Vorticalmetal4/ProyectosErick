#pragma once
#include <iostream>
#include <vector>

using namespace std;

struct VectorPosition 
{
	int x;
	int y;
};

class Player 
{
public:
	Player(class Renderer* Rend, class Ray* _PlayersRay);
	~Player();
	void Update(bool Pause);
	int getXPosition() { return Position.x; }
	int getYPosition() { return Position.y; }
	int getWidth() { return width; }
	int getHeight() { return height; }
	void ChangePower(char NPower);
	char getPower() { return Power; }
	void NoPower() { Power = 'N'; }
	bool CheckLasersCollition(class Brick* ActualBrick);

private:

	Renderer* Rend;
	int PlayerVelocity;
	VectorPosition Position;
	int width;
	int height;
	int Ammo;
	int Middle;
	char Power;
	class Ray* PlayersRay;

};