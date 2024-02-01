#ifndef ASGEGAME_GAMECOMPONENT_HPP
#define ASGEGAME_GAMECOMPONENT_HPP

#include <Engine/GameTime.hpp>
class GameComponent
{
 public:
  enum class ID
  {
    INVALID = -1
  };
  GameComponent()          = default;
  virtual ~GameComponent() = default;

  ID getID();
  void setID(ID idx);

  virtual void update(ASGE::GameTime& us) {}

 private:
  ID id_ = ID::INVALID;
};

#endif // ASGEGAME_GAMECOMPONENT_HPP
