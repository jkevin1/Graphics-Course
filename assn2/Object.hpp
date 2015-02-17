// virtual class for any object
#ifndef OBJECT_HPP
#define OBJECT_HPP

// other classes we use DIRECTLY in our interface
#include "Intersection.hpp"
#include "Material.hpp"

// classes we only use by pointer or reference
class World;
class Ray;

class Object {
public:							// public data
	Material material;				// this object's color
  
public:	// constructor & destructor
	Object();
	Object(const Material& mat);
	virtual ~Object();

  
public: // computational members
	// return t for closest intersection with ray
	virtual const Intersection intersect(const Ray &ray) const = 0;
	virtual Vec3 normal(const Vec3& pos) const = 0;
};

#endif
