
#ifndef ASGEGAME_HUMAN_HPP
#define ASGEGAME_HUMAN_HPP
#include "Character.hpp"

class Human : public Character
{
 public:
  Human()           = default;
  ~Human() override = default;

  void init(ASGE::Point2D new_pos) override;
  void init(float x_pos, float y_pos) override;

  void addGuess(GuessColours guess) override;
  std::vector<GuessColours> getGuesses() override;
  void resetGuesses() override;

 private:
  std::vector<GuessColours> minigame_guesses;
};

#endif // ASGEGAME_HUMAN_HPP
