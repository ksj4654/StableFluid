/*


  Simple Demo for GLSL 2.0

  www.lighthouse3d.com

*/
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "glew.h"
#include "freeglut.h"
//#include "glut.h"
#include "textfile.h"
#include "solver.h"
#include "MyCube.h"

#include "vec.h"
#include "mat.h"
#include "vgl.h"

typedef bool wuBOOL;

MyCube cube;

const int windowWidth = 600;
const int windowHeight = 800;

int debug = 0;

GLfloat translationX;
GLfloat translationY;
GLfloat translationZ;

GLfloat rotationX;
GLfloat rotationY;

int N;
Solver solver;
GLfloat alpha = 0.05;
wuBOOL drawVelocity = true;


GLuint v,f,f2,p;

GLuint uMat;
GLuint uColor;
mat4 g_Mat = mat4(1.0f);

float g_time = 0;
float g_aspect = 1;

mat4 myLookAt(vec3 eye, vec3 at, vec3 up)
{
	mat4 V = mat4(1.0f);

	up = normalize(up);
	vec3 n = normalize(at - eye);
	float a = dot(up, n);
	vec3 v = normalize(up - a * n);
	vec3 w = cross(n, v);

	V[0] = vec4(w.x, w.y, w.z, dot(-w, eye));
	V[1] = vec4(v.x, v.y, v.z, dot(-v, eye));
	V[2] = vec4(-n.x, -n.y, -n.z, dot(n, eye));

	return V;
}

mat4 myOrtho(float l, float r, float b, float t, float zNear, float zFar)
{
	vec3 center = vec3((l + r) / 2, (b + t) / 2, -(zNear) / 2);
	mat4 T = Translate(-center);
	mat4 S = Scale(2 / (r - l), 2 / (t - b), 1 / (-zNear + zFar));
	mat4 V = S * T;

	return V;
}

mat4 myPerspective(float angle, float aspect, float zNear, float zFar)
{
	mat4 V(1.0f);
	// Implement your own code for computing the perspective matrix

	float h = zFar * tan(angle / 2.0f * 3.141592f / 180.0f);
	float w = h * aspect;

	mat4 S = Scale(1 / w, 1 / h, 1.0f / zFar);
	mat4 Mpt(1.0f);
	float c = -zNear / zFar;
	Mpt[2] = vec4(0, 0, 1 / (c + 1), -c / (c + 1));
	Mpt[3] = vec4(0, 0, -1, 0);

	V = Mpt * S;
	return V;
}

void wuIdle()
{
	solver.update();
	glutPostRedisplay();
}

void changeSize(int w, int h)
{
	glViewport(0, 0, w, h);
	g_aspect = w / float(h);
	glutPostRedisplay();
}

