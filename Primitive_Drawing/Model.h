#pragma once
#include <glm/glm.hpp>
#include <gl/glew.h>
#include <vector>
using namespace glm;
using namespace std;
class Model
{
	GLuint VertexDataBufferID;
	GLuint ColorDataBufferID;
	GLuint IndicesDataBufferID;
	GLuint VertexArrayObjectID;
	GLuint UVDataBufferID;
	GLuint NormalsBufferID;
public:
	Model();
	~Model();
	vector<vec3> NormalData;
	vector<vec2> UVData;
	vector<vec3> VertexData;
	vector<vec3> ColorData;
	vector<short>IndicesData;
	vec3 Position;
	void Cleanup();
	void Initialize();
	void Draw();
};