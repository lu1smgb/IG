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
    unsigned short int random_color_preset;
    _vertex3f color_aleatorio();
    void colorear_caras_aleatorio();
    void colorear_caras();
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

//! EXAMEN
class _ejercicio1 : public _triangulos3D {
public:
    _ejercicio1(float size = 0.5, float altura = 1);
};

class _ejercicio2 : public _rotacion {
private:
    const float RADIO_CURVA = 0.5;
    const float ANCHURA_TOTAL = 1.25;
    const float ALTURA_TOTAL = 1;
    const unsigned short CALIDAD_CURVAS = 10;
public:
    _ejercicio2();
};

// *********************************************************************************
//                          Practica 3 - Modelo Jerarquico
// UN AVION
// *********************************************************************************

class _aleron : public _triangulos3D
{
public:
    _aleron();
    void draw(_modo modo, float r, float g, float b, float grosor);
protected:
    _cubo cubo;
};

class _ala : public _triangulos3D {
public:
    float longitud; // Longitud en el eje X
    float grosor; // Longitud en el eje Y
    float anchura; // Longitud en el eje Z
    float apertura_aleron;
    _ala(float longitud = 5, float grosor = 0.1, float anchura = 1, float apertura_aleron = 30);
    void draw(_modo modo, float r, float g, float b, float grosor);
protected:
    _cubo cubo;
    _aleron aleron;
};

class _aspa : public _triangulos3D
{
public:
    float longitud; // Longitud en el eje X
    float grosor;   // Longitud en el eje Y
    float anchura;  // Longitud en el eje Z
    float inclinacion; // Inclinacion del aspa
    _aspa(float longitud = 3, float grosor = 0.25, float anchura = 1, float inclinacion = 15);
    void draw(_modo modo, float r, float g, float b, float grosor);
protected:
    _cubo cubo;
};

class _helice : public _triangulos3D {
public:
    float longitud;    // Longitud en el eje X
    float grosor;      // Longitud en el eje Y
    float anchura;     // Longitud en el eje Z
    float inclinacion; // Inclinacion del aspa
    float margen;
    float longitud_brazo;
    unsigned short int num_helices;
    _helice(float longitud = 5, float grosor = 0.25, float anchura = 1, float inclinacion = 45, 
            float margen = 1, float num_helices = 4, float longitud_brazo = 2);
    void draw(_modo modo, float r, float g, float b, float grosor);
protected:
    _aspa aspa;
    _cilindro eje;
};

class _tren_aterrizaje : public _triangulos3D {
public:
    float radio_rueda;
    float longitud_brazo;
    // Aniadir inclinacion sobre eje X
    _tren_aterrizaje(float radio_rueda = 0.5, float longitud_brazo = 2);
    void draw(_modo modo, float r, float g, float b, float grosor);
protected:
    _cilindro cilindro;
};

class _fuselaje : public _triangulos3D {
public:
    float anchura;
    float altura;
    float longitud;
    _fuselaje(float anchura = 0.75, float altura = 0.5, float longitud = 5);
    void draw(_modo modo, float r, float g, float b, float grosor);
protected:
    _cubo cubo;
    _aleron retrovisor;
};

class _timon : public _triangulos3D
{
public:
    float grosor;
    float altura;
    float longitud;
    _timon(float grosor = 0.1, float altura = 1, float longitud = 0.5);
    void draw(_modo modo, float r, float g, float b, float grosor);
protected:
    _cubo cubo;
};

class _avion : public _triangulos3D {
public:
    float inclinacion_horizontal; // Rotacion sobre el eje Z
    float inclinacion_vertical; // Rotacion sobre el eje X
    float angulo_direccion; // Rotacion sobre el eje Y

    // Variables para el manejo de las articulaciones
    float apertura_alerones;
    float rotacion_helice;
    float apertura_tren;
    float direccion_timon;

    _vertex3f posicion;

    _avion(float inclinacion_horizontal = 0, float inclinacion_vertical = 0, float angulo_direccion = 0);
    void draw(_modo modo, float r, float g, float b, float grosor);
    void reset_model_values();
protected:
    _fuselaje fuselaje;
    _ala ala;
    _tren_aterrizaje tren_aterrizaje;
    _helice helice;
    _timon timon;
};