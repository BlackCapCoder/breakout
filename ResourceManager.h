#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H


#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>


class ResourceManager
{
private:
  using FilePath = std::string;

  std::map<FilePath, SDL_Texture *> textureStore;
  std::map<std::pair<FilePath, uint16_t>, TTF_Font *> fontStore;

  SDL_Renderer * rend;


public:
  ResourceManager (SDL_Renderer * r)
    : rend{r}
  {
  }

  SDL_Texture * getImage (const FilePath & pth)
  {
    // Is it already loaded?
    auto search = textureStore.find (pth);
    if (search != textureStore.end())
      return search->second;

    SDL_Surface * s = IMG_Load (pth.c_str());
    if (s == nullptr) return nullptr;

    SDL_Texture * t = SDL_CreateTextureFromSurface (rend, s);
    SDL_FreeSurface (s);

    textureStore.insert(std::make_pair(pth, t));
    return t;
  }

  TTF_Font * getFont (const FilePath & pth, const uint16_t size)
  {
    // Is it already loaded?
    auto search = fontStore.find (std::make_pair(pth, size));
    if (search != fontStore.end())
      return search->second;

    TTF_Font * f = TTF_OpenFont (pth.c_str(), size);
    if (f == nullptr) return nullptr;

    fontStore.insert(std::make_pair(std::make_pair(pth, size), f));
    return f;
  }


  ~ResourceManager ()
  {
    for (auto p : textureStore)
      SDL_DestroyTexture(p.second);

    for (auto f : fontStore)
      TTF_CloseFont(f.second);

    textureStore.clear();
    fontStore.clear();
  }

};


#endif // ifndef RESOURCE_MANAGER_H
