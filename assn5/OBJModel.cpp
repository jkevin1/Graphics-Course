// load an OBJ model from file

#ifdef _WIN32
// don't complain about MS-deprecated standard C functions
#pragma warning( disable: 4996 )
#endif

#include "OBJModel.hpp"
#include "Geometry.hpp"
#include "Textures.hpp"
#include "Vec.inl"
#include "Mat.inl"
#include <vector>
#include <stdio.h>

//Buffer to store each line that is read
#define BUFFER_SIZE 256
using std::vector;

//Struct to represent each unique vertex (pos, norm, and uv) and has a equality operator to test uniqueness
struct Vert {
	unsigned int pos, norm, uv;
	bool operator==(const Vert& other) {
		return pos == other.pos && norm == other.norm && uv == other.uv;
	}
};

// load the object data
OBJModel::OBJModel(class Geometry &geom, class Textures &tex, const char* filename, Vec<3> center, Vec<3> scale, Vec<3> rotation) {
	//Vectors to store the data as it is read
	vector<Vec<3>> pos;
	vector<Vec<3>> norm;
	vector<Vec<2>> uv;
	vector<Vert> verts;
	vector<Geometry::Index> indices;

	//Open the file
	printf("Loading %s\n", filename);
	FILE* file = fopen(filename, "r");
	if (!file) {
		fprintf(stderr, "Failed, could not open file.\n");
		return;
	}

	//Iterate through each line and read in data
	char buffer[BUFFER_SIZE];
	while (fgets(buffer, BUFFER_SIZE, file) != NULL) {
		//Variables for scanf's
		Vec<3> vec;
		Vert vert;
		if (buffer[0] == 'v') {
			if (buffer[1] == ' ') {
				sscanf(buffer, "v %f %f %f", &vec.x, &vec.y, &vec.z);
				pos.push_back(vec);
			} else if (buffer[1] == 't') {
				sscanf(buffer, "vt %f %f", &vec.x, &vec.y);
				uv.push_back(vec.xy);
			} else if (buffer[1] == 'n') {
				sscanf(buffer, "vn %f %f %f", &vec.x, &vec.y, &vec.z);
				norm.push_back(vec);
			}
		} else if (buffer[0] == 'f') {
			char* str = buffer;
			while (*str != '\0') {
				if (*str == ' ') {
					sscanf(str + 1, "%d/%d/%d", &vert.pos, &vert.uv, &vert.norm);
					bool dup = false;	//flag for whether it is a duplicate vertex
					for (unsigned int i = 0; i < verts.size(); i++) {
						if (verts[i] == vert) {	//if duplicate, add the correct index to the array and set flag
							indices.push_back(i);
							dup = true;
						}
					}
					if (!dup) {	//if not duplicate, add it to the vertex and index array
						indices.push_back(verts.size());
						verts.push_back(vert);
					}
				}
				str++;
			}
		}
	}

	//Print some info about the loaded file
	printf("Loaded %d positions\n", pos.size());
	printf("Loaded %d normals\n", norm.size());
	printf("Loaded %d texture coordinates\n", uv.size());
	printf("Loaded %d unique vertices\n", verts.size());
	printf("Loaded %d indices\n", indices.size());

	//Close the file
	fclose(file);

	//Allocate vertex and index arrays
	drawID = geom.addDraw(verts.size(), indices.size());
	Geometry::Vertex* vertices = geom.getVertices(drawID);
	Geometry::Index* tris = geom.getIndices(drawID);
	numTri = indices.size() / 3;

	//Copy vertex array to Geometry
	for (unsigned int i = 0; i < verts.size(); i++) {
		vertices[i].pos = pos[verts[i].pos - 1];
		vertices[i].norm = norm[verts[i].norm - 1];
		vertices[i].uv = uv[verts[i].uv - 1];
	}

	//Copy index array to Geometry
	for (unsigned int i = 0; i < indices.size(); i++) {
		tris[i] = indices[i];
	}

	//Same material as other models, but transforms based on the arguments
	Geometry::ModelUniforms &unif = geom.getModelUniforms(drawID);
	Xform rot = Xform::xrotate(rotation.x) * Xform::yrotate(rotation.y) * Xform::zrotate(rotation.z);
	unif.modelMats = Xform::translate(center) * Xform::scale(scale) * rot;
	unif.KdMap = tex.findOrLoad("pebbles-a.pam");
	unif.KsMap = Vec2(-1,-1);
	unif.Kd = Vec4(1,1,1,1);
	unif.Ks = Vec3(0.04f, 0.04f, 0.04f);
	unif.Ns = 32.f;
}
