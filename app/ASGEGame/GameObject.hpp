
#ifndef ASGEGAME_GAMEOBJECT_HPP
#define ASGEGAME_GAMEOBJECT_HPP
#include "GameComponent.hpp"
#include <Engine/GameTime.hpp>
#include <Engine/Sprite.hpp>
#include <vector>
class GameObject
{
 public:
  GameObject()          = default;
  virtual ~GameObject() = default;
  void update(ASGE::GameTime& us);
  void addComponent(GameComponent component);
  std::vector<GameComponent>* getComponents();
  std::unique_ptr<ASGE::Sprite> sprite = nullptr;

 private:
  std::vector<GameComponent> components = {};
  /// Maybe put sprite stuff into components/derived classes as not all gameobjects should have a
  /// sprite
};

#endif // ASGEGAME_GAMEOBJECT_HPP
