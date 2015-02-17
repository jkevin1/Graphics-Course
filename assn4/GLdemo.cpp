//
// Simple GL example
//

#include "Geometry.hpp"
#include "Textures.hpp"
#include "Input.hpp"
#include "View.hpp"
#include "OBJModel.hpp"
#include "Heightmap.hpp"
#include "Vec.inl"

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <assert.h>

// collected state for access in callbacks
struct AppContext {
	Geometry geom;				  // drawing data
	View view;					  // viewing data
	Input input;				  // user interface data
	bool redraw;				  // true if we need to redraw

	AppContext(GLFWwindow *win) : view(win), input(win) {
		redraw = true;

		// store context pointer to access application data from callbacks
		glfwSetWindowUserPointer(win, this);
	}
};

///////
// GLFW callbacks must use extern "C"
extern "C" {
	// called for GLFW error
	void winError(int error, const char *description) {
		fprintf(stderr, "GLFW error: %s\n", description);
	}

	// called whenever the window size changes
	void reshape(GLFWwindow *win, int width, int height) {
		AppContext *appctx = (AppContext*)glfwGetWindowUserPointer(win);
		appctx->view.update(win);
		appctx->redraw = true;
	}

	// called when mouse button is pressed
	void mousePress(GLFWwindow *win, int button, int action, int mods) {
		AppContext *appctx = (AppContext*)glfwGetWindowUserPointer(win);
		appctx->redraw |= appctx->input.mousePress(*win, button, action);
	}

	// called when mouse is moved
	void mouseMove(GLFWwindow *win, double x, double y) {
		AppContext *appctx = (AppContext*)glfwGetWindowUserPointer(win);
		appctx->redraw |= appctx->input.mouseMove(appctx->view, *win, x, y);
	}

	// called on any key press or release
	void keyPress(GLFWwindow *win, int key, int scancode, int action, int mods) {
		AppContext *appctx = (AppContext*)glfwGetWindowUserPointer(win);
		appctx->redraw |= appctx->input.keyPress(appctx->geom, *win, key, action, mods);
	}
}

// initialize GLFW - windows and interaction
GLFWwindow *initGLFW() {
	// set error callback before init
	glfwSetErrorCallback(winError);
	if (! glfwInit()) return 0;

	// OpenGL version: YOU MAY NEED TO ADJUST VERSION OR OPTIONS!
	// When figuring out the settings that will work for you, make
	// sure you can see error messages on console output.
	//
	// My driver needs FORWARD_COMPAT, but others will need to comment that out.
	// Likely changes for other versions:
	//	 OpenGL 4.0 (2010): configured for this already
	//	 OpenGL 3.3 (2010): change VERSION_MAJOR to 3, MINOR to 3
	//	   change "400 core" to "330 core" in the .vert and .frag files
	//	 OpenGL 3.2 (2009): change VERSION_MAJOR to 3, MINOR to 2
	//	   change "400 core" to "150 core" in the .vert and .frag files
	//	 OpenGL 3.1 (2009): change VERSION_MAJOR to 3, MINOR to 1
	//	   comment out PROFILE line
	//	   change "400 core" to "140" in the .vert and .frag files
	//	 OpenGL 3.0 (2008): does not support features we need
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// create window
	GLFWwindow *win = glfwCreateWindow(848, 480, "OpenGL Demo", 0, 0);
	if (! win) {
		glfwTerminate();
		return 0;
	}
	glfwMakeContextCurrent(win);

	// GLEW (GL Extension Wrangler) eases access to modern OpenGL functions
	glewExperimental = true;
	glewInit();

	// set functions to be called by GLFW when events happen
	glfwSetFramebufferSizeCallback(win, reshape);
	glfwSetKeyCallback(win, keyPress);
	glfwSetMouseButtonCallback(win, mousePress);
	glfwSetCursorPosCallback(win, mouseMove);

	return win;
}

int main(int argc, char *argv[]) {
	// set up window
	GLFWwindow *win = initGLFW();
	assert(win);				// window must exist

	// drawing infrastructure
	AppContext appctx(win);
	Textures tex;

	// create stuff to draw
	OBJModel terrain(appctx.geom, tex, "terrain.obj", Vec3(0, 0, 0), Vec3(1, 1, 1), Vec3(0, 0, 0));
	Heightmap heightmap(terrain.numTri, appctx.geom.getVertices(terrain.drawID), appctx.geom.getIndices(terrain.drawID));
	heightmap.calculateHeight(appctx.view.cam);
	appctx.redraw = true;
	appctx.geom.finalizeDrawData();

	// loop until GLFW says it's time to quit
	while (!glfwWindowShouldClose(win)) {
		// check for updates while a key is pressed
		appctx.redraw |= appctx.input.keyUpdate(appctx.geom, appctx.view, heightmap, *win);

		// do we need to redraw?
		if (appctx.redraw) {
			appctx.redraw = false; // don't draw again until something changes

			// clear old screen contents then draw
			glClearColor(1.f, 1.f, 1.f, 1.f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			appctx.geom.draw();
			glfwSwapBuffers(win);
		}

		glfwPollEvents();		// wait for user input
	}

	glfwDestroyWindow(win);
	glfwTerminate();

	return 0;
}
