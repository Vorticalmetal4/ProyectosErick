#pragma once

struct VectorPositionBrick 
{
	int x;
	int y;
};

class Brick 
{
public:
	Brick(class Renderer* _Rend, int XPosition, int YPosition, float Separation);
	~Brick();
	void Draw(int raw);

	int getXPosition() { return Position.x; }
	int getYPosition() { return Position.y; }
	int getWidth() { return width; }
	int getHeight() { return height; }
	bool getActive() { return Active; }
	bool getTraitor() { return Traitor; }
	void setActive(bool Change) { Active = Change; }
	void setTraitor(bool Change) { Traitor = Change; }
	bool CheckCollition(class Ball* Ball);
	void setYPosition(int Y) {Position.y = Y; }

private:
	VectorPositionBrick Position;
	int width;
	int height;
	Renderer* Rend;
	bool Active;
	bool Traitor;
	float VerticalSeparation;
	
};