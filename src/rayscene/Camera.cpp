#include <iostream>
#include <cmath>
#include "Camera.hpp"
#include "../raymath/Ray.hpp"

struct RenderSegment
{
public:
  int rowMin;
  int rowMax;
  Image *image;
  double height;
  double intervalX;
  double intervalY;
  int reflections;
  Scene *scene;
};

Camera::Camera() : position(Vector3())
{
}

Camera::Camera(Vector3 pos) : position(pos)
{
}

Camera::~Camera()
{
}

Vector3 Camera::getPosition()
{
  return position;
}

void Camera::setPosition(Vector3 &pos)
{
  position = pos;
}

/**
 * Render a segment (set of rows) of the image
 */
void renderSegment(RenderSegment *segment)
{

  for (int y = segment->rowMin; y < segment->rowMax; ++y)
  {
    double yCoord = (segment->height / 2.0) - (y * segment->intervalY);

    for (int x = 0; x < segment->image->width; ++x)
    {
      double xCoord = -0.5 + (x * segment->intervalX);

      Vector3 coord(xCoord, yCoord, 0);
      Vector3 origin(0, 0, -1);
      Ray ray(origin, coord - origin);

      Color pixel = segment->scene->raycast(ray, ray, 0, segment->reflections);
      segment->image->setPixel(x, y, pixel);
    }
  }
}

void Camera::render(Image &image, Scene &scene)
{

  double ratio = (double)image.width / (double)image.height;
  double height = 1.0 / ratio;

  double intervalX = 1.0 / (double)image.width;
  double intervalY = height / (double)image.height;

  scene.prepare();

  RenderSegment *seg = new RenderSegment();
  seg->height = height;
  seg->image = &image;
  seg->scene = &scene;
  seg->intervalX = intervalX;
  seg->intervalY = intervalY;
  seg->reflections = Reflections;
  seg->rowMin = 0;
  seg->rowMax = image.height;
  renderSegment(seg);
}

std::ostream &operator<<(std::ostream &_stream, Camera &cam)
{
  Vector3 pos = cam.getPosition();
  return _stream << "Camera(" << pos << ")";
}
