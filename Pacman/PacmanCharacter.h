#pragma once
#include <iostream>
class PacmanCharacter
{
public:
	PacmanCharacter(float movement_speed);
	~PacmanCharacter();
	void Render();
	void update(float deltaTime);
private:

};

