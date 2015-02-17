// lists of objects in scene
#ifndef LIGHTLIST_HPP
#define LIGHTLIST_HPP

// other classes we use DIRECTLY in our interface
#include "Vec3.hpp"
#include "Material.hpp"
#include "ObjectList.hpp"

// system includes
#include <vector>

class World;
class LightList {
	// list of objects
	std::vector<Vec3> list;

public:	// constructor & destructor
	LightList() {}
	
	void addLight(const Vec3& pos) {list.push_back(pos);}
	Vec3 calcLighting(const Material& m, const Vec3& pos, const Vec3& dir, const Vec3& N, const ObjectList& obj) const;
};

#endif
