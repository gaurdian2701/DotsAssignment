#pragma once
#include <SDL3/SDL.h>
#include <string>
#include <vector>

class DotRenderer
{
public:
    DotRenderer(SDL_Window* window);

    ~DotRenderer();

    SDL_Renderer* GetSDLRenderer() const { return m_sdlRenderer; }
    std::vector<uint32_t> m_PixelBuffer;

    void SetDrawColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a);
    void Clear();
    void Present();

    void DrawCircle(int centerX, int centerY, int radius);
    void DrawFilledCircle(int centerX, int centerY, int radius, int redColor, int greenColor, int blueColor);
    void RenderTexture(SDL_Texture* texture, const SDL_FRect* srcRect, const SDL_FRect* dstRect);

private:
    SDL_Renderer* m_sdlRenderer = nullptr;
    const SDL_PixelFormatDetails* m_pixelFormatDetails = nullptr;

    void DrawPoint(int x, int y);

    DotRenderer(const DotRenderer&) = delete;
    DotRenderer& operator=(const DotRenderer&) = delete;
};

