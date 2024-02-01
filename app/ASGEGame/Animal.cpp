
#include "Animal.hpp"

// ADD SWITCH TO DIFFERENTIATE BETWEEN DIFFERENT TYPES OF ANIMALS
void Animal::init(ASGE::Point2D new_pos)
{
  char_type   = ANIMAL;
  animal_type = INVALID;
  health      = 10;
  damage      = 10;
  speed       = 150;
  position    = new_pos;
}
void Animal::init(float x_pos, float y_pos)
{
  char_type   = ANIMAL;
  animal_type = INVALID;
  health      = 20;
  init_health = 20;
  damage      = 10;
  speed       = 150;
  init_speed  = 150;
  position.x  = x_pos;
  position.y  = y_pos;
}
void Animal::setAnimalType(Animal::AnimalType new_type)
{
  animal_type = new_type;

  // Add switch for stats here
}
