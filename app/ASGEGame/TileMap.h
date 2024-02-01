
#ifndef ASGEGAME_TILEMAP_H
#define ASGEGAME_TILEMAP_H

#include "Boundary.h"
#include <Engine/Sprite.hpp>
#include <tmxlite/Map.hpp>
class TileMap
{
 public:
  TileMap();
  tmx::Map map;
  std::vector<std::unique_ptr<ASGE::Sprite>> tiles;
  std::vector<std::unique_ptr<Boundary>> collisions;
  std::vector<std::unique_ptr<ASGE::Point2D>> spawnpoints;
  std::vector<std::unique_ptr<Boundary>> objectives;

  std::string filepath = "/data/map/GameMap.tmx";

  void storeCollisions(tmx::Layer& layer);
  void storeSpawns(tmx::Layer& layer);
  void storeObjectives(tmx::Layer& layer);
  void handleObjects(tmx::Layer& layer);
  bool checkCollision(Boundary boundary);
  bool
  checkCollision(Boundary boundary, const std::vector<std::unique_ptr<Boundary>>& collision_list);
  bool checkCollisionBetweenBounds(Boundary bound_1, Boundary bound_2);
  float getTileSize();
  int scale = 2;

 private:
  bool pointCollision(ASGE::Point2D point, Boundary object);
};

#endif // ASGEGAME_TILEMAP_H
