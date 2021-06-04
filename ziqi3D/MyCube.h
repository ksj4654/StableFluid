#pragma once

#include <vgl.h>
#include "vec.h""

struct MyCubeVertex
{
	vec4 position;
	vec4 color;
};

class MyCube
{
public:
	MyCube(void);
	~MyCube(void);

	static const int NumVertices = 36;

	MyCubeVertex Vertices[NumVertices];
	GLuint Init();
	void SetPositionAndColorAttribute(GLuint program);


	void ColorCube(vec4* vin, vec4* cin);
	void Quad(int a, int b, int c, int d, vec4* vin, vec4* cin);

	int NumCurVertices;

	GLuint vao;
	GLuint buffer;
	bool bInitialized;

	void Draw(GLuint program);
};



MyCube::MyCube(void)
{
	bInitialized = false;
	NumCurVertices = 0;
}

MyCube::~MyCube(void)
{
}

void MyCube::Quad(int a, int b, int c, int d, vec4* vin, vec4* cin)
{
	Vertices[NumCurVertices].position = vin[a]; Vertices[NumCurVertices].color = cin[a];
	NumCurVertices++;
	Vertices[NumCurVertices].position = vin[b]; Vertices[NumCurVertices].color = cin[b];
	NumCurVertices++;
	Vertices[NumCurVertices].position = vin[c]; Vertices[NumCurVertices].color = cin[c];
	NumCurVertices++;

	Vertices[NumCurVertices].position = vin[a]; Vertices[NumCurVertices].color = cin[a];
	NumCurVertices++;
	Vertices[NumCurVertices].position = vin[c]; Vertices[NumCurVertices].color = cin[c];
	NumCurVertices++;
	Vertices[NumCurVertices].position = vin[d]; Vertices[NumCurVertices].color = cin[d];
	NumCurVertices++;

}

void MyCube::ColorCube(vec4* vin, vec4* cin)
{
	Quad(1, 0, 3, 2, vin, cin);
	Quad(2, 3, 7, 6, vin, cin);
	Quad(3, 0, 4, 7, vin, cin);
	Quad(6, 5, 1, 2, vin, cin);
	Quad(4, 5, 6, 7, vin, cin);
	Quad(5, 4, 0, 1, vin, cin);
}


GLuint MyCube::Init()
{
	// The Cube should be initialized only once;
	if (bInitialized == true) return vao;

	vec4 vertex_positions[8] = {
		vec4(-0.5, -0.5,  0.5, 1.0),
		vec4(-0.5,  0.5,  0.5, 1.0),
		vec4(0.5,  0.5,  0.5, 1.0),
		vec4(0.5, -0.5,  0.5, 1.0),
		vec4(-0.5, -0.5, -0.5, 1.0),
		vec4(-0.5,  0.5, -0.5, 1.0),
		vec4(0.5,  0.5, -0.5, 1.0),
		vec4(0.5, -0.5, -0.5, 1.0),
	};

	vec4 vertex_colors[8] = {
		vec4(0, 0, 0, 1),
		vec4(1, 0, 0, 1),
		vec4(1, 1, 1, 1),
		vec4(0, 1, 0, 1),
		vec4(0, 0, 1, 1),
		vec4(1, 0, 1, 1),
		vec4(1, 1, 0, 1),
		vec4(0, 1, 1, 1)
	};

	ColorCube(vertex_positions, vertex_colors);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	bInitialized = true;
	return vao;
}

void MyCube::SetPositionAndColorAttribute(GLuint program)
{
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, sizeof(MyCubeVertex), BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, sizeof(MyCubeVertex), BUFFER_OFFSET(sizeof(vec4)));
}


void MyCube::Draw(GLuint program)
{
	if (!bInitialized) return;			// check whether it is initiazed or not. 

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	SetPositionAndColorAttribute(program);

	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}