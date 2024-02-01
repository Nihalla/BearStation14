
#ifndef ASGEGAME_ANIMAL_HPP
#define ASGEGAME_ANIMAL_HPP
#include "Character.hpp"

class Animal : public Character
{
 public:
  enum AnimalType
  {
    INVALID = -1,
    MONKEY  = 0,
    TIGER   = 1,
    DRAGON  = 2,
    BEAR    = 3,
    PUG     = 4
  };
  Animal()           = default;
  ~Animal() override = default;

  void init(ASGE::Point2D new_pos) override;
  void init(float x_pos, float y_pos) override;

  void setAnimalType(AnimalType new_type);

 protected:
  AnimalType animal_type = INVALID;
};

#endif // ASGEGAME_ANIMAL_HPP
