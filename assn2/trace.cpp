// ray tracer main program
// includes input file parsing and spawning screen pixel rays

// classes used directly by this file
#include "ObjectList.hpp"
#include "Polygon.hpp"
#include "Sphere.hpp"
#include "Ray.hpp"
#include "World.hpp"
#include "Vec3.hpp"

// standard includes
#include <stdio.h>
#include <string.h>
#include <list>

#ifdef _WIN32
// don't complain about MS-deprecated standard C functions
#pragma warning( disable: 4996 )
#endif

//Ray-pixel intersections for antialiased and standard rendering
float ANTIALIAS_X[] = {1/8.0f, 3/8.0f, 5/8.0f, 7/8.0f};
float ANTIALIAS_Y[] = {3/8.0f, 7/8.0f, 1/8.0f, 5/8.0f};
float STANDARD_X[] = {0.5f};
float STANDARD_Y[] = {0.5f};

int main(int argc, char **argv)
{
	// input file from command line or stdin
	FILE *infile;
	if (argc <= 1)
		infile = stdin;
	else {
		infile = fopen(argv[1], "r");
		if (!infile) {
			fprintf(stderr, "error opening %s\n", argv[1]);
			return 1;
		}
	}

	// everything we know about the world
	// image parameters, camera parameters
	World world(infile);
	
	//Whether to do RGSS supersampling is stored in readme
	int antialias;
	FILE* readme = fopen("README.txt", "r");
	if (!readme || fscanf(readme, "antialias=%d\n", &antialias) != 1) antialias = 0;

	float* xpos = STANDARD_X;
	float* ypos = STANDARD_Y;
	int samples = 1;

	if (antialias) {
		printf("Using RGSS supersampling for antialiasing");
		xpos = ANTIALIAS_X;
		ypos = ANTIALIAS_Y;
		samples = 4;
	}

	// array of image data in ppm-file order
	unsigned char (*pixels)[3] = new unsigned char[world.height*world.width][3];

	// spawn a ray for each pixel and place the result in the pixel
	int i,j;				// pixel index on screen
	for(j=0; j<world.height; ++j) {

		if (j % (world.height / 15)  == 0) printf("%2d%% complete\n", (100*j) / world.height); // show current line
		for(i=0; i<world.width; ++i) {
			Vec3 col;

			for(int sample = 0; sample < samples; sample++) {
				// trace new ray
				float us = world.left + 
					(world.right - world.left) * (i+xpos[sample])/world.width;
				float vs = world.top + 
					(world.bottom - world.top) * (j+ypos[sample])/world.height;
				Vec3 dir = -world.dist * world.w + us * world.u + vs * world.v;

				Ray ray(world.eye, dir, world.hither / world.dist);
				col = col + world.objects.trace(ray).color(world);
			}
			col = col / (float)samples;

			// assign color
			pixels[j*world.width + i][0] = col.r();
			pixels[j*world.width + i][1] = col.g();
			pixels[j*world.width + i][2] = col.b();
		}
	}
	printf("done\n");

	// write pam file of pixels
	FILE *output = fopen("trace.pam","wb");
	fprintf(output, "P7\n");
	fprintf(output, "WIDTH %d\nHEIGHT %d\nDEPTH 3\n", 
			world.width, world.height);
	fprintf(output, "MAXVAL 255\nTYPLTYPE RGB\nENDHDR\n");
	fwrite(pixels, world.height*world.width*3, 1, output);
	fclose(output);
  
	delete[] pixels;
	return 0;
}
  
