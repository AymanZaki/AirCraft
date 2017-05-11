#pragma once
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
using namespace glm;

class Camera
{
	//vec3 Center;
	vec3 Right;
	vec3 Up;
	vec3 Direction;
	mat4 ViewMatrix;
public:
	vec3 Position;
	vec3 LastPosition;
	mat4 GetViewMatrix();
	vec3 GetLookDirection();
	void Fly(float);
	void Walk(float);
	void Strafe(float);
	void Yaw(float);
	void Pitch(float);
	void Roll(float);
	void UpdateViewMatrix();
	void Reset();
	bool ThirdPerson;
	Camera();
	Camera(bool);
	~Camera();
};