#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H


#include <fstream>
#include <array>
#include <cstring>


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
};


#endif // ifndef HIGHSCOREMANAGER_H
