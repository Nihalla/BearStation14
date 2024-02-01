
#include "Player.hpp"
#include <math.h>

Player::~Player()
{
  delete controlled_character;
}

int Player::getPlayerID() const
{
  return player_id;
}

int Player::getController() const
{
  return controller_id;
}

Character* Player::getCharacter()
{
  return controlled_character;
}

ASGE::Point2D Player::getVelocity()
{
  return velocity;
}

ASGE::Point2D Player::getPosition()
{
  return controlled_character->getPosition();
}

Boundary Player::getGlobalBounds()
{
  return controlled_character->getGlobalBounds();
}

float Player::getScale()
{
  return SCALE;
}

int Player::getCamera() const
{
  return camera_id;
}

ASGE::Point2D Player::getCenter()
{
  return ASGE::Point2D(
    getPosition().x + getGlobalBounds().width / 2, getPosition().y + getGlobalBounds().height / 2);
}

float Player::getRange()
{
  return controlled_character->getRange();
}

int Player::getDamage()
{
  return controlled_character->getDamage();
}

int Player::getHP()
{
  return controlled_character->getHealth();
}

int Player::getScore()
{
  return score;
}

void Player::setPlayerID(int idx)
{
  player_id = idx;
}

void Player::setController(int idx)
{
  controller_id = idx;
}

void Player::setVelocity(ASGE::Point2D new_velocity)
{
  velocity = new_velocity;
}

void Player::setPosition(ASGE::Point2D new_pos)
{
  controlled_character->setPosition(new_pos);
}

void Player::setScale(float new_scale)
{
  SCALE = new_scale;
  controlled_character->sprite->scale(static_cast<float>(SCALE));
}

void Player::setCamera(int idx)
{
  camera_id = idx;
}

void Player::setScore(int new_score)
{
  score = new_score;
}

void Player::setCenter(ASGE::Point2D new_center)
{
  setPosition(ASGE::Point2D(
    new_center.x - getGlobalBounds().width / 2, new_center.y - getGlobalBounds().height / 2));
}

void Player::setCharacter(Character* new_char)
{
  controlled_character = new_char;
}

void Player::createNewCharacter(Character::CharacterType char_type)
{
  switch (char_type)
  {
    case Character::INVALID:
      break;
    case Character::HUMAN:
      /*controlled_character = std::make_unique<Human>();*/
      controlled_character = new Human();
      break;
    case Character::ANIMAL:
      /*controlled_character = std::make_unique<Animal>();*/
      controlled_character = new Animal();
      break;
  }
  controlled_character->init(0, 0);
}

void Player::createNewCharacter(Animal::AnimalType animal_type)
{
  /*controlled_character = std::make_unique<Animal>();*/
  controlled_character = new Animal();
  controlled_character->init(0, 0);
  dynamic_cast<Animal*>(controlled_character)->setAnimalType(animal_type);
}

void Player::resetCharacter()
{
  controlled_character->setHealth(20);
}

void Player::resetCharacter(ASGE::Point2D new_position)
{
  controlled_character->resetHP();
  controlled_character->resetGuesses();
  controlled_character->setInMinigame(false);
  setPosition(new_position);
}

void Player::updateFrames(const ASGE::GameTime& us, float gamepad_deadzone)
{
  gamepad_deadzone = abs(gamepad_deadzone);

  if (abs(velocity.x) > gamepad_deadzone || abs(velocity.y) > gamepad_deadzone)
  {
    controlled_character->animationTick(us);
  }
  else
  {
    controlled_character->animationIdle();
  }
}

float Player::FindDistanceToPoint(ASGE::Point2D point)
{
  float dif_x = abs(getCenter().x - point.x);
  float dif_y = abs(getCenter().y - point.y);

  float dist = sqrtf((dif_x * dif_x) + (dif_y * dif_y));
  return dist;
}

void Player::takeDamage(int damage)
{
  controlled_character->takeDamage(damage);
}