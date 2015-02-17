#include "Recording.hpp"
#include "Vec.inl"

template<class T>
T catmullRom(float t, const T& v0, const T& v1, const T& v2, const T& v3);

void Recording::start(const Camera& cam) {
	keyframes.clear();
	keyframes.push_back(cam);
	keyframes.push_back(cam);
}

void Recording::addFrame(const Camera& cam) {
	keyframes.push_back(cam);
}

void Recording::end(const Camera& cam) {
	keyframes.push_back(cam);
	keyframes.push_back(cam);
}

bool Recording::playback(float time, View& view, GLFWwindow* win) {
	time += 1.0f;
	unsigned int frame = (unsigned int)time;
	if (frame + 2 >= keyframes.size()) return true;
	Camera c0 = keyframes[frame - 1];
	Camera c1 = keyframes[frame + 0];
	Camera c2 = keyframes[frame + 1];
	Camera c3 = keyframes[frame + 2];
	view.cam.position = catmullRom(time - frame, c0.position, c1.position, c2.position, c3.position);
	view.cam.yaw = catmullRom(time - frame, c0.yaw, c1.yaw, c2.yaw, c3.yaw);
	view.cam.pitch = catmullRom(time - frame, c0.pitch, c1.pitch, c2.pitch, c3.pitch);
	view.update(win);
	return false;
}

template<class T>
T catmullRom(float t, const T& v0, const T& v1, const T& v2, const T& v3) {
	T c0 = v1;
	T c1 = (v0 * -0.5f) + (v2 * 0.5);
	T c2 = v0 + (v1 * -2.5f) + (v2 * 2.0f) + (v3 * -0.5f);
	T c3 = (v0 * -0.5f) + (v1 * 1.5f) + (v2 * -1.5f) + (v3 * 0.5);
	return ((c3*t + c2) * t + c1) * t + c0;
}
