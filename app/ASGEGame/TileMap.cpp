

#include "TileMap.h"
#include <Engine/FileIO.hpp>
#include <Engine/Logger.hpp>
#include <tmxlite/TileLayer.hpp>

TileMap::TileMap()
{
  spawnpoints.reserve(5);
}

void TileMap::storeCollisions(tmx::Layer& layer)
{
  auto object_group = layer.getLayerAs<tmx::ObjectGroup>();

  auto objects = object_group.getObjects();

  for (auto& object : objects)
  {
    Boundary temp_rect;
    temp_rect.build(
      object.getPosition().x * static_cast<float>(scale),
      object.getPosition().y * static_cast<float>(scale),
      object.getAABB().width * static_cast<float>(scale),
      object.getAABB().height * static_cast<float>(scale));
    collisions.emplace_back(std::make_unique<Boundary>(temp_rect));
  }
}

void TileMap::storeSpawns(tmx::Layer& layer)
{
  auto spawnpoint_group = layer.getLayerAs<tmx::ObjectGroup>();

  auto points = spawnpoint_group.getObjects();

  spawnpoints.emplace_back(std::make_unique<ASGE::Point2D>());
  spawnpoints.emplace_back(std::make_unique<ASGE::Point2D>());

  for (auto& point : points)
  {
    if (point.getName() == "Human Spawn")
    {
      *spawnpoints[0] = ASGE::Point2D(
        point.getPosition().x * static_cast<float>(scale),
        point.getPosition().y * static_cast<float>(scale));
    }
    else if (point.getName() == "Monster Spawn")
    {
      *spawnpoints[1] = ASGE::Point2D(
        point.getPosition().x * static_cast<float>(scale),
        point.getPosition().y * static_cast<float>(scale));
    }
  }
}

void TileMap::storeObjectives(tmx::Layer& layer)
{
  auto object_group = layer.getLayerAs<tmx::ObjectGroup>();

  auto objects = object_group.getObjects();

  for (auto& object : objects)
  {
    Boundary temp_rect;
    temp_rect.build(
      object.getPosition().x * static_cast<float>(scale),
      object.getPosition().y * static_cast<float>(scale),
      object.getAABB().width * static_cast<float>(scale),
      object.getAABB().height * static_cast<float>(scale));
    objectives.emplace_back(std::make_unique<Boundary>(temp_rect));
  }
}

/// Catch all function for object layers in case of an unaccounted object layer
void TileMap::handleObjects(tmx::Layer& layer)
{
  /*auto object_group = layer.getLayerAs<tmx::ObjectGroup>();

  auto objects = object_group.getObjects();

  for (auto& object : objects)
  {

  }*/
}

bool TileMap::checkCollision(Boundary boundary)
{
  for (auto& collision : collisions)
  {
    if (pointCollision(boundary.v1(), *collision))
    {
      return true;
    }
    if (pointCollision(boundary.v2(), *collision))
    {
      return true;
    }
    if (pointCollision(boundary.v3(), *collision))
    {
      return true;
    }
    if (pointCollision(boundary.v4(), *collision))
    {
      return true;
    }
    if (pointCollision(collision->v1(), boundary))
    {
      return true;
    }
    if (pointCollision(collision->v2(), boundary))
    {
      return true;
    }
    if (pointCollision(collision->v3(), boundary))
    {
      return true;
    }
    if (pointCollision(collision->v4(), boundary))
    {
      return true;
    }
  }
  return false;
}

bool TileMap::checkCollision(
  Boundary boundary, const std::vector<std::unique_ptr<Boundary>>& collision_list)
{
  for (auto& collision : collision_list)
  {
    if (pointCollision(boundary.v1(), *collision))
    {
      return true;
    }
    if (pointCollision(boundary.v2(), *collision))
    {
      return true;
    }
    if (pointCollision(boundary.v3(), *collision))
    {
      return true;
    }
    if (pointCollision(boundary.v4(), *collision))
    {
      return true;
    }
    if (pointCollision(collision->v1(), boundary))
    {
      return true;
    }
    if (pointCollision(collision->v2(), boundary))
    {
      return true;
    }
    if (pointCollision(collision->v3(), boundary))
    {
      return true;
    }
    if (pointCollision(collision->v4(), boundary))
    {
      return true;
    }
  }
  return false;
}

bool TileMap::checkCollisionBetweenBounds(Boundary bound_1, Boundary bound_2)
{
  if (pointCollision(bound_1.v1(), bound_2))
  {
    return true;
  }
  if (pointCollision(bound_1.v2(), bound_2))
  {
    return true;
  }
  if (pointCollision(bound_1.v3(), bound_2))
  {
    return true;
  }
  if (pointCollision(bound_1.v4(), bound_2))
  {
    return true;
  }

  return false;
}

bool TileMap::pointCollision(ASGE::Point2D point, Boundary object)
{
  if (object.v1().x <= point.x && point.x <= object.v2().x)
  {
    if (object.v1().y <= point.y && point.y <= object.v4().y)
    {
      return true;
    }
  }
  return false;
}

float TileMap::getTileSize()
{
  return tiles[0]->width();
}
