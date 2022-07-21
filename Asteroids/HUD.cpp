#include "HUD.h"
#include "Renderer.h"
#include "Inih/cpp/INIReader.h"

#include <string>
#include <iostream>
using namespace std;

HUD::HUD(Renderer* _Rend)
	:Rend(_Rend),
	Pause(false),
	Reset(false)
{
	INIReader ConFile("InitialData.ini");

	if (ConFile.ParseError() < 0)
		ConFile.PrintError("HUD");

	LivesInitialX = ConFile.GetInteger("HUD", "LivesInitialX", 0);
	LivesY = ConFile.GetInteger("HUD", "LivesY", 0);
	LivesWidth = ConFile.GetInteger("HUD", "LivesWidth", 0);
	LivesHeight = ConFile.GetInteger("HUD", "LivesHeight", 0);
	LivesSeparation = ConFile.GetInteger("HUD", "LivesSeparation", 0);

	LivesHWidth = LivesWidth / 2;
	LivesP1.y = LivesP3.y = LivesY + LivesHeight;
	LivesP2.y = LivesY;

	string GameOverstr = "Game Over";
	string Restartstr = "Press R to restart";

	GameOver = (char*)malloc((GameOverstr.size() + 1) * sizeof(char));
	Restart = (char*)malloc((Restartstr.size() + 1) * sizeof(char));

	if (GameOver != NULL)
	{
		GameOverstr.copy(GameOver, GameOverstr.size());
		GameOver[GameOverstr.size()] = '\0';
	}

	if (Restart != NULL) {
		Restartstr.copy(Restart, Restartstr.size());
		Restart[Restartstr.size()] = '\0';
	}

	HWindowWidth = Rend->getWindowWidth() / 2.0;
	HWindowHeight = Rend->getWindowHeight() / 2.0;

	InitialPosition();

}

HUD::~HUD()
{
}

void HUD::Update(HUDData* Data)
{

	InitialPosition();

	for(i = 0; i < Data->Lives; i++)
	{
		LivesP1.x += LivesSeparation;
		LivesP2.x += LivesSeparation;
		LivesP3.x += LivesSeparation;
		Rend->DrawTriangle(&LivesP1, &LivesP2, &LivesP3, 255, 255, 255, 255);
	}

	ChangeScore(Data->Score, 50, 50, 0, 10);

	if(Rend->CheckPause())
	{
		if (Pause)
			Pause = false;
		else
			Pause = true;
	}


	if (Data->Lives <= 0)
	{
		Rend->Write(GameOver, 100, 100, HWindowWidth - 50, HWindowHeight - 50);
		
		Rend->Write(Restart, 100, 100, HWindowWidth - 80, HWindowHeight);

		if (Rend->CheckReset())
			Reset = true;
	}
		
}

void HUD::InitialPosition()
{
	LivesP1.x = LivesInitialX;
	LivesP2.x = LivesInitialX + LivesHWidth;
	LivesP3.x = LivesInitialX + LivesWidth;
}

void HUD::ChangeScore(int Score, int TextW, int TextH, int TextX, int TextY)
{
	ScoreText = to_string(Score);
	ScoreText.copy(NText, ScoreText.size() + 1);
	NText[ScoreText.size() + 1] = '\0';
	Rend->Write(NText, TextH, TextH, TextX, TextY);

}

void HUD::ResetHUD(bool _Reset)
{
	Reset = _Reset;
	for (i = 0; i < strlen(NText); i++)
		NText[i] = ' ';
}