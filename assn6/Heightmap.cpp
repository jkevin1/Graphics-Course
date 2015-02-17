#include "Heightmap.hpp"
#include "Vec.inl"

Heightmap::Heightmap() {
	num = 0;
	vert = NULL;
	ind = NULL;
}

Heightmap::Heightmap(unsigned int numTri, Geometry::Vertex* vertices, Geometry::Index* tris) {
	num = numTri;
	vert = vertices;
	ind = tris;
}

bool Heightmap::calculateHeight(Camera &cam) {
	for (unsigned int tri = 0; tri < num; tri++) {
		Vec<3> a = vert[ind[3*tri + 0]].pos;
		Vec<3> b = vert[ind[3*tri + 1]].pos;
		Vec<3> c = vert[ind[3*tri + 2]].pos;
		Vec<3> start = Vec3(cam.position.x, cam.position.y, 0);

		Vec<3> c1 = a - b;
		Vec<3> c2 = a - c;
		Vec<3> c3 = Vec3(0, 0, 1);
		Vec<3> c4 = a - start;

		//The dummy variables a-l in the book
		float A = c1.x, B = c1.y, C = c1.z;
		float D = c2.x, E = c2.y, F = c2.z;
		float G = c3.x, H = c3.y, I = c3.z;
		float J = c4.x, K = c4.y, L = c4.z;

		float M = A*(E*I - H*F) + B*(G*F - D*I) + C*(D*H - E*G);

		float t = -(F*(A*K - J*B) + E*(J*C - A*L) + D*(B*L - K*C)) / M;
		//if (t < 0.0f) continue; //the intersection is behind the eye 

		float gamma = (I*(A*K - J*B) + H*(J*C - A*L) + G*(B*L - K*C)) / M;
		if (gamma < 0.0f || gamma > 1.0f) continue;

		float beta = (J*(E*I - H*F) + K*(G*F - D*I) + L*(D*H - E*G)) / M;
		if (beta < 0.0f || beta > 1.0f - gamma) continue;

		cam.position.z = t - CAMHEIGHT;
		return true;
	}
	return false;
}