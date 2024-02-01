
#include "Character.hpp"
#include <Engine/Logger.hpp>
void Character::init(float x_pos, float y_pos)
{
  health     = 10;
  damage     = 10;
  speed      = 10;
  position.x = x_pos;
  position.y = y_pos;
}

void Character::init(ASGE::Point2D new_pos)
{
  health   = 10;
  damage   = 10;
  speed    = 10;
  position = new_pos;
}

int Character::getHealth() const
{
  return health;
}

int Character::getDamage() const
{
  return damage;
}

int Character::getSpeed() const
{
  return speed;
}

ASGE::Point2D Character::getPosition()
{
  return position;
}

Boundary Character::getGlobalBounds() const
{
  Boundary bounds;
  bounds.build(sprite->xPos(), sprite->yPos(), sprite->width(), sprite->height());
  return bounds;
}

float Character::getRange() const
{
  return range;
}

Character::CharacterType Character::getCharacterType()
{
  return char_type;
}

double Character::getCD()
{
  return attack_cd;
}

std::vector<Character::GuessColours> Character::getGuesses()
{
  return std::vector<GuessColours>();
}

void Character::setHealth(int new_hp)
{
  health = new_hp;
}

void Character::setSpeed(int new_spd)
{
  speed = new_spd;
}

void Character::setPosition(float x_pos, float y_pos)
{
  position.x = x_pos;
  position.y = y_pos;
  sprite->xPos(x_pos);
  sprite->yPos(y_pos);
}

void Character::setPosition(ASGE::Point2D new_pos)
{
  position = new_pos;
  sprite->xPos(position.x);
  sprite->yPos(position.y);
}

void Character::setHasAttacked(bool state)
{
  has_attacked = state;
}

void Character::setInMinigame(bool minigame_status)
{
  in_minigame = minigame_status;
}

void Character::setCD(double new_cd)
{
  attack_cd = new_cd;
}

void Character::setSpriteSource()
{
  sprite->srcRect()[0] = animation_frames[current_frame][0];
  sprite->srcRect()[1] = animation_frames[current_frame][1];
}

void Character::setNearObjective(bool near)
{
  near_objective = near;
}

void Character::tickCD(double delta_in_sec)
{
  attack_cd -= delta_in_sec;
  if (attack_cd <= 0)
  {
    resetSpeed();
    Logging::DEBUG("Off Cooldown");
    has_attacked = false;
  }
}

bool Character::hasAttacked()
{
  return has_attacked;
}

void Character::takeDamage(int dmg)
{
  health -= dmg;
  if (health < 0)
  {
    health = 0;
  }
}

bool Character::isInMinigame()
{
  return in_minigame;
}

void Character::resetHP()
{
  health = init_health;
}

void Character::resetSpeed()
{
  speed = init_speed;
}

void Character::animationTick(const ASGE::GameTime& us)
{
  animation_timer += static_cast<float>(us.deltaInSecs());
  if (animation_timer > FRAME_RATE)
  {
    animation_timer = 0;

    if (current_frame < 3)
    {
      current_frame++;
    }
    else
    {
      current_frame = 1;
    }

    setSpriteSource();
  }
}

void Character::animationIdle()
{
  current_frame   = 0;
  animation_timer = 0;
  setSpriteSource();
}

void Character::addGuess(Character::GuessColours guess) {}

void Character::resetGuesses() {}

bool Character::isNearObjective()
{
  return near_objective;
}
