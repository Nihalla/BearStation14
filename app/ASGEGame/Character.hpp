
#ifndef ASGEGAME_CHARACTER_HPP
#define ASGEGAME_CHARACTER_HPP
#include "Boundary.h"
#include "GameObject.hpp"

class Character : public GameObject
{
 public:
  Character()           = default;
  ~Character() override = default;

  enum CharacterType
  {
    INVALID = -1,
    HUMAN   = 0,
    ANIMAL  = 1
  };

  enum GuessColours
  {
    DEFAULT = -1,
    YELLOW  = 0,
    BLUE    = 1,
    RED     = 2,
    GREEN   = 3
  };

  virtual void init(float x_pos, float y_pos);
  virtual void init(ASGE::Point2D new_pos);

  [[nodiscard]] int getHealth() const;
  [[nodiscard]] int getDamage() const;
  [[nodiscard]] int getSpeed() const;
  [[nodiscard]] float getRange() const;
  Boundary getGlobalBounds() const;
  ASGE::Point2D getPosition();
  double getCD();
  CharacterType getCharacterType();

  void setHealth(int new_hp);
  void setSpeed(int new_spd);
  void setPosition(float x_pos, float y_pos);
  void setPosition(ASGE::Point2D new_pos);
  void setHasAttacked(bool state);
  void setNearObjective(bool near);
  void setInMinigame(bool minigame_status);
  void setCD(double new_cd);

  void takeDamage(int dmg);

  void tickCD(double delta_in_sec);
  bool hasAttacked();
  bool isNearObjective();
  bool isInMinigame();

  virtual void addGuess(GuessColours guess);
  virtual std::vector<GuessColours> getGuesses();
  virtual void resetGuesses();

  void animationTick(const ASGE::GameTime& us);
  void animationIdle();
  void setSpriteSource();

  void resetHP();
  void resetSpeed();

 protected:
  CharacterType char_type = INVALID;
  int health              = 10;
  int init_health         = 10;
  int damage              = 10;
  int speed               = 10;
  int init_speed          = 10;
  float range             = 5.F;
  bool has_attacked       = false;
  bool in_minigame        = false;
  bool near_objective     = false;
  double attack_cd        = 0;
  ASGE::Point2D position;

  float animation_frames[4][2] = { { 0, 0 }, { 0, 33 }, { 0, 66 }, { 0, 99 } };
  int current_frame            = 0;
  float FRAME_RATE             = 0.07f;
  float animation_timer;
};

#endif // ASGEGAME_CHARACTER_HPP
