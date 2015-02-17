// implementation code for LightList class
// list of lights in scene

// include this class include file FIRST to ensure that it has
// everything it needs for internal self-consistency
#include "LightList.hpp"
#include <math.h>

using std::max;

Vec3 LightList::calcLighting(const Material& m, const Vec3& pos, const Vec3& dir, const Vec3& N, const ObjectList& obj) const {
	Vec3 clr(0.0f,0.0f,0.0f);
	float intensity = 1.0f / sqrtf(list.size());
	for (unsigned int l = 0; l < list.size(); l++) {
		Vec3 L = list[l] - pos;
		float dist = length(L);
		L = normalize(L);
		Intersection shadow = obj.trace(Ray(pos, L));
		if (shadow.t > dist) {
			Vec3 toEye = normalize(dir * -1.0f);
			Vec3 H = normalize(toEye + L);
			float diffuse = max(0.0f, dot(N, L));
			float specular = pow(max(0.0f, dot(N, H)), m.exp);
			float spec = m.Ks * specular;
			Vec3 curr = (m.Kd * m.color) + Vec3(spec, spec, spec);
			clr = clr + (curr * diffuse * intensity);
		}
	}
	return clr;
}
