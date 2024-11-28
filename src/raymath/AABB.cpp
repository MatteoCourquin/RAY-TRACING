#include "AABB.hpp"

AABB::AABB() : Min(Vector3()), Max(Vector3()) {}

AABB::AABB(Vector3 min, Vector3 max) : Min(min), Max(max) {}

AABB::~AABB() {}

AABB& AABB::operator=(AABB const &vec) {
    Min = vec.Min;
    Max = vec.Max;
    return *this;
}

void AABB::subsume(AABB const &other) {
    Min = Vector3(
        std::min(Min.x, other.Min.x),
        std::min(Min.y, other.Min.y),
        std::min(Min.z, other.Min.z)
    );
    
    Max = Vector3(
        std::max(Max.x, other.Max.x),
        std::max(Max.y, other.Max.y),
        std::max(Max.z, other.Max.z)
    );
}

bool AABB::intersects(Ray &r) {
    const Vector3& dir = r.GetDirection();
    const Vector3& pos = r.GetPosition();
    
    double tx1 = (Min.x - pos.x) * (1.0/dir.x);
    double tx2 = (Max.x - pos.x) * (1.0/dir.x);
    double tmin = std::min(tx1, tx2);
    double tmax = std::max(tx1, tx2);

    double ty1 = (Min.y - pos.y) * (1.0/dir.y);
    double ty2 = (Max.y - pos.y) * (1.0/dir.y);
    tmin = std::max(tmin, std::min(ty1, ty2));
    tmax = std::min(tmax, std::max(ty1, ty2));

    double tz1 = (Min.z - pos.z) * (1.0/dir.z);
    double tz2 = (Max.z - pos.z) * (1.0/dir.z);
    tmin = std::max(tmin, std::min(tz1, tz2));
    tmax = std::min(tmax, std::max(tz1, tz2));

    return tmax >= tmin && tmax > 0;
}

std::ostream& operator<<(std::ostream &_stream, AABB const &box) {
    return _stream << "AABB(Min:" << box.Min << ", Max:" << box.Max << ")";
}