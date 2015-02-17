// handle mouse and keyboard input

#include "Input.hpp"
#include "View.hpp"
#include "Geometry.hpp"
#include "Xform.hpp"
#include "Mat.inl"
#include "Vec.inl"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#ifndef F_PI
#define F_PI 3.1415926f
#endif

// initialize
Input::Input(GLFWwindow* win) {
	button = oldButton = -1;
	oldX = oldY = 0;
	wireframe = false;
	glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	mouseGrabbed = true;
}

// called when a mouse button is pressed.
// Remember where we were, and what mouse button it was.
bool Input::mousePress(GLFWwindow &win, int b, int action) {
	if (action == GLFW_PRESS) {
		// hide cursor, record button
		glfwSetInputMode(&win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
		mouseGrabbed = true;
		button = b;
	}
	else {
		// display cursor, update button state
		//glfwSetInputMode(&win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		button = -1;	   // no button
	}
	return false;				// no redraw yet
}

// called when the mouse moves
// use difference between oldX,oldY and x,y to define a rotation
bool Input::mouseMove(View &view, GLFWwindow &win, double x, double y) {
	// only update view after at least one old position is stored
	bool redraw = false;
	if (mouseGrabbed) {
		// record differences & update last position
		float dx = float(x - oldX);
		float dy = float(y - oldY);

		view.cam.look(dx, dy);
		view.update(&win);
		redraw = true;			// need to redraw
	}

	// update prior mouse state
	oldButton = button;
	oldX = x;
	oldY = y;
	return redraw;
}

// called when any key is pressed or released
bool Input::keyPress(Geometry &geom, GLFWwindow &win, int key, 
					 int action, int mods) 
{
	if (action == GLFW_PRESS && key=='R') {
		geom.updateShaders();	// reload shaders
		return true;			// need to redraw
	}

	if (action == GLFW_PRESS && key=='L') { // toggle wireframe ('L'ines)
		wireframe = !wireframe;
		glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
		return true;			// need to redraw
	}

	if (action == GLFW_PRESS && key == GLFW_KEY_ESCAPE) {
		glfwSetInputMode(&win, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
		mouseGrabbed = false;
	}

	return false;				// no redraw
}

// update view if necessary based on a/d keys
bool Input::keyUpdate(Geometry &geom, View &view, Heightmap &heightmap, GLFWwindow &win) {
	// elapsed time since last update
	double now = glfwGetTime();
	float dt = float(now - updateTime);
	updateTime = now;

	// move cylinder based on time elapsed since last update
	// ensures uniform rate of change
	moveRate = Vec2(0.0f, 0.0f);
	if (glfwGetKey(&win, GLFW_KEY_W) == GLFW_PRESS) moveRate.x += 25.0f;
	if (glfwGetKey(&win, GLFW_KEY_S) == GLFW_PRESS) moveRate.x -= 25.0f;
	if (glfwGetKey(&win, GLFW_KEY_A) == GLFW_PRESS) moveRate.y -= 25.0f;
	if (glfwGetKey(&win, GLFW_KEY_D) == GLFW_PRESS) moveRate.y += 25.0f;

	if (length(moveRate) > 0) {
		Vec<3> pos = view.cam.position;
		view.cam.move(moveRate.x * dt, moveRate.y * dt);
		if (!heightmap.calculateHeight(view.cam))
			view.cam.position = pos;
		view.update(&win);
		return true;			// need to redraw
	}
	return false;				// no redraw
}