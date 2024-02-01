
#ifndef ASGEGAME_BOUNDARY_H
#define ASGEGAME_BOUNDARY_H
#include <Engine/Point2D.hpp>

class Boundary
{
 public:
  ASGE::Point2D position;
  float width  = 0;
  float height = 0;

  void build(float x, float y, float new_width, float new_height);
  [[nodiscard]] ASGE::Point2D v1() const;
  [[nodiscard]] ASGE::Point2D v2() const;
  [[nodiscard]] ASGE::Point2D v3() const;
  [[nodiscard]] ASGE::Point2D v4() const;
};

#endif // ASGEGAME_BOUNDARY_H
