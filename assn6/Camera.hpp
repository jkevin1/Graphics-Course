#ifndef CAMERA_hpp
#define CAMERA_hpp

#include "Vec.hpp"

#define X_SENSITIVITY 0.005f
#define Y_SENSITIVITY 0.005f
#define MOVESPEED 0.05f
#define CAMHEIGHT 1.0f
#define F_PI 3.14159265f

class Camera {
	friend class View;	//so it can access position and direction
	friend class Input;	//so it can access position and direction
	friend class Recording;	//so it can access position and direction
	friend class Heightmap;	//so it can access position and direction
	Vec<3> position;
	float yaw, pitch;
public:
	Camera();
	Camera(Vec<3> pos, float yaw, float pitch);

	void translate(Vec<3> displacement);
	void move(float forward, float strafe);
	void look(float dYaw, float dPitch);
};

#endif //CAMERA_hpp