#ifndef RESOURCE_MANAGER_H
#define RESOURCE_MANAGER_H


#include <map>
#include <unordered_map>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_mixer.h>
#include <stdexcept>

struct Sound
{
private:
  Mix_Chunk * mc;

public:
  Sound (Mix_Chunk * mc) : mc{mc} {}

  void play ()
  {
    Mix_PlayChannel(-1, mc, 0);
  }
};

class ResourceManager
{
private:
  using FilePath = std::string;

  std::map<FilePath, SDL_Texture *> textureStore;
  std::map<std::pair<FilePath, uint16_t>, TTF_Font *> fontStore;
  std::unordered_map<std::string, Mix_Chunk*> audioStore;

  SDL_Renderer & rend;


public:
  ResourceManager (SDL_Renderer & r)
    : rend {r}
  {
  }

  SDL_Texture * getImage (const FilePath & pth)
  {
    // Is it already loaded?
    const auto search = textureStore.find (pth);
    if (search != textureStore.end())
      return search->second;

    SDL_Surface * s = IMG_Load (pth.c_str());
    if (s == nullptr) return nullptr;

    SDL_Texture * t = SDL_CreateTextureFromSurface (&rend, s);
    SDL_FreeSurface (s);

    textureStore.insert(std::make_pair(pth, t));
    return t;
  }

  TTF_Font * getFont (const FilePath & pth, const uint16_t size)
  {
    // Is it already loaded?
    const auto search = fontStore.find (std::make_pair(pth, size));
    if (search != fontStore.end())
      return search->second;

    TTF_Font * f = TTF_OpenFont (pth.c_str(), size);
    if (f == nullptr) return nullptr;

    fontStore.insert(std::make_pair(std::make_pair(pth, size), f));
    return f;
  }

  Sound getAudio (const FilePath & pth)
  {
    // Is it already loaded?
    const auto search = audioStore.find (pth);
    if (search != audioStore.end())
      return Sound {search->second};

    Mix_Chunk* sound = Mix_LoadWAV(pth.c_str());
    if (sound == nullptr)
      throw std::runtime_error ("Sound not found!");

    audioStore.insert(std::make_pair(pth, sound));
    return Sound{sound};
  }

  ~ResourceManager ()
  {
    for (auto p : textureStore)
      SDL_DestroyTexture(p.second);

    for (auto f : fontStore)
      TTF_CloseFont(f.second);

    for (auto f : audioStore)
      Mix_FreeChunk(f.second);

    textureStore.clear();
    fontStore.clear();
    audioStore.clear();
  }

};


#endif // ifndef RESOURCE_MANAGER_H
