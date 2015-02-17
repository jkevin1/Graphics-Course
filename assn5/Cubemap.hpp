#ifndef Cubemap_hpp
#define Cubemap_hpp

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Cubemap {
	GLuint id;
public:
	Cubemap(const char* files_fmt, int order[6], int texture);
	~Cubemap();
};

#endif // Cubemap_hpp