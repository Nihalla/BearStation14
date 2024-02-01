#include "Human.hpp"

void Human::init(ASGE::Point2D new_pos)
{
  char_type   = HUMAN;
  health      = 20;
  init_health = 20;
  damage      = 10;
  speed       = 128;
  init_speed  = 128;
  position    = new_pos;
  minigame_guesses.reserve(6);
}
void Human::init(float x_pos, float y_pos)
{
  char_type   = HUMAN;
  health      = 20;
  init_health = 20;
  damage      = 10;
  speed       = 128;
  init_speed  = 128;
  position.x  = x_pos;
  position.y  = y_pos;
  minigame_guesses.reserve(6);
}

void Human::addGuess(Character::GuessColours guess)
{
  minigame_guesses.emplace_back(guess);
}

std::vector<Character::GuessColours> Human::getGuesses()
{
  return minigame_guesses;
}

void Human::resetGuesses()
{
  minigame_guesses.clear();
}
