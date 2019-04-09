#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H


#include <fstream>
#include <array>
#include <cstring>
#include <iostream>


struct HighscoreManager
{
  static constexpr unsigned NScores = 3;
  static constexpr char     Path[]  = "highscore";

  static inline
  std::array<u_int16_t, NScores> read (void)
  {
    auto* pf = std::fopen (Path, "r");

    std::array<u_int16_t, NScores> arr;
    if (std::fread(&arr, 2, NScores, pf) != NScores) memset (&arr, 0, NScores*2);
    std::fclose (pf);

    return arr;
  }

  static inline
  void commit (u_int16_t score)
  {
    auto scores  = read ();
    uint16_t tmp = UINT16_MAX;

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
};


#endif // ifndef HIGHSCOREMANAGER_H
