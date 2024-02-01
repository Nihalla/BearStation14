#include "GameObject.hpp"

void GameObject::update(ASGE::GameTime& us)
{
  std::for_each(
    std::begin(components),
    std::end(components),
    [&us](GameComponent& component) { component.update(us); });
}
void GameObject::addComponent(GameComponent component)
{
  components.emplace_back(component);
}
std::vector<GameComponent>* GameObject::getComponents()
{
  return &components;
}
