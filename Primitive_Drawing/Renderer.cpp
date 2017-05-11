#define _CRT_SECURE_NO_WARNINGS
#include "Renderer.h"
#include "Camera.h"
#include<glm\gtc\matrix_transform.hpp>
#include<glm\gtx\transform.hpp>
#include <Windows.h>
#include <gl/glew.h>
#include <gl/glfw3.h>
#include <iostream>
#include "objloader.hpp"

#include <Mmsystem.h>
#include <mciapi.h>
//these two headers are already included in the <Windows.h> header
#pragma comment(lib, "Winmm.lib")
using namespace glm;
using namespace std;

float Renderer::SkyBoxLength = 100000;
const vec3 Renderer::AircraftFirstPosition = vec3(0.0f, -(SkyBoxLength - 1.0f), -(SkyBoxLength - 30.0f));
float Renderer::Distance = 15;

Renderer::Renderer(){}

Renderer::~Renderer(){}

void Renderer::Initialize()
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	Pitch = Yaw = acceleration = Yawangle = 0;
	
	angle = 0.2;
	Switch = false;
	time = glfwGetTime();

	programID = LoadShaders("SimpleVertexShader.vertexshader", "SimpleFragmentShader.fragmentshader");
	MatrixID = glGetUniformLocation(programID, "MVP");
	ModelMatrixID = glGetUniformLocation(programID, "ModelMatrix");
	ColorID = glGetUniformLocation(programID, "Colors");
	RenderingModeID = glGetUniformLocation(programID, "RenderingMode");
	LightPositionID = glGetUniformLocation(programID, "LightPosition_worldspace");
	AmbientLightID = glGetUniformLocation(programID, "ambientLight");
	EyePositionID = glGetUniformLocation(programID, "EyePosition_worldspace");

	ambientLight = vec3(0.1, 0.1, 0.1);
	glUniform3fv(AmbientLightID, 1, &ambientLight[0]);

	ProjectionMatrix = perspective(45.0f, 1024.0f / 768.0f, 0.1f, 10000000.0f);

	//Initialize SkyBox
	SkyBox.VertexData = {
		vec3(1.0f, 1.0f, -1.0f),
		vec3(1.0f, 1.0f, 1.0f),
		vec3(-1.0f, 1.0f, 1.0f),
		vec3(-1.0f, 1.0f, -1.0f)
	};

	SkyBox.IndicesData = { 0, 1, 2, 0, 2, 3 };
	SkyBox.UVData = { vec2(1.0f, 1.0f),
		vec2(1.0f, 0.0f),
		vec2(0.0f, 0.0f),
		vec2(0.0f, 1.0f) };

	SkyBox.NormalData.push_back(vec3(0.0, 0.0, 1.0));
	SkyBox.NormalData.push_back(vec3(0.0, 0.0, 1.0));
	SkyBox.NormalData.push_back(vec3(0.0, 0.0, 1.0));
	SkyBox.NormalData.push_back(vec3(0.0, 0.0, 1.0));

	SkyBox.Initialize();

	//load Aircraft Model
	loadOBJ("Aircraft\\F-16C_FightingFalcon.obj", Aircraft.VertexData, Aircraft.UVData, Aircraft.NormalData);
	Aircraft.Initialize();
	Aircraft.Position = AircraftFirstPosition;
	PitchMovment();
	YawMovment();
	//load Textures
	AircraftTexture = new Texture("Aircraft\\F-16C_FightingFalcon_P01.png", 0);
	InitializeTexture(4);
	glUseProgram(programID);
}

