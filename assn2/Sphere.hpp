// sphere objects
#ifndef SPHERE_HPP
#define SPHERE_HPP

// other classes we use DIRECTLY in the interface
#include "Object.hpp"
#include "Vec3.hpp"

// classes we only use by pointer or reference
class World;
class Ray;

// sphere objects
class Sphere : public Object {
	Vec3 d_center;
	float d_radius;

public:	// constructors
	Sphere(const Material& mat, const Vec3 &_center, float _radius)
		: Object(mat) 
	{
		d_center = _center;
		d_radius = _radius;
	}

public: // object functions
	const Intersection intersect(const Ray &ray) const;
	Vec3 normal(const Vec3& pos) const;
};

#endif
