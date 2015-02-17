#include "vec3.h"
#include <math.h>


Vec3::Vec3() : x(0.0f), y(0.0f), z(0.0f) { }
Vec3::Vec3(float x0, float y0, float z0) : x(x0), y(y0), z(z0) { }
Vec3::Vec3(const Vec3& v) : x(v.x), y(v.y), z(v.z) { }

float Vec3::mag() const {
    return sqrtf(lengthSquared());
}

float Vec3::lengthSquared() const {
    return x*x + y*y + z*z;
}

Vec3 Vec3::operator+(const Vec3& rhs) const {
    return Vec3(x+rhs.x, y+rhs.y, z+rhs.z);
}

Vec3 Vec3::operator-(const Vec3& rhs) const {
    return Vec3(x-rhs.x, y-rhs.y, z-rhs.z);
}

Vec3 Vec3::operator*(float rhs) const {
    return Vec3(x*rhs, y*rhs, z*rhs);
}

Vec3 Vec3::operator/(float rhs) const {
    return Vec3(x/rhs, y/rhs, z/rhs);
}

void Vec3::operator+=(const Vec3& rhs) {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
}

void Vec3::operator-=(const Vec3& rhs) {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
}

void Vec3::operator*=(float rhs) {
    x *= rhs;
    y *= rhs;
    z *= rhs;
}

void Vec3::operator/=(float rhs) {
    x /= rhs;
    y /= rhs;
    z /= rhs;
}

float Vec3::dot(const Vec3& a, const Vec3& b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vec3 Vec3::cross(const Vec3& a, const Vec3& b) {
    return Vec3(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
}

float Vec3::angle(const Vec3& a, const Vec3& b) {
    return acosf(dot(a, b) / a.mag() / b.mag());
}
