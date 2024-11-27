#include <iostream>
#include "Mesh.hpp"
#include "../raymath/Vector3.hpp"
#include "../objloader/OBJ_Loader.h"

#include <iostream>
#include "Mesh.hpp"
#include "../raymath/Vector3.hpp"
#include "../objloader/OBJ_Loader.h"

Mesh::Mesh() : SceneObject()
{
}

Mesh::~Mesh()
{
  for (int i = 0; i < triangles.size(); ++i)
  {
    delete triangles[i];
  }
}

void Mesh::calculateBoundingBox()
{
  // ? Init bounding box with infinite values
  Vector3 minPt(INFINITY, INFINITY, INFINITY);
  Vector3 maxPt(-INFINITY, -INFINITY, -INFINITY);

  // ? Loop through all triangles to find min and max points
  for (const auto &triangle : triangles)
  {
    for (const Vector3 *point : {&triangle->tA, &triangle->tB, &triangle->tC})
    {
      minPt = Vector3(
          std::min(minPt.x, point->x),
          std::min(minPt.y, point->y),
          std::min(minPt.z, point->z));

      maxPt = Vector3(
          std::max(maxPt.x, point->x),
          std::max(maxPt.y, point->y),
          std::max(maxPt.z, point->z));
    }
  }

  boundingBox = AABB(minPt, maxPt);
}

void Mesh::loadFromObj(std::string path)
{
  objl::Loader *loader = new objl::Loader();
  bool loadout = loader->LoadFile(path);

  if (loadout)
  {
    for (int i = 0; i < loader->LoadedMeshes.size(); i++)
    {
      objl::Mesh curMesh = loader->LoadedMeshes[i];

      for (int j = 0; j < curMesh.Indices.size(); j += 3)
      {
        Vector3 v1(
            curMesh.Vertices[curMesh.Indices[j]].Position.X,
            curMesh.Vertices[curMesh.Indices[j]].Position.Y,
            curMesh.Vertices[curMesh.Indices[j]].Position.Z);
        Vector3 v2(
            curMesh.Vertices[curMesh.Indices[j + 1]].Position.X,
            curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y,
            curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z);
        Vector3 v3(
            curMesh.Vertices[curMesh.Indices[j + 2]].Position.X,
            curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y,
            curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z);

        Triangle *triangle = new Triangle(v1, v2, v3);
        triangle->name = "T:" + std::to_string(j);
        triangle->ID = j;
        triangles.push_back(triangle);
      }
    }
  }

  this->applyTransform();
  delete loader;
}

void Mesh::applyTransform()
{
  for (int i = 0; i < triangles.size(); ++i)
  {
    triangles[i]->material = this->material;
    triangles[i]->transform = transform;
    triangles[i]->applyTransform();
  }
  calculateBoundingBox();
}

bool Mesh::intersects(Ray &r, Intersection &intersection, CullingType culling)
{
  // ? Check if ray intersects bounding box
  if (!boundingBox.intersects(r))
  {
    return false;
  }

  Intersection tInter;
  double closestDistance = -1;
  Intersection closestInter;

  for (int i = 0; i < triangles.size(); ++i)
  {
    if (triangles[i]->intersects(r, tInter, culling))
    {
      tInter.Distance = (tInter.Position - r.GetPosition()).length();
      if (closestDistance < 0 || tInter.Distance < closestDistance)
      {
        closestDistance = tInter.Distance;
        closestInter = tInter;
      }
    }
  }

  if (closestDistance < 0)
  {
    return false;
  }

  intersection = closestInter;
  return true;
}