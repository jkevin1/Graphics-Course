// information about our view of the world
// expected to change up to once per frame

#include "View.hpp"
#include "Geometry.hpp"
#include "Mat.inl"
#include "Vec.inl"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef F_PI
#define F_PI 3.1415926f
#endif

// create and initialize view
View::View(GLFWwindow *win) {
	// default view in spherical coordinates (theta, phi, distance)
	viewSph = Vec3(0, -1.4f, 500);

	// create uniform buffer objects
	glGenBuffers(1, &frameUniformBufferID);
	glBindBuffer(GL_UNIFORM_BUFFER, frameUniformBufferID);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(Geometry::FrameUniforms), 0,
				 GL_STREAM_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, Geometry::FRAME_UNIFORMS, 
					 frameUniformBufferID);

	// tell OpenGL to handle overlapping surfaces for this view
	glEnable(GL_DEPTH_TEST);

	// initialize scene data
	update(win);
}

View::~View() {
	glDeleteBuffers(1, &frameUniformBufferID);
}

// New view, pointing to origin, at specified angle
void View::update(GLFWwindow *win) {
	// set viewport from window dimensions
	glfwGetFramebufferSize(win, &width, &height);
	glViewport(0, 0, width, height);

	// adjust 3D projection into this window
	frameUniforms.projMats = Xform::perspective(
		F_PI/4, (float)width/height, 1, 10000);

	// update view matrix
	frameUniforms.viewMats = Xform::translate(Vec3(0,0,-viewSph.z))
		* Xform::xrotate(viewSph.y) * Xform::zrotate(viewSph.x);

	// update shader data
	glBindBuffer(GL_UNIFORM_BUFFER, frameUniformBufferID);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(Geometry::FrameUniforms),
					&frameUniforms);
}
