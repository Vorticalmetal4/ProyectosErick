#include "Inih/cpp/INIReader.h"
#include "Renderer.h"
#include "Player.h"
#include "Asteroid.h"
#include "HUD.h"
#include "EnemyShip.h"

#include <stdlib.h>
#include <time.h>
using namespace std;

int main()
{ 

    INIReader ConFile("InitialData.ini");

	if (ConFile.ParseError() < 0)
		ConFile.PrintError("AsteroidsWindow");

    int i, j, k;
    int Collisions = 0;
    int NAsteroids = ConFile.GetInteger("Asteroid", "NAsteroids", 0);
    int AsteroidsMaxSize = ConFile.GetInteger("Asteroid", "MaxSize", 3);
    bool ResetAsteroids = false;
    bool ResetGame = false;

    Renderer Rend;
    HUDData GameData;
    bool Pause;

    int AsteroidWidth = ConFile.GetInteger("Asteroid", "Width", 1);
    int AsteroidHeight = ConFile.GetInteger("Asteroid", "Height", 1);
    int AsteroidVelocity = ConFile.GetInteger("Asteroid", "Velocity", 1);

    GameData.Lives = ConFile.GetInteger("HUD", "Lives", 0);
    GameData.Score = 0;
    int Points = ConFile.GetInteger("HUD", "PointsPerAsteroid", 0);
    int NEnemies = ConFile.GetInteger("EnemyShip", "NEnemies", 0);
    int EnemyProb = ConFile.GetInteger("EnemyShip", "EnemyProb", 0);
    int EnemyPoints = ConFile.GetInteger("EnemyShip", "Points", 0);

    bool success = Rend.Initialize(ConFile.GetString("Window", "Name", "Error"),
                                   ConFile.GetInteger("Window", "TopLeftXCoordinate", 100),
                                   ConFile.GetInteger("Window", "TopLeftYCoordinate", 20),
                                   ConFile.GetInteger("Window", "Width", 1080),
                                   ConFile.GetInteger("Window", "Height", 700),
                                   ConFile.GetInteger("Window", "Flags", 0),
                                   ConFile.GetString("Window", "Font", "Error"));
    
    HUD MainHUD = HUD(&Rend);
    Player MainPlayer = Player(&Rend);
    vector<Asteroid> Asteroids;
    vector<EnemyShip> Enemies;
    srand((unsigned int)time(NULL));


    for (i = 0; i < NAsteroids * 4; i++)
    {
        Asteroid NAsteroid = Asteroid(&Rend, (float)(AsteroidWidth  + rand() % Rend.getWindowWidth() - AsteroidWidth), (float)(AsteroidHeight + rand() % Rend.getWindowHeight() - AsteroidHeight), rand() % 360);
        Asteroids.push_back(NAsteroid);
        if (i < NAsteroids)
        {
            Asteroids[i].setActive(true);
            Asteroids[i].setSize(0);
        }
    }

    for(i = 0; i < NEnemies; i++)
    {
        EnemyShip NewEnemy = EnemyShip(&Rend);
        Enemies.push_back(NewEnemy);
    }

    if(success)
    {
        while(Rend.getIsRunning())
        {
            Rend.ProcessInput();
            Rend.UpdateGame();
            Rend.ClearRender();
            Pause = MainHUD.getPause();
            ResetGame = MainHUD.getReset();

            for(i = 0; i < Asteroids.size(); i++)
            {
                if(Asteroids[i].getActive())
                {
                    Asteroids[i].Update(Pause);

                    if (!Pause)
                    {
                        if (MainPlayer.CheckCollisions(Asteroids[i].getCenter(), Asteroids[i].getHypotenuse()))  //Collision between player and asteroids
                            GameData.Lives--;

                        if (MainPlayer.CheckLasersCollisions(Asteroids[i].getCenter(), Asteroids[i].getHypotenuse()))
                        {
                            Collisions = 1;
                            Asteroids[i].setLastObjectHitted(-1);
                            if(GameData.Lives > 0)
                                GameData.Score += Points * (Asteroids[i].getSize() + 1);
                            if (rand() % 101 <= EnemyProb)
                            {
                                for (k = 0; k < Enemies.size(); k++)
                                {
                                    if (!Enemies[k].getActive())
                                    {
                                        if (rand() % 2 == 1)
                                            Enemies[k].setNewData(true, true);
                                        else
                                            Enemies[k].setNewData(false, true);
                                        break;
                                    }

                                }
                            }
                        }


                        for (k = i + 1; k < Asteroids.size(); k++)
                            if (Asteroids[k].getActive())
                                if (Asteroids[i].CheckCollision(Asteroids[k].getCenter(), Asteroids[k].getHypotenuse()))
                                {
                                    Asteroids[i].ChangeDirection( k + 4);
                                    Asteroids[k].ChangeDirection( i + 4);
                                }

                        if (Collisions > 0)
                        {
                            if (Asteroids[i].getSize() < AsteroidsMaxSize)
                            {
                                for (j = 0; j < Asteroids.size(); j++)
                                    if (!Asteroids[j].getActive())
                                    {
                                        Asteroids[j].setActive(true);
                                        Asteroids[j].setNewData(Asteroids[i].getCenter(), Asteroids[i].getSize(), Asteroids[i].getWidth(), Asteroids[i].getHeight(), true, Asteroids[i].getVelocity());
                                        Asteroids[i].setNewData(Asteroids[i].getCenter(), Asteroids[i].getSize(), Asteroids[i].getWidth(), Asteroids[i].getHeight(), false, Asteroids[i].getVelocity());
                                        break;
                                    }

                            }
                            else
                                Asteroids[i].setActive(false);
                            Collisions--;
                        }
                    }
                }

                
            }

            for (j = 0; j < Enemies.size(); j++)
            {
                if (Enemies[j].getActive())
                {
                    Enemies[j].Update(MainPlayer.getCenter(), MainPlayer.getHypotenuse(), Pause);
                    if (!Pause)
                    {
                        if (MainPlayer.CheckCollisions(Enemies[j].getCenter(), Enemies[j].getHypotenuse()))
                        {
                            GameData.Lives--;
                            Enemies[j].setActive(false);
                        }
                        if (MainPlayer.CheckLasersCollisions(Enemies[j].getCenter(), Enemies[j].getHypotenuse()))
                        {
                            if(GameData.Lives > 0)
                                GameData.Score += EnemyPoints;
                            Enemies[j].setActive(false);
                        }
                    }
                }
            }

            GameData.Invincibility = MainPlayer.getInvincibiliy();
            MainHUD.Update(&GameData);
            if (GameData.Lives > 0)
                MainPlayer.Update(Pause);
            
            for (i = 0; i < Asteroids.size(); i++)
                if (Asteroids[i].getActive())
                    ResetAsteroids = false;

            if(ResetGame && GameData.Lives <= 0)
            {
                ResetAsteroids = true;
                GameData.Lives = ConFile.GetInteger("HUD", "Lives", 0);
                GameData.Score = 0;
                for (i = 0; i < Asteroids.size(); i++)
                    Asteroids[i].setActive(false);
                for (i = 0; i < NEnemies; i++)
                    Enemies[i].setActive(false);
                MainPlayer.ResetLasers();
                MainHUD.ResetHUD(false);
            }


            if(ResetAsteroids)
                for(i = 0; i < NAsteroids; i++)
                    Asteroids[i].setBigAsteroid(AsteroidWidth, AsteroidHeight, (float)(AsteroidWidth + rand() % Rend.getWindowWidth() - AsteroidWidth), (float)(AsteroidHeight + rand() % Rend.getWindowHeight() - AsteroidHeight), AsteroidVelocity);
            

            ResetAsteroids = true;

            Rend.GenerateOutput();

        }

        Rend.FreeMemory();
        MainHUD.FreeMemory();
    }
}

