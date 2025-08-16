#include "DotRenderer.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL.h>
#include <cmath> 

#include "Game.h"

DotRenderer::DotRenderer(SDL_Window* window) : m_sdlRenderer(nullptr)
{
	m_sdlRenderer = SDL_CreateRenderer(window, nullptr);
	m_pixelFormatDetails = SDL_GetPixelFormatDetails(SDL_PIXELFORMAT_ARGB8888);
	m_PixelBuffer.resize(SCREEN_WIDTH * SCREEN_HEIGHT, 0x00000000);
	if (!m_sdlRenderer) return;
}

DotRenderer::~DotRenderer()
{
	if (m_sdlRenderer)
	{
		SDL_DestroyRenderer(m_sdlRenderer);
		m_sdlRenderer = nullptr;
	}
}

void DotRenderer::SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a)
{
	if (m_sdlRenderer)
	{
		SDL_SetRenderDrawColor(m_sdlRenderer, r, g, b, a);
	}
}

void DotRenderer::Clear()
{
	if (m_sdlRenderer)
	{
		SDL_RenderClear(m_sdlRenderer);
	}
}

void DotRenderer::Present()
{
	if (m_sdlRenderer)
	{
		SDL_RenderPresent(m_sdlRenderer);
	}
}

void DotRenderer::DrawPoint(int x, int y)
{
	if (m_sdlRenderer)
	{
		SDL_RenderPoint(m_sdlRenderer, x, y);
	}
}

void DotRenderer::DrawCircle(int centerX, int centerY, int radius)
{
	if (!m_sdlRenderer) return;

	int x = radius;
	int y = 0;
	int err = 0;

	while (x >= y) 
	{
		DrawPoint(centerX + x, centerY + y);
		DrawPoint(centerX + y, centerY + x);
		DrawPoint(centerX - y, centerY + x);
		DrawPoint(centerX - x, centerY + y);
		DrawPoint(centerX - x, centerY - y);
		DrawPoint(centerX - y, centerY - x);
		DrawPoint(centerX + y, centerY - x);
		DrawPoint(centerX + x, centerY - y);

		if (err <= 0) 
		{
			y++;
			err += 2 * y + 1;
		}
		if (err > 0) 
		{
			x--;
			err -= 2 * x + 1;
		}
	}
}

void DotRenderer::DrawFilledCircle(int centerX, int centerY, int radius, int redColor, int greenColor, int blueColor)
{
	if (!m_sdlRenderer) return;

	int minX = std::max(0, centerX - radius);
	int maxX = std::min(SCREEN_WIDTH - 1, centerX + radius);
	int minY = std::max(0, centerY - radius);
	int maxY = std::min(SCREEN_HEIGHT - 1, centerY + radius);

	for (int y = minY; y <= maxY; y++)
	{
		for (int x = minX; x <= maxX; x++)
		{
			int dx = x - centerX;
			int dy = y - centerY;

			if (dx * dx + dy * dy <= radius * radius)
			{
				int index = y * SCREEN_WIDTH + x;
				m_PixelBuffer[index] = SDL_MapRGBA(
					m_pixelFormatDetails,
					nullptr,
					redColor,
					blueColor,
					greenColor,
					255);
			}
		}
	}
}

void DotRenderer::RenderTexture(SDL_Texture* texture, const SDL_FRect* srcRect, const SDL_FRect* dstRect)
{
	if (m_sdlRenderer && texture)
	{
		SDL_RenderTexture(m_sdlRenderer, texture, srcRect, dstRect);
	}
}




