#include <gl/glew.h>
#include "shader.hpp"
#include "Camera.h"
#include "texture.h"
#include "Model.h"
#include <gl/glfw3.h>
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#pragma once

using namespace glm;
using namespace std;

#define COLOR_ONLY 0
#define TEXTURE_ONLY 1
#define COLOR_TEXTURE 2

class Renderer
{
	GLuint programID;
	GLuint ModelMatrixID;
	GLuint MatrixID;
	GLuint ColorID;
	GLuint Color;
	GLuint RenderingModeID;
	GLuint LightPositionID;
	GLuint AmbientLightID;
	GLuint EyePositionID;

	Texture *AircraftTexture;
	Texture *RightTexture;
	Texture *FrontTexture;
	Texture *LeftTexture;
	Texture *BackTexture;
	Texture *UpTexture;
	Texture *DownTexture;

	mat4 ProjectionMatrix;
	mat4 ViewMatrix;
	mat4 AircraftM;
	mat4 bottom;
	mat4 left;
	mat4 behind;
	mat4 top;
	mat4 front;
	mat4 right;
	mat4 PV;
		
	vec3 lightPosition;
	vec3 ambientLight;	

	Model SkyBox;
	Model Aircraft;

	Camera camera = Camera(true);

	bool Switch;

	float Pitch, Yaw, Yawangle, angle;
	float acceleration;
	float deltaTime;
	static float SkyBoxLength;
	static float Distance;

	double time;
	short RenderingMODE;
	

	float CalcSpeed(float);
	void PitchMovment();
	void YawMovment();
public:
	Renderer();
	~Renderer();
	void Update();
	void HandleKeyboardInput(int);
	void HandleMouse(double, double);
	void Initialize();
	void Draw();
	void Cleanup();
	static float GetSkyBoxLength();
	void InitializeTexture(short);

	static const vec3 AircraftFirstPosition;
};
