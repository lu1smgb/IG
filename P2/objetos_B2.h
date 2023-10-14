//**************************************************************************
// Práctica 2
//**************************************************************************

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>

const float AXIS_SIZE = 5000;
const float _0 = 1e-5;
typedef enum
{
    POINTS,
    EDGES,
    SOLID,
    SOLID_COLORS
} _modo;

//*************************************************************************
// clase punto
//*************************************************************************

class _puntos3D
{
public:
    _puntos3D();
    void draw_puntos(float r, float g, float b, int grosor);

    vector<_vertex3f> vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

class _triangulos3D : public _puntos3D
{
protected:
    _vertex3f color_aleatorio();
    void colorear_caras_aleatorio();
    void colorear_caras(unsigned short int preset = 0);
public:
    _triangulos3D();
    void draw_aristas(float r, float g, float b, int grosor);
    void draw_solido(float r, float g, float b);
    void draw_solido_colores();
    void draw(_modo modo, float r, float g, float b, float grosor);

    vector<_vertex3i> caras;
    vector<_vertex3f> colores;
};

//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo : public _triangulos3D
{
public:
    _cubo(float tam = 0.5);
};

//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide : public _triangulos3D
{
public:
    _piramide(float tam = 0.5, float al = 1.0);
};

//*************************************************************************
// clase octaedro
//*************************************************************************

class _octaedro : public _triangulos3D
{
public:
    _octaedro(float tam = 0.5, float al = 1.0);
};

//*************************************************************************
// clase objeto ply
//*************************************************************************

class _objeto_ply : public _triangulos3D
{
public:
    _objeto_ply();

    void parametros(char *archivo);
};

//************************************************************************
// objeto por revolución
//************************************************************************

class _rotacion : public _triangulos3D
{
private:
    void ordenar_perfil(vector<_vertex3f> &perfil);
public:
    _rotacion();

    void parametros(vector<_vertex3f> perfil, unsigned num = 10, bool tapa_inferior = true, bool tapa_superior = true);
};

class _rotacion_ply : public _rotacion
{
public:
    _rotacion_ply();

    void parametros(char* archivo, unsigned num = 10, bool tapa_inferior = true, bool tapa_superior = true);  
};

class _cono : public _rotacion {
    public:
        _cono(float radio = 0.5, float al = 1, unsigned num = 10, bool tapa_inferior = true, bool tapa_superior = true);
};

class _cilindro : public _rotacion {
    public:
        _cilindro(float radio = 0.5, float al = 1, unsigned num = 10, bool tapa_inferior = true, bool tapa_superior = true);
};

class _esfera : public _rotacion {
    public:
        _esfera(float radio = 0.5, unsigned num_x = 10, unsigned num_y = 10, bool tapa_inferior = true, bool tapa_superior = true);
};

//************************************************************************
// objeto por extrusión
//************************************************************************

class _extrusion : public _triangulos3D
{
public:
    _extrusion(vector<_vertex3f> poligono, float x, float y, float z);
};
