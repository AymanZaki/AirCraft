#include "Model.h"


Model::Model()
{
}


Model::~Model()
{
	Cleanup();
}

void Model::Initialize()
{
	glGenVertexArrays(1, &VertexArrayObjectID);
	glBindVertexArray(VertexArrayObjectID);

	if (VertexData.size() > 0)
	{
		glGenBuffers(1, &VertexDataBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, VertexDataBufferID);
		glBufferData(GL_ARRAY_BUFFER, VertexData.size() * sizeof(glm::vec3), &VertexData[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}
	if (IndicesData.size() > 0){
		glGenBuffers(1, &IndicesDataBufferID);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndicesDataBufferID);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(short) * IndicesData.size(), &IndicesData[0], GL_STATIC_DRAW);
	}
	
	if (ColorData.size() > 0)
	{
		glGenBuffers(1, &ColorDataBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, ColorDataBufferID);
		glBufferData(GL_ARRAY_BUFFER, ColorData.size() * sizeof(vec3), &ColorData[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

	if (UVData.size() > 0)
	{
		glGenBuffers(1, &UVDataBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, UVDataBufferID);
		glBufferData(GL_ARRAY_BUFFER, UVData.size() * sizeof(vec2), &UVData[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}
	if (NormalData.size() > 0)
	{
		glGenBuffers(1, &NormalsBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, NormalsBufferID);
		glBufferData(GL_ARRAY_BUFFER, NormalData.size() * sizeof(vec3), &NormalData[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	}

}

void Model::Draw()
{
	glBindVertexArray(VertexArrayObjectID); 
	if (IndicesData.size() > 0)
	{
		glDrawElements(GL_TRIANGLES, IndicesData.size(), GL_UNSIGNED_SHORT, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, VertexData.size());
	}
}

void Model::Cleanup()
{
	glDeleteBuffers(1, &VertexDataBufferID);
	glDeleteBuffers(1, &ColorDataBufferID);
	glDeleteBuffers(1, &IndicesDataBufferID);
	glDeleteBuffers(1, &UVDataBufferID);
	glDeleteBuffers(1, &NormalsBufferID);
	glDeleteVertexArrays(1, &VertexArrayObjectID);
}
