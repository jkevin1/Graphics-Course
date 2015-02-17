// information about our view of the world
// data expected to change at most once per frame
#ifndef View_hpp
#define View_hpp

#include "Geometry.hpp"
#include "Camera.hpp"
#include "Vec.hpp"
#include "Mat.hpp"

struct GLFWwindow;

class View {
// private data
private:
	// GL uniform buffer IDs
	unsigned int frameUniformBufferID;
	Geometry::FrameUniforms frameUniforms;

// public data
public:
	int width, height;		// current window dimensions
	Camera cam;

// public methods
public:
	View(GLFWwindow *win);
	~View();

	void update(GLFWwindow *win);	// update after data change
};

#endif
