
#ifndef ASGEGAME_PLAYER_HPP
#define ASGEGAME_PLAYER_HPP
#include "Animal.hpp"
#include "Human.hpp"

class Player
{
 public:
  Player() = default;
  ~Player();

  [[nodiscard]] int getPlayerID() const;
  [[nodiscard]] int getController() const;
  [[nodiscard]] int getCamera() const;
  Character* getCharacter();
  ASGE::Point2D getVelocity();
  ASGE::Point2D getPosition();
  Boundary getGlobalBounds();
  ASGE::Point2D getCenter();
  float getRange();
  int getDamage();
  int getHP();
  float getScale();
  int getScore();

  void setPlayerID(int idx);
  void setController(int idx);
  void setVelocity(ASGE::Point2D new_velocity);
  void setPosition(ASGE::Point2D new_pos);
  void setCenter(ASGE::Point2D new_center);
  void setScale(float new_scale);
  void setCamera(int idx);
  void setScore(int new_score);
  void setCharacter(Character* new_char);

  void takeDamage(int damage);
  void createNewCharacter(Character::CharacterType char_type);
  void createNewCharacter(Animal::AnimalType animal_type);
  void resetCharacter();
  void resetCharacter(ASGE::Point2D new_position);

  void updateFrames(const ASGE::GameTime& us, float gamepad_deadzone);

  float FindDistanceToPoint(ASGE::Point2D point);

 private:
  int player_id     = -1;
  int controller_id = -1;
  int camera_id     = -1;
  float SCALE       = 1;
  int score         = 0;
  ASGE::Point2D velocity{ 0, 0 };
  /*std::unique_ptr<Character>*/ Character* controlled_character = nullptr;
};

#endif // ASGEGAME_PLAYER_HPP
