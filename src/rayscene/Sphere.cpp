#include <iostream>
#include <cmath>
#include "Sphere.hpp"
#include "../raymath/Vector3.hpp"

Sphere::Sphere(double r) : SceneObject(), radius(r)
{
  updateBoundingBox();
}

Sphere::~Sphere()
{
}

void Sphere::updateBoundingBox()
{
  // ? Find min and max points from center and radius
  Vector3 minPt(
      center.x - radius,
      center.y - radius,
      center.z - radius);

  Vector3 maxPt(
      center.x + radius,
      center.y + radius,
      center.z + radius);

  boundingBox = AABB(minPt, maxPt);
}

void Sphere::applyTransform()
{
  Vector3 c;
  this->center = this->transform.apply(c);
  updateBoundingBox();
}

void Sphere::countPrimes()
{
  int prime_counter = 0;
  for (int n = 2; n < 1000; n++)
  {
    int count = 0;
    for (int i = 2; i <= i / 2; i++)
    {
      if (n % i == 0)
      {
        count++;
      }
      if (count == 0)
      {
        prime_counter++;
      }
    }
  }
}

bool Sphere::intersects(Ray &r, Intersection &intersection, CullingType culling)
{
  if (!boundingBox.intersects(r))
  {
    return false;
  }

  Vector3 OC = center - r.GetPosition();
  Vector3 OP = OC.projectOn(r.GetDirection());

  if (OP.dot(r.GetDirection()) <= 0)
  {
    return false;
  }

  Vector3 P = r.GetPosition() + OP;
  Vector3 CP = P - center;
  double distance = CP.length();
  if (distance > radius)
  {
    return false;
  }

  double a = sqrt(radius * radius - distance * distance);
  double t = OP.length() - a;
  Vector3 P1 = r.GetPosition() + (r.GetDirection() * t);

  intersection.Position = P1;
  intersection.Mat = this->material;
  intersection.Normal = (P1 - center).normalize();

  countPrimes();

  return true;
}