void wuDrawDensity()
{
	GLfloat positionX;
	GLfloat positionY;
	GLfloat positionZ;

	GLfloat density000;
	GLfloat density010;
	GLfloat density100;
	GLfloat density110;
	GLfloat density001;
	GLfloat density011;
	GLfloat density101;
	GLfloat density111;

	GLfloat h = 1.3f / N;

	glBegin(GL_QUADS);
	for (int x = 0; x < N; x++)
	{
		positionX = (x - 0.5f) * h;

		for (int y = 0; y < N; y++)
		{
			positionY = (y - 0.5f) * h;

			for (int z = 0; z < N; z++)
			{
				positionZ = (z - 0.5f) * h;

				density000 = solver.getDensity(x, y, z);
				density010 = solver.getDensity(x, y + 1, z);
				density100 = solver.getDensity(x + 1, y, z);
				density110 = solver.getDensity(x + 1, y + 1, z);

				density001 = solver.getDensity(x, y, z + 1);
				density011 = solver.getDensity(x, y + 1, z + 1);
				density101 = solver.getDensity(x + 1, y, z + 1);
				density111 = solver.getDensity(x + 1, y + 1, z + 1);

				glColor4f(density111, density111, density111, alpha);
				glVertex3f(positionX + h, positionY + h, positionZ + h);

				glColor4f(density011, density011, density011, alpha);
				glVertex3f(positionX, positionY + h, positionZ + h);

				glColor4f(density001, density001, density001, alpha);
				glVertex3f(positionX, positionY, positionZ + h);

				glColor4f(density101, density101, density101, alpha);
				glVertex3f(positionX + h, positionY, positionZ + h);

				glColor4f(density110, density110, density110, alpha);
				glVertex3f(positionX + h, positionY + h, positionZ);

				glColor4f(density111, density111, density111, alpha);
				glVertex3f(positionX + h, positionY + h, positionZ + h);

				glColor4f(density101, density101, density101, alpha);
				glVertex3f(positionX + h, positionY, positionZ + h);

				glColor4f(density100, density100, density100, alpha);
				glVertex3f(positionX + h, positionY, positionZ);

				glColor4f(density010, density010, density010, alpha);
				glVertex3f(positionX, positionY + h, positionZ);

				glColor4f(density110, density110, density110, alpha);
				glVertex3f(positionX + h, positionY + h, positionZ);

				glColor4f(density100, density100, density100, alpha);
				glVertex3f(positionX + h, positionY, positionZ);

				glColor4f(density000, density000, density000, alpha);
				glVertex3f(positionX, positionY, positionZ);

				glColor4f(density011, density011, density011, alpha);
				glVertex3f(positionX, positionY + h, positionZ + h);

				glColor4f(density010, density010, density010, alpha);
				glVertex3f(positionX, positionY + h, positionZ);

				glColor4f(density000, density000, density000, alpha);
				glVertex3f(positionX, positionY, positionZ);

				glColor4f(density001, density001, density001, alpha);
				glVertex3f(positionX, positionY, positionZ + h);

				glColor4f(density100, density100, density100, alpha);
				glVertex3f(positionX + h, positionY, positionZ);

				glColor4f(density000, density000, density000, alpha);
				glVertex3f(positionX, positionY, positionZ);

				glColor4f(density001, density001, density001, alpha);
				glVertex3f(positionX, positionY, positionZ + h);

				glColor4f(density101, density101, density101, alpha);
				glVertex3f(positionX + h, positionY, positionZ + h);

				glColor4f(density110, density110, density110, alpha);
				glVertex3f(positionX + h, positionY + h, positionZ);

				glColor4f(density010, density010, density010, alpha);
				glVertex3f(positionX, positionY + h, positionZ);

				glColor4f(density011, density011, density011, alpha);
				glVertex3f(positionX, positionY + h, positionZ + h);

				glColor4f(density111, density111, density111, alpha);
				glVertex3f(positionX + h, positionY + h, positionZ + h);
			}
		}
	}
	glEnd();
}
/*
void wuDrawVelocity()
{
	GLfloat positionX;
	GLfloat positionY;
	GLfloat positionZ;

	float* velocityVertices = new float[N * N * N * 2 * 6];
	int index = 0;

	GLfloat h = 1.3f / N;
	glColor3f(1.0, 1.0, 1.0);

	for (int x = 0; x < N; x++)
	{
		positionX = (x - 0.5f) * h;
		for (int y = 0; y < N; y++)
		{
			positionY = (y - 0.5f) * h;
			for (int z = 0; z < N; z++)
			{
				positionZ = (z - 0.5f) * h;
				/*
				glBegin(GL_LINES);
				glVertex3f(positionX, positionY, positionZ);
				glVertex3f(positionX + solver.getVelocityU(x, y, z) / 2,
					positionY + solver.getVelocityV(x, y, z) / 2,
					positionZ + solver.getVelocityW(x, y, z) / 2);
				glEnd();
				
				//
				velocityVertices[index * 6] = positionX;
				velocityVertices[index * 6 + 1] = positionY;
				velocityVertices[index * 6 + 2] = positionZ;
				velocityVertices[index * 6 + 3] = 0.f;
				velocityVertices[index * 6 + 4] = 0.f;
				velocityVertices[index * 6 + 5] = 1.f;
				index += 1;
				velocityVertices[index * 6] = positionX + solver.getVelocityU(x, y, z) / 2;
				velocityVertices[index * 6 + 1] = positionY + solver.getVelocityV(x, y, z) / 2;
				velocityVertices[index * 6 + 2] = positionZ + solver.getVelocityW(x, y, z) / 2;
				velocityVertices[index * 6 + 3] = 0.f;
				velocityVertices[index * 6 + 4] = 0.f;
				velocityVertices[index * 6 + 5] = 1.f;
				index += 1;
			}
		}
	}

	glBufferData(GL_ARRAY_BUFFER, sizeof(velocityVertices), velocityVertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	mat4 ModelMat = myLookAt(vec3(3, 2, 2), vec3(0, 0, 0), vec3(0, 1, 0));
	//mat4 ProjMat = myOrtho(-2, 2,-2, 2, 0.01, 10.0f);
	mat4 ProjMat = myPerspective(60.f, g_aspect, 0.01f, 10.0f);

	g_Mat = ProjMat * ModelMat;
	//mat4 S9 = Scale(0.3f, 0.3f, 0.3f);
	mat4 T9 = Translate(translationX, translationY, translationZ);
	//mat4 L1 = T9 * S9;
	mat4 W1 = RotateX(-10.f);
	mat4 W2 = RotateY(rotationY);

	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat);
	glUniform4f(uColor, 1, 0, 0, 1);

	glDrawArrays(GL_LINES, 0, N * N * N * 2);
	delete velocityVertices;
}

*/

