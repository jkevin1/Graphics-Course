// add OBJ model object to draw list
#ifndef OBJModel_hpp
#define OBJModel_hpp

#include "Vec.hpp"

struct OBJModel {
	unsigned int drawID;

	// load an obj model from file
	OBJModel(class Geometry &geom, class Textures &tex, const char* filename, Vec<3> center, Vec<3> scale, Vec<3> rotation);
};

#endif
