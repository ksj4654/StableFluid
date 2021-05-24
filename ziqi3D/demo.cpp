//
//  main.c
//  Stable Fluids 3D
//
//  Created by satanwoo on 13-11-10.
//  Copyright (c) 2013年 Ziqi Wu. All rights reserved.
//
#pragma once

#include <stdio.h>
#include "freeglut.h"
#include "solver.h"

typedef bool wuBOOL;

const int windowWidth = 600;
const int windowHeight = 800;

GLfloat translationX;
GLfloat translationY;
GLfloat translationZ;

GLfloat rotationX;
GLfloat rotationY;

int N;
Solver solver;
GLfloat alpha = 0.05;
wuBOOL drawVelocity = false;

void wuInitialize()
{
    rotationX = 30;
    rotationY = -40;
    
    translationX = 0;
    translationY = 0;
    translationZ = -1.0;
    
    N = 32;
    solver.init(N);
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
    
    GLfloat h= 1.3f/N;
    
    glBegin(GL_QUADS);
    for(int x =0; x < N; x++)
    {
        positionX = (x - 0.5f) * h;
        
        for(int y = 0; y < N; y++)
        {
            positionY = (y - 0.5f) * h;
            
            for(int z = 0; z < N; z++)
            {
                positionZ= (z - 0.5f) * h;
                
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
                glVertex3f(positionX+h, positionY, positionZ + h);
                
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

void wuDrawVelocity()
{
    GLfloat positionX;
    GLfloat positionY;
    GLfloat positionZ;
    
    GLfloat h = 1.3f/N;
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
                    glVertex3f(positionX + solver.getVelocityU(x, y, z)/2,
                               positionY + solver.getVelocityV(x, y, z)/2,
                               positionZ + solver.getVelocityW(x, y, z)/2);
                glEnd();
            }
        }
    }
    
}

void wuDisplay()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    
    glEnable(GL_ALPHA_TEST);
    glAlphaFunc(GL_GREATER, 0);
    
    glClear(GL_COLOR_BUFFER_BIT);
    
    glPushMatrix();
    glTranslatef(translationX, translationY, translationZ);
    
    glRotatef(rotationX, 1.0f, 0, 0);
    glRotatef(rotationY, 0, 1.0f, 0);
    
    if (drawVelocity)
        wuDrawVelocity();
    else 
        wuDrawDensity();
    
    wuDrawGrid();
    glPopMatrix();
    glutSwapBuffers();
}

void wuReshape(GLint width, GLint height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    gluPerspective(45.0, (float)width/height, 0.001, 100.0);
    glMatrixMode(GL_MODELVIEW);
    
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -3.0f);
}

void wuSpecialKeys(int key, int x, int y)
{
    glutPostRedisplay();
}

void wuKeyBoard(unsigned char key, int x, int y)
{
    if(key == 'i' || key == 'I')
    {
        solver.addSource();
    }
    
    if(key == 'c' || key == 'C')
    {
        solver.reset();
    }
    
    if(key == 'v' || key == 'V')
    {
        drawVelocity = !drawVelocity;
    }
}

void wuIdle()
{
    solver.update();
    glutPostRedisplay();
}

int main(int argc, char * argv[])
{
    glutInit(&argc, argv);
    
    wuInitialize();
    
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(windowWidth, windowHeight);
    glutCreateWindow("Wu Stable Fluids 3D");
    
    glutDisplayFunc(wuDisplay);
    glutReshapeFunc(wuReshape);
    glutSpecialFunc(wuSpecialKeys);
    glutKeyboardFunc(wuKeyBoard);
    glutIdleFunc(wuIdle);
    
    glutMainLoop();
    return 0;
}

