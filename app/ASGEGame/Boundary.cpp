
#include "Boundary.h"
void Boundary::build(float x, float y, float new_width, float new_height)
{
  position.x = x;
  position.y = y;
  width      = new_width;
  height     = new_height;
}
ASGE::Point2D Boundary::v1() const
{
  return { position.x, position.y };
}
ASGE::Point2D Boundary::v2() const
{
  return { position.x + width, position.y };
}
ASGE::Point2D Boundary::v3() const
{
  return { position.x, position.y + height };
}
ASGE::Point2D Boundary::v4() const
{
  return { position.x + width, position.y + height };
}
