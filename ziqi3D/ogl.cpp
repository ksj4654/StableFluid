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

typedef bool wuBOOL;

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


GLuint v,f,f2,p;

void wuIdle()
{
	solver.update();
	glutPostRedisplay();
}

void changeSize(GLint width, GLint height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0, (float)width / height, 0.001, 100.0);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -3.0f);
}

void wuDrawGrid()
{
	glLineWidth(1.0f);

	glBegin(GL_LINES);
	glColor3f(1.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(1.3f, 0.0f, 0.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 1.3f, 0.0f);

	glVertex3f(0.0f, 0.0f, 0.0f);
	glVertex3f(0.0f, 0.0f, 1.3f);

	glVertex3f(1.3f, 0.0f, 0.0f);
	glVertex3f(1.3f, 1.3f, 0.0f);

	glVertex3f(1.3f, 1.3f, 0.0f);
	glVertex3f(0.0f, 1.3f, 0.0f);

	glVertex3f(0.0f, 1.3f, 1.3f);
	glVertex3f(0.0f, 0.0f, 1.3f);

	glVertex3f(0.0f, 1.3f, 1.3f);
	glVertex3f(0.0f, 1.3f, 0.0f);

	glVertex3f(1.3f, 0.0f, 0.0f);
	glVertex3f(1.3f, 0.0f, 1.3f);

	glVertex3f(0.0f, 0.0f, 1.3f);
	glVertex3f(1.3f, 0.0f, 1.3f);

	glVertex3f(1.3f, 1.3f, 0.0f);
	glVertex3f(1.3f, 1.3f, 1.3f);

	glVertex3f(1.3f, 1.3f, 1.3f);
	glVertex3f(1.3f, 0.0f, 1.3f);

	glVertex3f(0.0f, 1.3f, 1.3f);
	glVertex3f(1.3f, 1.3f, 1.3f);

	glEnd();
}

void wuDrawVelocity()
{
	GLfloat positionX;
	GLfloat positionY;
	GLfloat positionZ;

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
				glBegin(GL_LINES);
				glVertex3f(positionX, positionY, positionZ);
				glVertex3f(positionX + solver.getVelocityU(x, y, z) / 2,
					positionY + solver.getVelocityV(x, y, z) / 2,
					positionZ + solver.getVelocityW(x, y, z) / 2);
				glEnd();
			}
		}
	}

}

void renderScene(void) {

	glMatrixMode(GL_MODELVIEW);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	float vertices[] = {
		// positions         // colors
		 1.5f, 0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
		0.5f, 0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
		 1.0f,  1.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
	};

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	if (!debug)
	{
		printf("%f, %f, %f, %f, %f", translationX, translationY, translationZ, rotationX,rotationY);
		debug += 1;
	}

	glPushMatrix();

	glLoadIdentity();

	glTranslatef(translationX, translationY, translationZ);
	glRotatef(rotationX, 1.0f, 0, 0);
	glRotatef(rotationY, 0, 1.0f, 0);

	wuDrawGrid();

	glDrawArrays(GL_TRIANGLES, 0, 3);

	glPopMatrix();





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

