#include "Power.h"
#include "Player.h"
#include "Renderer.h"
#include "Inih/cpp/INIReader.h"

#include <string>
#include <ctime>

Power::Power(Player* _MainPlayer, Renderer* _Rend)
	:MainPlayer(_MainPlayer),
	Rend(_Rend),
	DeltaTime(0),
	NPositionY(0),
	Active(false)
{

	INIReader ConFile("InitialData.ini");

	if (ConFile.ParseError() < 0)
		ConFile.PrintError("Power");

	Velocity = ConFile.GetInteger("Power", "Velocity", 0);
	Height = ConFile.GetInteger("Power", "Height", 0);
	LaserProbability = ConFile.GetInteger("Power", "LaserProbability", 0);

	Position.x = -50;
	Position.y = -50;

}

Power::~Power() 
{

}

void Power::SetData(int _X, int _Y, bool _Active)
{
	srand(time(NULL));
	if ((rand() % (100) + 1) <= LaserProbability) 
		PowerType = 'L';
	else
		PowerType = 'T';
		

	Active = _Active;
	Position.x = _X;
	Position.y = _Y;
}

void Power::Update() 
{
	NText[0] = PowerType;
	NText[1] = '\0';
	Rend->Write(NText, 0, Height, Position.x, Position.y);

}

bool Power::CheckCollision(bool Pause)
{
	if (!Pause)
	{
		DeltaTime = Rend->getDeltaTime();
		NPositionY = Position.y + DeltaTime * Velocity;

		if (NPositionY + Height <= Rend->getWindowHeight())
		{
			if (Position.x >= MainPlayer->getXPosition() && Position.x <= MainPlayer->getXPosition() + MainPlayer->getWidth())
			{
				if (NPositionY + Height >= MainPlayer->getYPosition() && NPositionY + Height <= MainPlayer->getYPosition() + MainPlayer->getHeight())
				{
					MainPlayer->ChangePower(PowerType);
					return true;
				}
				else
					Position.y = NPositionY;
			}
			else
				Position.y = NPositionY;
		}
		else
			return true;
	}

	return false;
}