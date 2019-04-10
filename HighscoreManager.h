#ifndef HIGHSCOREMANAGER_H
#define HIGHSCOREMANAGER_H


#include <fstream>
#include <array>
#include <cstring>
#include <iostream>


struct HighscoreManager
{
public:
  static constexpr unsigned NScores = 5;
  static constexpr char     Path[]  = "highscore";

  static std::array<u_int16_t, NScores> read (void);
  static void commit (u_int16_t score);
  static uint16_t getLastScore ();
};


#endif // ifndef HIGHSCOREMANAGER_H
