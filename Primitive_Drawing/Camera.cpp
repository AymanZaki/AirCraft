#include "Camera.h"
#include "Renderer.h"
#include<glm/gtx/rotate_vector.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/constants.hpp>

using namespace glm;

Camera::Camera()
{
	this->ThirdPerson = ThirdPerson;
	Reset();
}

Camera::Camera(bool ThirdPerson)
{
	this->ThirdPerson = ThirdPerson;
	Reset();
}


Camera::~Camera()
{

}

vec3 Camera::GetLookDirection()
{
	return -Direction;
}

mat4 Camera::GetViewMatrix()
{
	UpdateViewMatrix();
	return ViewMatrix;
}

void Camera::UpdateViewMatrix()
{
	ViewMatrix = lookAt(Position, Position + GetLookDirection(), Up);
}

void Camera::Walk(float dist)
{
	LastPosition = Position;
	Position -= dist * Direction;
	UpdateViewMatrix();
}

void Camera::Strafe(float dist)
{
	LastPosition = Position;
	Position += dist *Right;
	UpdateViewMatrix();
}

void Camera::Fly(float dist)
{
	LastPosition = Position;
	Position += dist * Up;
	UpdateViewMatrix();
}

void Camera::Yaw(float Angle)
{
	Direction = rotate(Direction, Angle, Up);
	Right = rotate(Right, Angle, Up);
	//if (this->ThirdPerson)
		//this->Position = rotate(this->Position, Angle, this->Up);
	UpdateViewMatrix();
}

void Camera::Pitch(float Angle)
{
	Up = rotate(Up, Angle, Right);
	Direction = rotate(Direction, Angle, Right);
	//if (this->ThirdPerson)
		//this->Position = rotate(this->Position, Angle, this->Right);
	UpdateViewMatrix();
}

void Camera::Roll(float Angle)
{
	Right = rotate(Right, Angle, Direction);
	Up = rotate(Up, Angle, Direction);
	//if (this->ThirdPerson)
		//this->Position = rotate(this->Position, Angle, this->Direction);
	UpdateViewMatrix();
}

void Camera::Reset()
{
	float SkyBoxLength = Renderer::GetSkyBoxLength();
	vec3 Center;// = vec3(0.0f, -(SkyBoxLength - 3.0f), -(SkyBoxLength - 20.0f));
	if (ThirdPerson){
		Center = vec3(0.0f, -(SkyBoxLength - 7.0f), -(SkyBoxLength - 30.0f));
		Position = vec3(0.0f, -(SkyBoxLength - 7.0f), -(SkyBoxLength - 1));
	}
	else
	{
		Center = vec3(0.0f, -(SkyBoxLength - 5.0f), -(SkyBoxLength - 30.0f));
		Position = vec3(0.0f, -(SkyBoxLength - 5.0f), -(SkyBoxLength - 20));
	}

	vec3 nUp = vec3(0, 1, 0);

	Direction = Position - Center;
	Right = normalize(cross(nUp, Direction));
	Up = cross(Direction, Right);
	UpdateViewMatrix();
}
