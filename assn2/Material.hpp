#ifndef MATERIAL_HPP
#define MATERIAL_HPP

#include "Vec3.hpp"

struct Material {
	Vec3 color;
	float Kd, Ks, exp;

	Material() { }
	Material(Vec3 c, float d, float s, float e) {
		color = c;
		Kd = d;
		Ks = s;
		exp = e;
	}
};

#endif //MATERIAL_HPP
