#ifndef HEIGHTMAP_hpp
#define HEIGHTMAP_hpp

#include "Camera.hpp"
#include "Geometry.hpp"
#include "Vec.hpp"

class Heightmap {
	unsigned int num;
	Geometry::Vertex* vert;
	Geometry::Index* ind;
public:
	Heightmap();
	Heightmap(unsigned int numTri, Geometry::Vertex* vertices, Geometry::Index* tris);

	bool calculateHeight(Camera &cam);
};

#endif //HEIGHTMAP_hpp