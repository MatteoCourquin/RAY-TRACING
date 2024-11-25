#include <iostream>
#include <cmath>
#include "Vector3.hpp"

Vector3::Vector3() : x(0), y(0), z(0)
{
}

Vector3::Vector3(double iX, double iY, double iZ) : x(iX), y(iY), z(iZ)
{
}

Vector3::~Vector3()
{
}

const Vector3 Vector3::operator+(Vector3 const &vec) const
{
  Vector3 c;
  c.x = x + vec.x;
  c.y = y + vec.y;
  c.z = z + vec.z;
  return c;
}

const Vector3 Vector3::operator-(Vector3 const &vec) const
{
  Vector3 c;
  c.x = x - vec.x;
  c.y = y - vec.y;
  c.z = z - vec.z;
  return c;
}

const Vector3 Vector3::operator*(double const &f) const
{
  Vector3 c;
  c.x = x * f;
  c.y = y * f;
  c.z = z * f;
  return c;
}

const Vector3 Vector3::operator/(double const &f) const
{
  Vector3 c;
  c.x = x / f;
  c.y = y / f;
  c.z = z / f;
  return c;
}

Vector3 &Vector3::operator=(Vector3 const &vec)
{
  x = vec.x;
  y = vec.y;
  z = vec.z;
  return *this;
}

double Vector3::length() const
{
  return std::sqrt(this->lengthSquared());
}

double Vector3::lengthSquared() const
{
  return (x * x + y * y + z * z);
}

const Vector3 Vector3::normalize() const
{
  double length = this->length();

  if (length == 0)
  {
    return Vector3();
  }
  return *this / length;
}

double Vector3::dot(Vector3 const &vec) const
{
  return (x * vec.x + y * vec.y + z * vec.z);
}

const Vector3 Vector3::projectOn(Vector3 const &vec) const
{
  return vec * this->dot(vec);
}

const Vector3 Vector3::reflect(Vector3 const &normal) const
{
  Vector3 proj = this->projectOn(normal) * -2;
  Vector3 reflectDir = proj + *this;
  return reflectDir;
}

const Vector3 Vector3::cross(Vector3 const &b) const
{
  Vector3 c(y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x);
  return c;
}

const Vector3 Vector3::inverse() const
{
  Vector3 c(1.0 / x, 1.0 / y, 1.0 / z);
  return c;
}

std::ostream &operator<<(std::ostream &_stream, Vector3 const &vec)
{
  return _stream << "(" << vec.x << "," << vec.y << "," << vec.z << ")";
}
