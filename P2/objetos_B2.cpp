//**************************************************************************
// Práctica 2
//**************************************************************************

#include "objetos_B2.h"
#include "file_ply_stl.h"

//*************************************************************************
// _puntos3D
//*************************************************************************

_puntos3D::_puntos3D()
{
}

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

_triangulos3D::_triangulos3D()
{
}

//*************************************************************************
// generar un color aleatorio
//*************************************************************************

_vertex3f _triangulos3D::color_aleatorio()
{
    _vertex3f color;
    for (unsigned short i = 0; i < 3; i++)
    {
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
    for (unsigned i = 0; i < caras.size(); i++)
    {
        glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
        glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
        glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
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
        glColor3fv((GLfloat *)&colores[i]);
        glVertex3fv((GLfloat *)&vertices[caras[i]._0]);
        glVertex3fv((GLfloat *)&vertices[caras[i]._1]);
        glVertex3fv((GLfloat *)&vertices[caras[i]._2]);
    }
    glEnd();
}

//*************************************************************************
// dibujar con distintos modos
//*************************************************************************

void _triangulos3D::draw(_modo modo, float r, float g, float b, float grosor)
{
    switch (modo)
    {
    case POINTS:
        draw_puntos(r, g, b, grosor);
        break;
    case EDGES:
        draw_aristas(r, g, b, grosor);
        break;
    case SOLID:
        draw_solido(r, g, b);
        break;
    case SOLID_COLORS:
        draw_solido_colores();
        break;
    }
}

//*************************************************************************
// clase cubo
//*************************************************************************

_cubo::_cubo(float tam)
{
    // vertices
    vertices.resize(8);
    vertices[0] = _vertex3f(-1, -1, 1) * tam;
    vertices[1] = _vertex3f(1, -1, 1) * tam;
    vertices[2] = _vertex3f(1, -1, -1) * tam;
    vertices[3] = _vertex3f(-1, -1, -1) * tam;
    vertices[4] = _vertex3f(-1, 1, 1) * tam;
    vertices[5] = _vertex3f(1, 1, 1) * tam;
    vertices[6] = _vertex3f(1, 1, -1) * tam;
    vertices[7] = _vertex3f(-1, 1, -1) * tam;

    // triangulos
    caras.resize(12);
    caras[0] = _vertex3i(0, 1, 5);
    caras[1] = _vertex3i(5, 4, 0);
    caras[2] = _vertex3i(1, 2, 6);
    caras[3] = _vertex3i(6, 5, 1);
    caras[4] = _vertex3i(2, 3, 7);
    caras[5] = _vertex3i(7, 6, 2);
    caras[6] = _vertex3i(3, 0, 4);
    caras[7] = _vertex3i(4, 7, 3);
    caras[8] = _vertex3i(3, 2, 1);
    caras[9] = _vertex3i(1, 0, 3);
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

_octaedro::_octaedro(float tam, float al)
{

    // vertices
    vertices.resize(6);
    vertices[0] = _vertex3f(0, 0, 1) * tam;
    vertices[1] = _vertex3f(1, 0, 0) * tam;
    vertices[2] = _vertex3f(0, 0, -1) * tam;
    vertices[3] = _vertex3f(-1, 0, 0) * tam;
    vertices[4] = _vertex3f(0, al, 0) * tam;
    vertices[5] = _vertex3f(0, -al, 0) * tam;

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

//*************************************************************************
// clase objeto ply
//*************************************************************************

_objeto_ply::_objeto_ply()
{
    // leer lista de coordenadas de vértices y lista de indices de vértices
}

void _objeto_ply::parametros(char *archivo)
{
    int n_ver, n_car;

    vector<float> ver_ply;
    vector<int> car_ply;

    _file_ply::read(archivo, ver_ply, car_ply);

    n_ver = ver_ply.size() / 3;
    n_car = car_ply.size() / 3;

    printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

    vertices.resize(n_ver);
    caras.resize(n_car);
}

//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacion::_rotacion()
{
}

void _rotacion::parametros(vector<_vertex3f> perfil, int num)
{
    int i, j;
    _vertex3f vertice_aux;
    _vertex3i cara_aux;
    int num_aux;

    // tratamiento de los vértice

    num_aux = perfil.size();
    vertices.resize(num_aux * num);
    for (j = 0; j < num; j++)
    {
        for (i = 0; i < num_aux; i++)
        {
            vertice_aux.x = perfil[i].x * cos(2.0 * M_PI * j / (1.0 * num)) +
                            perfil[i].z * sin(2.0 * M_PI * j / (1.0 * num));
            vertice_aux.z = -perfil[i].x * sin(2.0 * M_PI * j / (1.0 * num)) +
                            perfil[i].z * cos(2.0 * M_PI * j / (1.0 * num));
            vertice_aux.y = perfil[i].y;
            vertices[i + j * num_aux] = vertice_aux;
        }
    }

    // tratamiento de las caras

    // tapa inferior

    // tapa superior
}

//************************************************************************
// objeto por extrusión
//************************************************************************

_extrusion::_extrusion(vector<_vertex3f> poligono, float x, float y, float z)
{
    int i;
    _vertex3f vertice_aux;
    _vertex3i cara_aux;
    int num_aux;

    // tratamiento de los vértice

    num_aux = poligono.size();
    vertices.resize(num_aux * 2);
    for (i = 0; i < num_aux; i++)
    {
        vertices[2 * i] = poligono[i];
        vertices[2 * i + 1].x = poligono[i].x + x;
        vertices[2 * i + 1].y = poligono[i].y + y;
        vertices[2 * i + 1].z = poligono[i].z + z;
    }

    // tratamiento de las caras

    caras.resize(num_aux * 2);
    int c = 0;
    for (i = 0; i < num_aux; i++)
    {
        caras[c]._0 = i * 2;
        caras[c]._1 = (i * 2 + 2) % (num_aux * 2);
        caras[c]._2 = i * 2 + 1;
        c = c + 1;
        caras[c]._0 = (i * 2 + 2) % (num_aux * 2);
        caras[c]._1 = (i * 2 + 2) % (num_aux * 2) + 1;
        caras[c]._2 = i * 2 + 1;
        c = c + 1;
    }
}
