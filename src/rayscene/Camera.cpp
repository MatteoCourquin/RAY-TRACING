#include <iostream>
#include <cmath>
#include <thread>
#include <vector>
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

#ifndef THREADING_ENABLED
  RenderSegment segment;
  segment.height = height;
  segment.image = &image;
  segment.scene = &scene;
  segment.intervalX = intervalX;
  segment.intervalY = intervalY;
  segment.reflections = Reflections;
  segment.rowMin = 0;
  segment.rowMax = image.height;

  renderSegment(&segment);
#else

  // ? Get threas depend hardware
  auto num_threads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;

  // ? Calculate rows / thread
  int rows_per_thread = image.height / num_threads;

  // ? Create segments for each thread
  std::vector<RenderSegment *> segments;
  for (int i = 0; i < num_threads; ++i)
  {
    RenderSegment *seg = new RenderSegment();
    seg->height = height;
    seg->image = &image;
    seg->scene = &scene;
    seg->intervalX = intervalX;
    seg->intervalY = intervalY;
    seg->reflections = Reflections;
    seg->rowMin = i * rows_per_thread;
    seg->rowMax = (i == num_threads - 1) ? image.height : (i + 1) * rows_per_thread;
    segments.push_back(seg);
  }

  // ? Start threads
  for (unsigned int i = 0; i < num_threads; ++i)
  {
    threads.emplace_back(renderSegment, segments[i]);
  }

  // ? Assemble threads
  for (auto &thread : threads)
  {
    thread.join();
  }
#endif
}

std::ostream &operator<<(std::ostream &_stream, Camera &cam)
{
  Vector3 pos = cam.getPosition();
  return _stream << "Camera(" << pos << ")";
}