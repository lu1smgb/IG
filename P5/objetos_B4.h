//**************************************************************************
// Práctica 2
//**************************************************************************

#include <vector>
#include <GL/gl.h>
#include "vertex.h"
#include <stdlib.h>
#include "CImg.h"

const float AXIS_SIZE = 5000;
const float _0 = 1e-5;
typedef enum
{
    POINTS,
    EDGES,
    SOLID,
    SOLID_COLORS,
    DIFUSSE_FLAT,
    DIFUSSE_GOURAUD,
    TEXTURA,
    PICKING
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
    vector<_vertex3f> colores_vertices;
};

//*************************************************************************
// clase triángulo
//*************************************************************************

struct _material {
    GLfloat ambiente[3];
    GLfloat difuso[3];
    GLfloat especular[3];
    float brillo;
    _material();
    _material(float a1, float a2, float a3, float d1, float d2, float d3, float e1, float e2, float e3, float brillo);
};

const _material TEST = _material(0.2, 0.2, 0.2, 0.8, 0.8, 0.8, 1, 1, 1, 50);
const _material MATE_CYAN = _material(0, 0.1, 0.2, 0, 0.4, 0.8, 0, 0, 0, 0);
const _material MATE_NEGRO = _material(0.1, 0.1, 0.1, 0.2, 0.2, 0.2, 0.1, 0.1, 0.1, 1);
const _material SATINADO_CYAN = _material(0, 0.7, 1, 0, 0.7, 1, 0.5, 0.5, 0.5, 10);
const _material SATINADO_NEGRO = _material(0.1, 0.1, 0.1, 0.2, 0.2, 0.2, 0.5, 0.5, 0.5, 10);
const _material BRILLANTE_CYAN = _material(0.3, 0.6, 1, 0.3, 0.6, 1, 1, 1, 1, 50);

struct _textura {
    unsigned int id;
    vector<unsigned char> image;
    _textura();
    _textura(string path);
    ~_textura();
};

class _triangulos3D : public _puntos3D
{
protected:
    unsigned short int random_color_preset;
    _vertex3f color_aleatorio();
    void colorear_caras_aleatorio();
    void colorear_caras();
    void apply_material();
public:
    _triangulos3D();
    void draw_aristas(float r, float g, float b, int grosor);
    void draw_solido(float r, float g, float b);
    void draw_solido_colores();
    void draw(_modo modo, float r, float g, float b, float grosor);

    void calcular_normales_caras();
    void calcular_normales_vertices();

    void draw_difuse_flat(_vertex3f color);
    void draw_difuse_gouraud(_vertex3f color);
    vector<_vertex3i> caras;
    vector<_vertex3f> colores;
    vector<_vertex3f> normales_caras;
    vector<_vertex3f> normales_vertices;

    const _material *material;

    void draw_picking(_vertex3f color);
};

bool iluminacionActivada();
void activarLuces();
void desactivarLuces();

//*************************************************************************
// clase cubo
//*************************************************************************

class _cubo : public _triangulos3D
{
public:
    _textura textura;
    _cubo(float tam = 0.5);
    void draw_textura();
};

//*************************************************************************
// clase piramide
//*************************************************************************

class _piramide : public _triangulos3D
{
private:
    float tam, al;
public:
    _textura textura;
    _piramide(float tam = 0.5, float al = 1.0);
    void draw_textura();
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
        void calcular_normales_vertices_esfera();
};

//************************************************************************
// objeto por extrusión
//************************************************************************

class _extrusion : public _triangulos3D
{
public:
    _extrusion(vector<_vertex3f> poligono, float x, float y, float z);
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
    void draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

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
    void draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

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
    void draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float grosor);

protected:
    _cilindro cilindro;
};

class _fuselaje : public _triangulos3D {
public:
    float anchura;
    float altura;
    float longitud;
    _fuselaje(float anchura = 0.75, float altura = 0.5, float longitud = 5);
    void draw(_modo modo, float r1, float g1, float b1, float r2, float g2, float b2, float r3, float g3, float b3, float grosor);

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
    void picking();
    void reset_model_values();

    _vertex3f color_pick;
    vector<_vertex3i> color_select;
    vector<int> activo;
    int piezas;
    int grosor_select;

protected:
    _fuselaje fuselaje;
    _ala ala;
    _tren_aterrizaje tren_aterrizaje;
    _helice helice;
    _timon timon;
};