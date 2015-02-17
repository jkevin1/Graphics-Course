#include <stdio.h>
#include <math.h>
#include "vec3.h"
#include "scene.h"

void init();
void render();
void write();
void close();

Color* data = NULL;
Scene scene;

int main(int argc, char* argv[]) {
	init();
	render();
	write();
	close();
	return 0;
}

void init() {
	//Load the NFF file from stdin
	scene = loadScene(stdin);
	//Allocate memory for the image
	data = new Color[scene.width * scene.height];
}

void render() {
	//Calculate basis vectors for the algorithm in the book
	Vec3 W = scene.eyePos - scene.target;
	Vec3 V = scene.upDir;
	Vec3 U = Vec3::cross(V, W);

	//Calculate the dimensions of the virtual image
	float d = W.mag();
	float halfHeight = d * tanf(scene.fov * M_PI / 360.0f);
	float halfWidth = scene.width * halfHeight / scene.height;

	//Normalize the vectors
	W /= W.mag();
	V /= V.mag();
	U /= U.mag();

	//Rendering loop
	printf("Rendering\n");
	for (int y = 0; y < scene.height; y++) {
		for (int x = 0; x < scene.width; x++) {
			//The algorithm from the book
			float u = (2.0f * halfWidth * (x + 0.5f) / scene.width) - halfWidth;
			float v = (2.0f * halfHeight * (y + 0.5f) / scene.height) - halfHeight;		
			
			Vec3 dir = (W * -d) + (V * -v) + (U * u);
			Triangle* tri = scene.triangles;
			
			//Set the background color initially	
			data[y * scene.width + x] = scene.bg;
			while (tri) {
				if (tri->intersection(scene.eyePos, dir)) {
					//Set the foreground color if an object is hit
					data[y * scene.width + x] = scene.fg;
					break;
				}
				tri = tri->next;
			}
		}
		//Output percent completion
		if (y % (scene.width / 10) == 0) printf("%d%%\n", (y * 100)  / scene.height); 
	}
	printf("COMPLETE\n");
}

void write() {
	FILE *f = fopen("trace.pam","wb");
	fprintf(f, "P7\n");
	fprintf(f, "WIDTH %d\nHEIGHT %d\nDEPTH 3\n", scene.width, scene.height);
	fprintf(f, "MAXVAL 255\nTUPLTYPE RGB\nENDHDR\n");
	fwrite(data, 1, scene.width * scene.height * 3, f);
	fclose(f);
}

void close() {
	delete scene.triangles;
	delete[] data;
}
