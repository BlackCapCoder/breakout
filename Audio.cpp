#include "Audio.h"

Audio::Audio(std::string const& file)
  : sound{ Mix_LoadWAV(file.c_str()) }
{ }

Audio::~Audio()
{
  Mix_FreeChunk(sound);
}

void Audio::play()
{
  Mix_PlayChannel(-1, sound, 0);
}