void Renderer::Draw()
{
	glUseProgram(programID);
	// Clear the screen
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	lightPosition = vec3(0, 0, 0);
	glUniform3fv(LightPositionID, 1, &lightPosition[0]);
	 
	RenderingMODE = TEXTURE_ONLY;
	glUniform1i(RenderingModeID, RenderingMODE);
	PV = ProjectionMatrix * camera.GetViewMatrix();

	//Right
	vec3 EyePosition = vec3(0.0f, -SkyBoxLength / 2.0f, 0.0f);
	glUniform3fv(EyePositionID, 1, &(EyePosition)[0]);
	RightTexture->Bind();
	right = PV * translate(0.0f, -SkyBoxLength / 2.0f, 0.0f) * scale(SkyBoxLength, SkyBoxLength / 2.0f, SkyBoxLength) * rotate(90.0f, vec3(0.0f, 0.0f, 1.0f));
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &right[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &right[0][0]);
	SkyBox.Draw();

	//front
	EyePosition = vec3(0.0f, -SkyBoxLength / 2.0f, 0.0f);
	glUniform3fv(EyePositionID, 1, &(EyePosition)[0]);
	FrontTexture->Bind();
	front = PV * translate(0.0f, -SkyBoxLength / 2.0f, 0.0f) * scale(SkyBoxLength, SkyBoxLength / 2.0f, SkyBoxLength) * rotate(90.0f, vec3(-1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &front[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &front[0][0]);
	SkyBox.Draw();

	//left
	EyePosition = vec3(0.0f, -SkyBoxLength / 2.0f, 0.0f);
	glUniform3fv(EyePositionID, 1, &(EyePosition)[0]);
	LeftTexture->Bind();
	left = PV * translate(0.0f, -SkyBoxLength / 2.0f, 0.0f) * scale(SkyBoxLength, SkyBoxLength / 2.0f, SkyBoxLength) * rotate(90.0f, vec3(0.0f, 0.0f, -1.0f));
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &left[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &left[0][0]);
	SkyBox.Draw();

	//Back
	EyePosition = vec3(0.0f, 0.0f, SkyBoxLength / 2.0f);
	glUniform3fv(EyePositionID, 1, &(EyePosition)[0]);
	BackTexture->Bind();
	behind = PV * rotate(90.0f, vec3(SkyBoxLength, 0.0f, 0.0f)) * translate(0.0f, 0.0f, SkyBoxLength / 2.0f) * scale(SkyBoxLength, SkyBoxLength, SkyBoxLength / 2.0f);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &behind[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &behind[0][0]);
	SkyBox.Draw();

	//Up
	EyePosition = vec3(0.0f, -SkyBoxLength / 2.0f, 0.0f);
	glUniform3fv(EyePositionID, 1, &(EyePosition)[0]);
	UpTexture->Bind();
	top = PV * translate(0.0f, -SkyBoxLength / 2.0f, 0.0f) * scale(SkyBoxLength, SkyBoxLength/2.0f, SkyBoxLength);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &top[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &top[0][0]);
	SkyBox.Draw();

	//Down
	EyePosition = vec3(0.0f, -2 * SkyBoxLength, 0.0f);
	glUniform3fv(EyePositionID, 1, &(EyePosition)[0]);
	DownTexture->Bind();
	bottom = PV * translate(0.0f, -2 * SkyBoxLength, 0.0f) * scale(SkyBoxLength, SkyBoxLength, SkyBoxLength);
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &bottom[0][0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &bottom[0][0]);
	SkyBox.Draw();
	
	
	if (Switch)
	{		
		float vf = CalcSpeed(acceleration);
		if (vf > 2)vf = 2;
		camera.Walk(vf);

		camera.Position.y = min((SkyBoxLength)-30, camera.Position.y);
		camera.Position.y = max((-SkyBoxLength) + 30, camera.Position.y);

		camera.Position.x = min((SkyBoxLength) - 30, camera.Position.x);
		camera.Position.x = max((-SkyBoxLength) + 30, camera.Position.x);

		camera.Position.z = min((SkyBoxLength)-30, camera.Position.z);
		camera.Position.z = max((-SkyBoxLength) + 30, camera.Position.z);

		Aircraft.Position += camera.Position - camera.LastPosition;

	}
	lightPosition = Aircraft.Position + vec3(5, 0, 0);
	glUniform3fv(LightPositionID, 1, &lightPosition[0]);

	lightPosition = Aircraft.Position + vec3(-5, 0, 0);
	glUniform3fv(LightPositionID, 1, &lightPosition[0]);

	AircraftTexture->Bind();
	AircraftM = PV * translate(Aircraft.Position)
		* rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f))
		* rotate(180.0f, vec3(0.0f, 0.0f, 1.0f))
		* rotate(Pitch, vec3(1.0f, 0.0f, 0.0f))
		* rotate(Yaw, vec3(0.0f, 0.0f, 1.0f))
		* rotate(Yawangle, vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &AircraftM[0][0]);
	glUniform3fv(EyePositionID, 1, &(Aircraft.Position)[0]);
	glUniformMatrix4fv(ModelMatrixID, 1, GL_FALSE, &AircraftM[0][0]);
	Aircraft.Draw();
}
 