void renderScene(void) {
	/*glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE);

	glEnable(GL_ALPHA_TEST);
	glAlphaFunc(GL_GREATER, 0);*/

	uMat = glGetUniformLocation(p, "uMat");
	uColor = glGetUniformLocation(p, "uColor");

	glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float vertices[] = {
	0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	1.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 1.3f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.3f, 1.0f, 0.0f, 0.0f,
	1.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	1.3f, 1.3f, 0.0f, 1.0f, 0.0f, 0.0f,
	1.3f, 1.3f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 1.3f, 0.0f, 1.0f, 0.0f, 0.0f,
	0.0f, 1.3f, 1.3f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.3f, 1.0f, 0.0f, 0.0f,
	0.0f, 1.3f, 1.3f, 1.0f, 0.0f, 0.0f,
	0.0f, 1.3f, 0.0f, 1.0f, 0.0f, 0.0f,
	1.3f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	1.3f, 0.0f, 1.3f, 1.0f, 0.0f, 0.0f,
	0.0f, 0.0f, 1.3f, 1.0f, 0.0f, 0.0f,
	1.3f, 0.0f, 1.3f, 1.0f, 0.0f, 0.0f,
	1.3f, 1.3f, 0.0f, 1.0f, 0.0f, 0.0f,
	1.3f, 1.3f, 1.3f, 1.0f, 0.0f, 0.0f,
	1.3f, 1.3f, 1.3f, 1.0f, 0.0f, 0.0f,
	1.3f, 0.0f, 1.3f, 1.0f, 0.0f, 0.0f,
	0.0f, 1.3f, 1.3f, 1.0f, 0.0f, 0.0f,
	1.3f, 1.3f, 1.3f, 1.0f, 0.0f, 0.0f
	};

	float velocityVertices[] = {
	  0., 0., 0., 0.0f,1.0f,0.0f,
	  1.0, 1.0, 1.0, 0.0f,0.0f,1.0f
	};
	GLuint abo[2];
	glGenBuffers(2, &abo[0]);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindBuffer(GL_ARRAY_BUFFER, abo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	uMat = glGetUniformLocation(p, "uMat");
	mat4 ModelMat = myLookAt(vec3(2, 1.0f, -1.5), vec3(0, 0, 0), vec3(0, 1, 0));
	mat4 ProjMat = myPerspective(60.f, 1, 1, 10);
	g_Mat = ProjMat * ModelMat * Translate(-0.65f, -0.65f, -0.65f);
	glUniformMatrix4fv(uMat, 1, GL_TRUE, g_Mat);
	
	// Draw Grid
	glDrawArrays(GL_LINES, 0, 24);
	

	// Draw Velocity Line
	glBindBuffer(GL_ARRAY_BUFFER, abo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(velocityVertices), velocityVertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	GLfloat positionX;
	GLfloat positionY;
	GLfloat positionZ;
	mat4 u_Mat;

	GLfloat h = 1.3f / N;

	for (int x = 0; x < N; x++)
	{
		positionX = (x - 0.5f) * h;
		for (int y = 0; y < N; y++)
		{
			positionY = (y - 0.5f) * h;
			for (int z = 0; z < N; z++)
			{
				positionZ = (z - 0.5f) * h;

				float VU = solver.getVelocityU(x, y, z) / 2;
				float VV = solver.getVelocityV(x, y, z) / 2;
				float VW = solver.getVelocityW(x, y, z) / 2;

				u_Mat = g_Mat * Translate(positionX, positionY, positionZ) * Scale(VU, VV, VW);;
				glUniformMatrix4fv(uMat, 1, GL_TRUE, u_Mat);
				glDrawArrays(GL_LINES, 0, 2);
			}
		}
	}

	//g_Mat = ProjMat * ModelMat * Translate(-1.0f, -1.0f, -1.0f);

	/*
	wuDrawVelocity();

	glBindBuffer(GL_ARRAY_BUFFER, abo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(fluidvertice), fluidvertice, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	for (int i = 0; i < N * N * N; i++) {
	   glDrawArrays(GL_LINES, 2 * i, 2);
	}
	*/
	glutSwapBuffers();

}

void wuInitialize()
{
	rotationX = float(30);
	rotationY = float(-40);

	translationX = 0;
	translationY = 0;
	translationZ = float(-1.0);

	N = 32;
	solver.init(N);
}


void setShaders() {

	char* vs = NULL, * fs = NULL;

	v = glCreateShader(GL_VERTEX_SHADER);
	f = glCreateShader(GL_FRAGMENT_SHADER);


	vs = textFileRead("toon.vert");

	fs = textFileRead("toon.frag");

	const char * ff = fs;
	const char * vv = vs;

	glShaderSource(v, 1, &vv,NULL);
	glShaderSource(f, 1, &ff,NULL);

	free(vs);free(fs);

	glCompileShader(v);
	glCompileShader(f);

	p = glCreateProgram();
	glAttachShader(p,f);
	glAttachShader(p,v);

	glLinkProgram(p);
	glUseProgram(p);
}

void wuKeyBoard(unsigned char key, int x, int y)
{
	if (key == 'i' || key == 'I')
	{
		solver.addSource();
	}

	if (key == 'c' || key == 'C')
	{
		solver.reset();
	}
	if (key == 'v' || key == 'V')
	{
		drawVelocity = !drawVelocity;
	}
}

int main(int argc, char **argv) {

	glutInit(&argc, argv);

	wuInitialize();

	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(windowWidth, windowHeight);
	glutCreateWindow("FirstFluid");

	glutDisplayFunc(renderScene);
	glutReshapeFunc(changeSize);
	glutKeyboardFunc(wuKeyBoard);
	glutIdleFunc(wuIdle);

	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);

	glewInit();
	if (glewIsSupported("GL_VERSION_2_0"))
		printf("Ready for OpenGL 2.0\n");
	else {
		printf("OpenGL 2.0 not supported\n");
		exit(1);
	}
	setShaders();

	glutMainLoop();

	// just for compatibiliy purposes
	return 0;
}

