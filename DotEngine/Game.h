#pragma once

#include <vector>
struct SDL_Texture;
static const int SCREEN_WIDTH = 1000;
static const int SCREEN_HEIGHT = 800;

class Dot;
class DotRenderer;

const int m_DotAmount = 2000;

class Game
{
public:
	Game(DotRenderer* aRenderer);
	void Update(float aDeltaTime);
	void Render(float aDeltaTime);
	void CleanUp();

	std::vector<Dot*> m_Dots;

private:
	DotRenderer* m_Renderer;
	SDL_Texture* m_ScreenTexture;
};