void Renderer::HandleKeyboardInput(int key)
{
	switch (key)
	{
	case GLFW_KEY_T://Switch Camera from First Person to Third
		if (!camera.ThirdPerson){
			Distance *= -1;
			PitchMovment();
			YawMovment();
		}
		camera.ThirdPerson = true;
		break;
	case GLFW_KEY_Y://Switch Camera from Third Person to First
		if (camera.ThirdPerson){
			Distance *= -1;
			PitchMovment();
			YawMovment();
		}
		camera.ThirdPerson = false;
		break;
	case GLFW_KEY_R://Reset
		Pitch = Yaw = Yawangle = 0;
		acceleration = 0;
		time = 0;
		Switch = false;
		PitchMovment();
		YawMovment();
		camera.Reset();
		break;
	case GLFW_KEY_UP:
	case GLFW_KEY_W:
		if (!Switch)
		PlaySound(TEXT("takeoff.wav"), NULL, SND_FILENAME | SND_ASYNC | SND_LOOP);
		Switch = true;
		if (CalcSpeed(acceleration) < 2)
		acceleration += 0.0005;
		break;
	case GLFW_KEY_DOWN:
	case GLFW_KEY_S:
		if (CalcSpeed(acceleration) > 0)
		acceleration -= 0.0005;
		if (acceleration <= 0)Switch = false;
		break;
	case GLFW_KEY_RIGHT:
	case GLFW_KEY_D:
		if (Switch){
			YawMovment();
			camera.Yaw(-angle);
			Yaw -= angle;
			if (Yawangle < 10)
				Yawangle += 0.2;
		}
		break;
	case GLFW_KEY_LEFT:
	case GLFW_KEY_A:
		if (Switch){
			YawMovment();
			camera.Yaw(angle);
			Yaw += angle;
			if (Yawangle > -10)
				Yawangle -= 0.2;
		}
		break;
	case GLFW_KEY_F://Fly
		if (Switch){
			PitchMovment();
			if (Pitch < 15){
				camera.Pitch(angle);
				Pitch += angle;
			}
		}
		break;
	case GLFW_KEY_L://Land
		if (Switch){
			PitchMovment();
			if (Pitch > -15){
				camera.Pitch(-angle);
				Pitch -= angle;
			}
		}
		break;
	default:
		break;
	}
	camera.UpdateViewMatrix();
}

void Renderer::Update()
{
	double currentTime = glfwGetTime();
	double deltaTime = (currentTime - time) * 1000;
	time = currentTime;
}

void Renderer::Cleanup()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(3);
	glDeleteProgram(programID);
}

void Renderer::HandleMouse(double deltaX, double deltaY)
{
	if (camera.ThirdPerson){
		camera.Yaw(deltaX);
		camera.Pitch(deltaY);
		PitchMovment();
		YawMovment();
	}
}

float Renderer::GetSkyBoxLength()
{
	return SkyBoxLength;
}

void Renderer::InitializeTexture(short x)
{
	/*if (x == 1)
	{
		RightTexture = new Texture("CloudyLightRays\\CloudyLightRaysRight2048.png", 1);
		FrontTexture = new Texture("CloudyLightRays\\CloudyLightRaysFront2048.png", 2);
		LeftTexture = new Texture("CloudyLightRays\\CloudyLightRaysLeft2048.png", 3);
		BackTexture = new Texture("CloudyLightRays\\CloudyLightRaysBack2048.png", 4);
		UpTexture = new Texture("CloudyLightRays\\CloudyLightRaysUp2048.png", 5);
		DownTexture = new Texture("CloudyLightRays\\CloudyLightRaysDown2048.png", 6);
	}
	else if (x == 2)
	{
		RightTexture = new Texture("SunSet\\SunSetRight2048.png", 1);
		FrontTexture = new Texture("SunSet\\SunSetFront2048.png", 2);
		LeftTexture = new Texture("SunSet\\SunSetLeft2048.png", 3);
		BackTexture = new Texture("SunSet\\SunSetBack2048.png", 4);
		UpTexture = new Texture("SunSet\\SunSetUp2048.png", 5);
		DownTexture = new Texture("SunSet\\SunSetDown2048.png", 6);
	}
	else if (x == 3)
	{
		RightTexture = new Texture("DarkStormy\\DarkStormyRight2048.png", 1);
		FrontTexture = new Texture("DarkStormy\\DarkStormyFront2048.png", 2);
		LeftTexture = new Texture("DarkStormy\\DarkStormyLeft2048.png", 3);
		BackTexture = new Texture("DarkStormy\\DarkStormyBack2048.png", 4);
		UpTexture = new Texture("DarkStormy\\DarkStormyUp2048.png", 5);
		DownTexture = new Texture("DarkStormy\\DarkStormyDown2048.png", 6);
	}
	else */if (x == 4)
	{
		RightTexture = new Texture("skybox\\right.png", 1);
		FrontTexture = new Texture("skybox\\front.png", 2);
		LeftTexture = new Texture("skybox\\left.png", 3);
		BackTexture = new Texture("skybox\\back.png", 4);
		UpTexture = new Texture("skybox\\top.png", 5);
		DownTexture = new Texture("skybox\\bottom.png", 6);
	}
}

float Renderer::CalcSpeed(float acc)
{
	return acc*time;
}

void Renderer::PitchMovment()
{
	vec3 Direction = camera.GetLookDirection();
	float angleInRadians = atan2(Direction.y, Direction.z);
	if (camera.ThirdPerson)
	angleInRadians /= 5;
	Aircraft.Position.y = camera.Position.y + sin(angleInRadians) * Distance;
	Aircraft.Position.z = camera.Position.z + cos(angleInRadians) * Distance;
}

void Renderer::YawMovment()
{
	vec3 Direction = camera.GetLookDirection();
	float angleInRadians = atan2(Direction.x, Direction.z);
	if (camera.ThirdPerson)
		angleInRadians /= 5;
	Aircraft.Position.x = camera.Position.x + sin(angleInRadians) * Distance;
	Aircraft.Position.z = camera.Position.z + cos(angleInRadians) * Distance;
}
