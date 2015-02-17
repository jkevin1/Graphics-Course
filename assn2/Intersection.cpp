// implementation code for Intersection class

// include this class include file FIRST to ensure that it has
// everything it needs for internal self-consistency
#include "Intersection.hpp"

// other classes used directly in the implementation
#include "Object.hpp"
#include "World.hpp"


// new intersection with object and intersection location
Intersection::Intersection(const Object *_obj, float _t, const Ray& r, int rec) {
	t = _t;
	d_obj = _obj;
	pos = r.start + (r.direction * t);
	dir = r.direction;
	N = normalize(_obj->normal(pos));
	recursion = rec;
}

// return color for one intersection
const Vec3 
Intersection::color(const World &w) const {
	if (d_obj) {
		Vec3 clr =  w.lights.calcLighting(d_obj->material, pos, dir, N, w.objects);
		if (d_obj->material.Ks > 0.0f && recursion < 5) {
			Vec3 reflection = dir - (N * 2.0f * dot(dir, N));
			Intersection next = w.objects.trace(Ray(pos, reflection));
			next.recursion = recursion + 1;
			clr = clr + (next.color(w) * d_obj->material.Ks);
		}
		return clr;
	} else {
		// background color
		return w.background;
	}
}
