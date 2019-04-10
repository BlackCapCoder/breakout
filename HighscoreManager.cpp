#include "HighscoreManager.h"




std::array<u_int16_t, HighscoreManager::NScores> HighscoreManager::read (void)
{
  auto* pf = std::fopen (Path, "r");

  std::array<u_int16_t, NScores> arr{};
  int nread = std::fread(&arr, 2, NScores, pf);
  std::fclose (pf);

  return arr;
}

uint16_t LASTHIGHSCORE;

void HighscoreManager::commit (u_int16_t score)
{
  LASTHIGHSCORE = score;
  auto scores   = read ();
  uint16_t tmp  = UINT16_MAX;

  for (int i = 0; i < NScores; i++) {
    if (scores[i] > score) continue;
    tmp       = scores[i];
    scores[i] = score;
    score     = tmp;
  }

  if (tmp == UINT16_MAX) return;

  auto* pf = std::fopen (Path, "w");
  std::fwrite (&scores, 2, NScores, pf);
  std::fclose (pf);
}

uint16_t HighscoreManager::getLastScore ()
{
  return LASTHIGHSCORE;
}
