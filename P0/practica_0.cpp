/**
 * Informática Gráfica - Práctica 0
 * Luis Miguel Guirado Bautista
 * Universidad de Granada - 2023
 * 
 * Isotipo usado: Steam
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/freeglut.h>

void Ejes(int width)
{
   if (width <= 0) return;
   glLineWidth(width);
   glBegin(GL_LINES);
   glColor3f(1.0, 0.0, 0.0);
   glVertex3f(-1.0, 0.0, 0.0);
   glVertex3f(1.0, 0.0, 0.0);
   glColor3f(0.0, 1.0, 0.0);
   glVertex3f(0.0, -1.0, 0.0);
   glColor3f(1.0, 1.0, 0.0);
   glVertex3f(0.0, 1.0, 0.0);
   glEnd();
}

// *********************************************************************************

static void Circulo(float radio = 1, float cx = 0, float cy = 0, unsigned short n = 50, float r = 0, float g = 0, float b = 0, GLenum modo = GL_POINT, float width = 3) {
   switch (modo) {
      case GL_LINE:
         glLineWidth(width);
         break;
      case GL_FILL:
         break;
      default:
         glPointSize(width);
         modo = GL_POINT;
         break;
   }
   glPolygonMode(GL_FRONT_AND_BACK, modo);
   glBegin(GL_POLYGON);
      glColor3f(r,g,b);
      for (unsigned short i = 0; i < n; i++) {
         glVertex2f(cx + radio * cos(2*M_PI * i/n), cy + radio * sin(2*M_PI * i/n));
      }
   glEnd();
}

static void Rectangulo(float v1x, float v1y, float v2x, float v2y, float v3x, float v3y, float v4x, float v4y, float r = 0, float g = 0, float b = 0, GLenum modo = GL_POINT, float width = 3) {
   switch (modo) {
      case GL_LINE:
         glLineWidth(width);
         break;
      case GL_FILL:
         break;
      default:
         glPointSize(width);
         modo = GL_POINT;
         break;
   }
   glPolygonMode(GL_FRONT_AND_BACK, modo);
   glPointSize(2);
   glBegin(GL_QUADS);
      glColor3f(r, g, b);
      glVertex2f(v1x, v1y);
      glVertex2f(v2x, v2y);
      glVertex2f(v3x, v3y);
      glVertex2f(v4x, v4y);
   glEnd();
}

static void Isotipo()
{
   // Fondo circular negro
   Circulo(1, 0, 0, 100, 0, 0, 0, GL_FILL);

   // Eje superior
   Circulo(0.35, 0.35, 0.25, 100, 1, 1, 1, GL_FILL);
   Circulo(0.25, 0.35, 0.25, 100, 0, 0, 0, GL_FILL);
   Circulo(0.2, 0.35, 0.25, 100, 1, 1, 1, GL_FILL);

   // Eje inferior
   Circulo(0.25, -0.30, -0.4, 100, 1, 1, 1, GL_FILL);
   Circulo(0.2, -0.30, -0.4, 100, 0, 0, 0, GL_FILL);
   Circulo(0.15, -0.30, -0.4, 100, 1, 1, 1, GL_FILL);

   // Brazo que une los dos ejes
   Rectangulo(0.30, -0.1, 0, 0.3, -0.3, -0.15, -0.05, -0.35, 1, 1, 1, GL_FILL);

   // Brazo que une el eje inferior
   Rectangulo(-0.25, -0.26, -1.1, 0.15, -1.1, -0.2, -0.35, -0.54, 1, 1, 1, GL_FILL);
}

// *********************************************************************************

static void Init()
{
   glShadeModel(GL_FLAT);
}

static void Reshape(int width, int height)
{
   glViewport(0, 0, (GLint)width, (GLint)height);
   glOrtho(-1.0, 1.0, -1.0, 1.0, -10, 10.0);
}

static void Display()
{

   glClearColor(1, 1, 1, 0.0);
   glClear(GL_COLOR_BUFFER_BIT);

   Ejes(0);
   Isotipo();

   glFlush();

}

static void Keyboard(unsigned char key, int x, int y)
{
   if (key == 27)
      exit(0);
}

int main(int argc, char **argv)
{
   
   // Hola

   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_RGB);

   glutInitWindowPosition(235, 80);
   glutInitWindowSize(500, 500);
   glutCreateWindow("Practica 0 IG - Luis Miguel Guirado Bautista");

   Init();

   glutReshapeFunc(Reshape);
   glutDisplayFunc(Display);
   glutKeyboardFunc(Keyboard);

   glutMainLoop();

   return 0;
}
