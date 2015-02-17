// polygon objects
#ifndef POLYGON_HPP
#define POLYGON_HPP

// other classes we use DIRECTLY in our interface
#include "Object.hpp"
#include "Vec3.hpp"

// system includes necessary for the interface
#include <vector>

// classes we only use by pointer or reference
class World;
class Ray;

class Polygon : public Object {
	struct PolyVert {
		Vec3 v;			// vertex

		// the rest are derived values used in intersection testing
		float v_t, v_b;		// v dot tangent and v dot bitangent
		PolyVert(const Vec3 &_v) { v = _v; }
	};
	typedef std::vector<PolyVert> VertexList;

	VertexList d_vertex;	// list of vertices
	Vec3 d_normal;			// face normal
	Vec3 d_tangent;			// face tangent
	Vec3 d_bitangent;		// second face tangent

	// derived values for intersection testing
	float d_v0_n;		// v0 dot d_normal

public:	// constructors
	Polygon(int verts, const Material& mat) : Object(mat) 
	{
		d_vertex.reserve(verts);
	}

public: // manipulators
	// add a new vertex to the polygon
	// given vertex and per-vertex normal
	void addVertex(const Vec3 &v);

	// close the polygon after the last vertex
	void closePolygon();
  
public: // object functions
	const Intersection intersect(const Ray &ray) const;
	Vec3 normal(const Vec3& pos) const;
};

#endif
