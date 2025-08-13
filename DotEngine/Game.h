#pragma once

#include <vector>
static const int SCREEN_WIDTH = 1000;
static const int SCREEN_HEIGHT = 800;

class Dot;
class DotRenderer;

const int m_DotAmount = 1000;

class Game
{
public:
	Game(DotRenderer* aRenderer);
	void Update(float aDeltaTime);
	void CleanUp();

	std::vector<Dot*> m_Dots;

private:
	DotRenderer* m_Renderer;
};

