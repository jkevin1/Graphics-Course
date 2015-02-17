// implementation code for Polygon object class

// include this class include file FIRST to ensure that it has
// everything it needs for internal self-consistency
#include "Polygon.hpp"

// other classes used directly in the implementation
#include "Ray.hpp"
#include "Intersection.hpp"

void
Polygon::addVertex(const Vec3 &v)
{
	d_vertex.push_back(PolyVert(v));
}

void
Polygon::closePolygon()
{
	// compute normal from first two edges
	VertexList::iterator v2 = d_vertex.begin(), v0 = v2++, v1 = v2++;
	Vec3 e1 = v1->v - v0->v, e2 = v2->v - v1->v;
	d_normal = normalize(e1 ^ e2);

	// tangent and bitangent (2nd tangent perpendicular to 1st)
	// use edge between first and last vertex
	// avoids having to test that edge, and makes loops easier
	d_tangent = normalize(d_vertex.front().v - d_vertex.back().v);
	d_bitangent = d_normal ^ d_tangent;

	// for each vertex, precompute dot product with tangent and bitangent
	for(VertexList::iterator v = d_vertex.begin(); v != d_vertex.end(); ++v) {
		v->v_t = dot(v->v, d_tangent);
		v->v_b = dot(v->v, d_bitangent);
	}

	// precompute dot product of first vertex with normal
	d_v0_n = dot(d_vertex.front().v, d_normal);
}

const Intersection
Polygon::intersect(const Ray &ray) const 
{
	// compute intersection point with plane
	float t = (d_v0_n - dot(d_normal, ray.start)) /
		dot(d_normal, ray.direction);

	if (t < ray.near || t > ray.far)
		return Intersection();	// not in ray bounds: no intersection

	Vec3 p = ray.start + ray.direction * t;

	// dot product of intersection with polygon tangent and bitangent
	float p_t = dot(p, d_tangent), p_b = dot(p, d_bitangent);

	// check if intersection is inside or outside
	// trace ray from p along a tangent vector and count even/odd intersections
	bool inside = false;
	VertexList::const_iterator v1 = d_vertex.begin(), v0 = v1++;
	for(; v1 != d_vertex.end(); v0 = v1, ++v1) {
		// does edge straddle test ray where q dot bitangent = p dot bitangent?
		float b0 = v1->v_b - p_b, b1 = p_b - v0->v_b;
		if ((b0 > 0) ^ (b1 < 0)) {
			// outbound on test ray?
			float q_t = (b0 * v0->v_t + b1 * v1->v_t)/(v1->v_b - v0->v_b);
			if (q_t > p_t)
				inside = !inside;
		}
	}

	if (inside) return Intersection(this,t,ray);
  
	return Intersection();		// no intersection
}

Vec3 Polygon::normal(const Vec3& pos) const {
	return d_normal;
}
