#include "scene.h"

void loadPolygon(FILE* src, Scene* scene, int vertices);

Color::Color() {
	r = g = b = 0;
}

Color::Color(unsigned char r0, unsigned char g0, unsigned char b0) {
	r = r0;
	g = g0;
	b = b0;
}

Color::Color(float r0, float g0, float b0) {
	r = (r0<0) ? 0 : (r0>1) ? 255 : (unsigned char)(r0*255);
	g = (g0<0) ? 0 : (g0>1) ? 255 : (unsigned char)(g0*255);
	b = (b0<0) ? 0 : (b0>1) ? 255 : (unsigned char)(b0*255);
}

Triangle::Triangle(const Vec3& v1, const Vec3& v2, const Vec3& v3) {
	a = v1;
	b = v2;
	c = v3;
}

Triangle::~Triangle() {
	delete next;
}

bool Triangle::intersection(const Vec3& pos, const Vec3& dir) {
	//The following is a direct translation from the textbook into c++
	Vec3 c1 = a - b;
	Vec3 c2 = a - c;
	Vec3 c3 = dir;
	Vec3 c4 = a - pos;
	
	//The dummy variables a-l in the book
	float A = c1.x, B = c1.y, C = c1.z;
	float D = c2.x, E = c2.y, F = c2.z;
	float G = c3.x, H = c3.y, I = c3.z;
	float J = c4.x, K = c4.y, L = c4.z;

	float M = A*(E*I - H*F) + B*(G*F - D*I) + C*(D*H - E*G);

	float t = -(F*(A*K - J*B) + E*(J*C - A*L) + D*(B*L - K*C)) / M;
	if (t < 0.0f) return false; //the intersection is behind the eye 

	float gamma = (I*(A*K - J*B) + H*(J*C - A*L) + G*(B*L - K*C)) / M;
	if (gamma < 0.0f || gamma > 1.0f) return false;

	float beta = (J*(E*I - H*F) + K*(G*F - D*I) + L*(D*H - E*G)) / M;
	if (beta < 0.0f || beta > 1.0f - gamma) return false;

	return true;
}

#define BUFFER_SIZE 128
Scene loadScene(FILE* src) {
	//Buffer and Scene object
	char buffer[BUFFER_SIZE];
	Scene scene;
	scene.triangles = NULL;

	//variables used in scanfs
	int vertices;
	float hither;
	float r, g, b;

	//Begin loading and counting triangles
	int triCount = 0;
	printf("Loading NFF scene...");

	//Iterate through each line of the file
	while(fgets(buffer, BUFFER_SIZE, src) != NULL) {
		switch(buffer[0]) {
		case 'v':	//view
			fscanf(src, "from %f %f %f\n", &scene.eyePos.x, &scene.eyePos.y, &scene.eyePos.z);
			fscanf(src, "at %f %f %f\n", &scene.target.x, &scene.target.y, &scene.target.z);
			fscanf(src, "up %f %f %f\n", &scene.upDir.x, &scene.upDir.y, &scene.upDir.z);
			fscanf(src, "angle %f\n", &scene.fov);
			fscanf(src, "hither %f\n", &hither);
			fscanf(src, "resolution %d %d\n", &scene.width, &scene.height);
			break;
		case 'b':	//background color
			sscanf(buffer, "b %f %f %f", &r, &g, &b);
			scene.bg = Color(r, g, b);
			break;
		case 'l':	//light
			break;
		case 'f':	//material
			sscanf(buffer, "f %f %f %f", &r, &g, &b);
			scene.fg = Color(r, g, b);
			break;
		case 'c':	//cone/cylinder
			break;
		case 's':	//sphere
			break;
		case 'p':	//polygon or polygonal patch
			if (buffer[1] == 'p') break;
			sscanf(buffer, "p %d", &vertices);
			loadPolygon(src, &scene, vertices);
			triCount += vertices - 2;
			break;
		}
	}

	//Done loading file
	printf("COMPLETE: loaded %d triangles\n", triCount);
	return scene;
}

void loadPolygon(FILE* src, Scene* scene, int vertices) {
	//Vertices used for fscanf
	Vec3 v1, v2, v3;

	//Load first two vertices
	fscanf(src, "%f %f %f\n", &v1.x, &v1.y, &v1.z);
	fscanf(src, "%f %f %f\n", &v2.x, &v2.y, &v2.z);

	//Iterate through remaining vertices and add to linked list
	for (int i = 2; i < vertices; i++) {
		fscanf(src, "%f %f %f\n", &v3.x, &v3.y, &v3.z);
		Triangle* next = new Triangle(v1, v2, v3);
		next->next = scene->triangles;
		scene->triangles = next;
	}
}
