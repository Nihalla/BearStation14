#include "GameComponent.hpp"
GameComponent::ID GameComponent::getID()
{
  return id_;
}
void GameComponent::setID(ID idx)
{
  id_ = idx;
}
