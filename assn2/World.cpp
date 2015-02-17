// implementation code World object
// World holds everything we know about the world.
// Code here initializes it based on file input

// include this class include file FIRST to ensure that it has
// everything it needs for internal self-consistency
#include "World.hpp"

// local includes
#include "Polygon.hpp"
#include "Sphere.hpp"

// system includes
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#ifdef _WIN32
#pragma warning( disable: 4996 )
#endif


// report an error
static void err(int lineNum)
{
	fprintf(stderr, "NFF file error at line %d\n", lineNum);
	exit(1);
}

// read a line
static char *readLine(FILE *f, char line[1024], int &lineNumber)
{
	++lineNumber;
	return fgets(line, 1024, f);
}

// read input file
World::World(FILE *f)
{
	char line[1024];			// line of file
	int lineNumber = 0;			// current line for error reporting
	Material current;			// current object color

	while(readLine(f, line, lineNumber)) {
		switch(line[0]) {
		case ' ': case '\t':	// blank lines and comments
		case '\f': case '\r': case '\n':
		case '#': case '\0':
			break;

		case 'v': {			// view point
			// read view parameters
			Vec3 vAt, vUp;
			float angle;
			readLine(f, line, lineNumber);
			if (sscanf(line,"from %f %f %f", &eye[0], &eye[1], &eye[2]) != 3) 
				err(lineNumber);

			readLine(f, line, lineNumber);
			if (sscanf(line,"at %f %f %f", &vAt[0], &vAt[1], &vAt[2]) != 3)
				err(lineNumber);

			readLine(f, line, lineNumber);
			if (sscanf(line,"up %f %f %f", &vUp[0], &vUp[1], &vUp[2]) != 3) 
				err(lineNumber);

			readLine(f, line, lineNumber);
			if (sscanf(line,"angle %f", &angle) != 1) 
				err(lineNumber);

			readLine(f, line, lineNumber);
			if (sscanf(line,"hither %f", &hither) != 1) 
				err(lineNumber);

			readLine(f, line, lineNumber);
			if (sscanf(line, "resolution %d %d", &width, &height) != 2) 
				err(lineNumber);

			// compute view basis
			w = eye - vAt;
			dist = length(w);
			w = normalize(w);
			u = normalize(vUp ^ w);
			v = w ^ u;

			// solve w/2d = tan(fov/2), where w=2 and fov must be in radians
			float t = float(tan(angle * M_PI/360));
			top = dist*t;
			bottom = -top;
			right = top * width / height;
			left = -right;

			break;
		}

		case 'b': {			// background
			if (sscanf(line,"b %f %f %f",
					   &background[0], &background[1], &background[2]) != 3) 
				err(lineNumber);

			break;
		}

		case 'l': {			// light
			Vec3 pos;
			if (sscanf(line,"l %f %f %f", &pos[0], &pos[1], &pos[2]) != 3)
				err(lineNumber);
			lights.addLight(pos);
			printf("New light at <%f, %f, %f>\n", pos[0], pos[1], pos[2]);
			break;
		}

		case 'f': {			// fill/material properties
			// ignore all but the color
			if (sscanf(line,"f %f %f %f %f %f %f", 
					   &current.color[0], &current.color[1], &current.color[2], &current.Kd, &current.Ks, &current.exp) != 6) 
				err(lineNumber);
		   
			break;
		}

		case 'c':			// cone or cylinder: ignore
			break;

		case 's': {			// sphere
			Vec3 center;
			float radius;
			if (sscanf(line,"s %f %f %f %f",
					   &center[0], &center[1], &center[2], &radius) != 4) 
				err(lineNumber);

			objects.addObject(new Sphere(current, center, radius));
		
			break;
		}

		case 'p': {			// p or pp polygon primitives
			// number of vertices, starting after "p " or "pp"
			int nv;
			if (sscanf(line+2, "%d", &nv) != 1 || nv < 3) 
				err(lineNumber);

			// polygon primitive w/ type
			Polygon *poly = new Polygon(nv, current);

			// read vertices
			for(int i=0; i<nv; ++i) {
				Vec3 v;
				readLine(f, line, lineNumber);
				if (sscanf(line,"%f %f %f", &v[0], &v[1], &v[2]) != 3) 
					err(lineNumber);
				poly->addVertex(v);
			}

			poly->closePolygon();
			objects.addObject(poly);

			break;
		}
		default:
			err(lineNumber);
		}
	}
}
