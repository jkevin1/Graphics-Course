#ifndef VEC3_H_INCLUDED
#define VEC3_H_INCLUDED

struct Vec3 {
    float x, y, z;

    Vec3();
    Vec3(float x0, float y0, float z0);
    Vec3(const Vec3& v);

    float mag() const;
    float lengthSquared() const;

    Vec3 operator+(const Vec3& rhs) const;
    Vec3 operator-(const Vec3& rhs) const;
    Vec3 operator*(float rhs) const;
    Vec3 operator/(float rhs) const;

    void operator+=(const Vec3& rhs);
    void operator-=(const Vec3& rhs);
    void operator*=(float rhs);
    void operator/=(float rhs);

    static float dot(const Vec3& a, const Vec3& b);
    static Vec3 cross(const Vec3& a, const Vec3& b);
    static float angle(const Vec3& a, const Vec3& b);
};

#endif // VEC3_H_INCLUDED
