// add sphere object to draw list
#ifndef Sphere_hpp
#define Sphere_hpp

#include "Vec.hpp"

struct Sphere {
	unsigned int drawID;

	// create sphere given radius and center
	Sphere(class Geometry &geom, class Textures &tex, 
		   float radius, Vec<3> center);
};

#endif
