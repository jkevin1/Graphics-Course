#ifndef SCENE_H
#define SCENE_H

#include <stdio.h>
#include "vec3.h"

struct Color {
	unsigned char r, g, b;

	Color();
	Color(unsigned char r0, unsigned char g0, unsigned char b0);
	Color(float r0, float g0, float b0);
};

struct Triangle {
	Vec3 a, b, c;
	Triangle* next;
	
	Triangle(const Vec3& v1, const Vec3& v2, const Vec3& v3);
	~Triangle();

	bool intersection(const Vec3& pos, const Vec3& dir);
};

struct Scene {
	int width, height;
	Vec3 eyePos;
	Vec3 target;
	Vec3 upDir;
	float fov;
	Color fg, bg;
	Triangle* triangles;
};

Scene loadScene(FILE* src);

#endif // SCENE_H
