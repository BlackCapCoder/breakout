#include "Image.h"

Image::Image(SDL_Renderer* r, std::string const& file, int w, int h)
  : surf{ IMG_Load(file.c_str()) }
  , image{ SDL_CreateTextureFromSurface(r, surf) }
  , rect{ 0, 0, w, h }
{
  SDL_FreeSurface(surf);
}

Image::~Image()
{
  SDL_DestroyTexture(image);
}

void Image::render(SDL_Renderer* r)
{
  SDL_RenderCopy(r, image, nullptr, &rect);
}

void Image::setSize(int w, int h)
{
  rect.w = w;
  rect.h = h;
}
