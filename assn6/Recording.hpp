#ifndef Recording_hpp
#define Recording_hpp

#include "Camera.hpp"
#include "View.hpp"
#include <vector>

class Recording {
	std::vector<Camera> keyframes;
public:
	void start(const Camera& cam);
	void addFrame(const Camera& cam);
	void end(const Camera& cam);

	bool playback(float time, View& view, GLFWwindow* win);
};

#endif Recording_hpp