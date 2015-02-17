// implementation code for Sphere object class

// include this class include file FIRST to ensure that it has
// everything it needs for internal self-consistency
#include "Sphere.hpp"

// other classes used directly in the implementation
#include "Ray.hpp"

// sphere-ray intersection
const Intersection
Sphere::intersect(const Ray &r) const
{
	// solve p=r.start-center + t*r.direction; p*p -radius^2=0
	float a = dot(r.direction, r.direction);
	Vec3 dc = r.start - d_center;
	float b = 2 * dot(r.direction, dc);
	float c = dot(dc,dc) - (d_radius*d_radius);

	float discriminant = b*b - 4*a*c;
	if (discriminant < 0)	// no intersection
		return Intersection();

	// solve quadratic equation for desired surface
	float dsq = sqrtf(discriminant);
	float t = (-b - dsq) / (2*a); // first intersection within ray extent?
	if (t > r.near && t < r.far) 
		return Intersection(this,t,r);

	t = (-b + dsq) / (2*a);	     // second intersection within ray extent?
	if (t > r.near && t < r.far) 
		return Intersection(this,t,r);
  
	return Intersection();		// sphere entirely behind start point
}

Vec3 Sphere::normal(const Vec3& pos) const {
	return normalize(pos - d_center);
}
