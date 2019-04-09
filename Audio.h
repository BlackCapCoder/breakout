#ifndef AUDIO_H
#define AUDIO_H

#include <string>
#include <SDL2/SDL_mixer.h>

class Audio {
  Mix_Chunk* sound;

public:
  explicit Audio(std::string const& file);
  ~Audio();
  void play();
};

#endif // AUDIO_H
