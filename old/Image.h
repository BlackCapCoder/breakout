#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Image {
private:
  SDL_Surface* surf;
  SDL_Texture* image;
  SDL_Rect     rect;

public:
  explicit Image(SDL_Renderer* r, std::string const& file, int w, int h) noexcept;
  ~Image() noexcept;

public:
  void render(SDL_Renderer* r) noexcept;
  void setSize(int w, int h) noexcept;
};

#endif // IMAGE_H
