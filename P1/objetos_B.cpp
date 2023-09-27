//**************************************************************************
// Práctica 1 usando objetos
//**************************************************************************

#include "objetos_B.h"

//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D() {}

//*************************************************************************
// dibujar puntos
//*************************************************************************

void _puntos3D::draw_puntos(float r, float g, float b, int grosor)
{
	int i;
	glPointSize(grosor);
	glColor3f(r, g, b);
	glBegin(GL_POINTS);
	for (i = 0; i < vertices.size(); i++)
	{
		glVertex3fv((GLfloat *)&vertices[i]);
	}
	glEnd();
}

//*************************************************************************
// _triangulos3D
//*************************************************************************

_triangulos3D::_triangulos3D() {}

//*************************************************************************
// generar un color aleatorio
//*************************************************************************

_vertex3f _triangulos3D::color_aleatorio() {
	_vertex3f color;
	for (unsigned short i = 0; i < 3; i++) {
		color[i] = (float)rand() / (float)RAND_MAX;
	}
	return color;
}

//*************************************************************************
// dibujar en modo arista
//*************************************************************************

void _triangulos3D::draw_aristas(float r, float g, float b, int grosor)
{
	int i;
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(grosor);
	glColor3f(r, g, b);
	glBegin(GL_TRIANGLES);
	for (i = 0; i < caras.size(); i++)
	{
		glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
	}
	glEnd();
}

//*************************************************************************
// dibujar en modo sólido con un único color
//*************************************************************************

void _triangulos3D::draw_solido(float r, float g, float b)
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(r, g, b);
	glBegin(GL_TRIANGLES);
	for (unsigned i = 0; i < caras.size(); i++) {
		glVertex3fv((GLfloat*) &vertices[caras[i]._0]);
		glVertex3fv((GLfloat*) &vertices[caras[i]._1]);
		glVertex3fv((GLfloat*) &vertices[caras[i]._2]);
	}
	glEnd();
}

//*************************************************************************
// dibujar en modo sólido con colores diferentes para cada cara
//*************************************************************************

void _triangulos3D::colorear_caras_aleatorio()
{
	// Establecemos la semilla aleatoria
	srand((unsigned)time(NULL));
	for (unsigned int i = 0; i < colores.size(); i++)
	{
		colores[i] = color_aleatorio();
	}
}

void _triangulos3D::draw_solido_colores()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glColor3f(0, 0, 0);
	glBegin(GL_TRIANGLES);
	for (unsigned i = 0; i < caras.size(); i++)
	{
		glColor3fv((GLfloat *) &colores[i]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
		glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
	}
	glEnd();
}

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
	// vertices
	vertices.resize(8);
	vertices[0] = _vertex3f(-1, -1,  1) * tam;
	vertices[1] = _vertex3f( 1, -1,  1) * tam;
	vertices[2] = _vertex3f( 1, -1, -1) * tam;
	vertices[3] = _vertex3f(-1, -1, -1) * tam;
	vertices[4] = _vertex3f(-1,  1,  1) * tam;
	vertices[5] = _vertex3f( 1,  1,  1) * tam;
	vertices[6] = _vertex3f( 1,  1, -1) * tam;
	vertices[7] = _vertex3f(-1,  1, -1) * tam;

	// triangulos
	caras.resize(12);
	caras[0]  = _vertex3i(0, 1, 5);
	caras[1]  = _vertex3i(5, 4, 0);
	caras[2]  = _vertex3i(1, 2, 6);
	caras[3]  = _vertex3i(6, 5, 1);
	caras[4]  = _vertex3i(2, 3, 7);
	caras[5]  = _vertex3i(7, 6, 2);
	caras[6]  = _vertex3i(3, 0, 4);
	caras[7]  = _vertex3i(4, 7, 3);
	caras[8]  = _vertex3i(3, 2, 1);
	caras[9]  = _vertex3i(1, 0, 3);
	caras[10] = _vertex3i(4, 5, 6);
	caras[11] = _vertex3i(6, 7, 4);

	colores.resize(caras.size());
	colorear_caras_aleatorio();
}

//*************************************************************************
// clase piramide
//*************************************************************************

_piramide::_piramide(float tam, float al)
{

	// vertices
	vertices.resize(5);
	vertices[0].x = -tam; vertices[0].y = 0;  vertices[0].z = tam;
	vertices[1].x = tam;  vertices[1].y = 0;  vertices[1].z = tam;
	vertices[2].x = tam;  vertices[2].y = 0;  vertices[2].z = -tam;
	vertices[3].x = -tam; vertices[3].y = 0;  vertices[3].z = -tam;
	vertices[4].x = 0;    vertices[4].y = al; vertices[4].z = 0;

	caras.resize(6);
	caras[0]._0 = 0; caras[0]._1 = 1; caras[0]._2 = 4;
	caras[1]._0 = 1; caras[1]._1 = 2; caras[1]._2 = 4;
	caras[2]._0 = 2; caras[2]._1 = 3; caras[2]._2 = 4;
	caras[3]._0 = 3; caras[3]._1 = 0; caras[3]._2 = 4;
	caras[4]._0 = 3; caras[4]._1 = 1; caras[4]._2 = 0;
	caras[5]._0 = 3; caras[5]._1 = 2; caras[5]._2 = 1;

	colores.resize(caras.size());
	colorear_caras_aleatorio();

}

//*************************************************************************
// clase octaedro
//*************************************************************************

_octaedro::_octaedro(float tam, float al) {

	// vertices
	vertices.resize(6);
	vertices[0] = _vertex3f( 0,   0,  1) * tam;
	vertices[1] = _vertex3f( 1,   0,  0) * tam;
	vertices[2] = _vertex3f( 0,   0, -1) * tam;
	vertices[3] = _vertex3f(-1,   0,  0) * tam;
	vertices[4] = _vertex3f( 0,  al,  0) * tam;
	vertices[5] = _vertex3f( 0, -al,  0) * tam;

	// caras
	caras.resize(8);
	caras[0] = _vertex3i(5, 1, 0);
	caras[1] = _vertex3i(5, 2, 1);
	caras[2] = _vertex3i(5, 3, 2);
	caras[3] = _vertex3i(5, 0, 3);
	caras[4] = _vertex3i(1, 4, 0);
	caras[5] = _vertex3i(2, 4, 1);
	caras[6] = _vertex3i(3, 4, 2);
	caras[7] = _vertex3i(0, 4, 3);

	colores.resize(caras.size());
	colorear_caras_aleatorio();
}