//**************************************************************************
// Práctica 2
//**************************************************************************

#include "objetos_B3.h"
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
    glPointSize(grosor);
    glColor3f(r, g, b);
    glBegin(GL_POINTS);
    for (unsigned i = 0; i < vertices.size(); i++)
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
    random_color_preset = 2;
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
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glLineWidth(grosor);
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

void _triangulos3D::colorear_caras() {
    colores.resize(caras.size());
    srand((unsigned)time(NULL));
    switch (this->random_color_preset) {
        case 2:
            // Gamma de colores grises
            for (unsigned int i = 0; i < colores.size(); i++)
            {
                float val = (3.0 + (rand() % 3)) / 10.0;
                colores[i] = _vertex3f(val, val, val);
            }
            break;
        case 1:
            // Gamma de colores azules
            // R: 0 - 0.3
            // G: 0.3 - 0.6
            // B: 1
            for (unsigned int i = 0; i < colores.size(); i++)
            {
                colores[i] = _vertex3f(
                    (float)(0 + (rand() % 30)) / (float)100,
                    (float)(30 + (rand() % 30)) / (float)100,
                    1);
            }
            break;
        case 0:
        default:
            // Aleatorio por defecto
            for (unsigned int i = 0; i < colores.size(); i++) {
                colores[i] = color_aleatorio();
            }
            break;
    }
}

void _triangulos3D::colorear_caras_aleatorio()
{
    random_color_preset = 0;
    colorear_caras();
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

    colorear_caras();
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

    colorear_caras();
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

    colorear_caras();
}

//*************************************************************************
// clase objeto ply
//*************************************************************************

_objeto_ply::_objeto_ply() {}

void _objeto_ply::parametros(char *archivo) {
    unsigned n_ver, n_car;

    vector<float> ver_ply;
    vector<int> car_ply;

    _file_ply::read(archivo, ver_ply, car_ply);

    n_ver = ver_ply.size() / 3;
    n_car = car_ply.size() / 3;

    // printf("Number of vertices=%d\nNumber of faces=%d\n", n_ver, n_car);

    vertices.resize(n_ver);
    for (unsigned i = 0; i < n_ver; i++) {
        vertices[i] = _vertex3f(ver_ply[i*3], ver_ply[i*3+1], ver_ply[i*3+2]);
    }

    caras.resize(n_car);
    for (unsigned i = 0; i < n_car; i++) {
        caras[i] = _vertex3i(car_ply[i*3], car_ply[i*3+1], car_ply[i*3+2]);
    }

    colorear_caras();
}

//************************************************************************
// objeto por revolucion
//************************************************************************

_rotacion::_rotacion() {};

// Intercambia dos tuplas de tamaño 3 de cualquier tipo de dato
template <class T>
void swap_vertex(_vertex3<T> &a, _vertex3<T> &b) {
    _vertex3<T> aux = a;
    a = b;
    b = aux;
}

/**
 * Ordena un conjunto de vértices según su coordenada Y en orden descendente
 * Utiliza el algoritmo de ordenación por burbuja
*/
void _rotacion::ordenar_perfil(vector<_vertex3f> &perfil) {

    for (unsigned i = 0; i < perfil.size() - 1; i++) {
        for (unsigned j = 0; j < perfil.size() - i - 1; j++) {
            _vertex3f &a = perfil[j];
            _vertex3f &b = perfil[j+1];
            if (a.y < b.y) {
                swap_vertex(a, b);
            }
        }
    }

}
// Genera un poligono por revolucion a partir del perfil y de un numero positivo de revoluciones
void _rotacion::parametros(vector<_vertex3f> perfil, unsigned num, bool tapa_inferior, bool tapa_superior) {

    // Ordenamos el perfil proporcionado según su coordenada Y en orden descendente
    ordenar_perfil(perfil);
    
    // ***** Vertices *****
    // Num de vertices = Num de vertices del perfil * Num de revoluciones
    unsigned tam_perfil = perfil.size();
    unsigned num_vertices = tam_perfil * num;
    vertices.resize(num_vertices);

    // Generamos los vertices
    _vertex3f vertice_aux;
    for (unsigned j = 0; j < num; j++)
    {
        for (unsigned i = 0; i < tam_perfil; i++)
        {
            vertice_aux.x = perfil[i].x * cos(2.0 * M_PI * j / (1.0 * num)) +
                            perfil[i].z * sin(2.0 * M_PI * j / (1.0 * num));
            vertice_aux.z = -perfil[i].x * sin(2.0 * M_PI * j / (1.0 * num)) +
                            perfil[i].z * cos(2.0 * M_PI * j / (1.0 * num));
            vertice_aux.y = perfil[i].y;
            vertices[i + j * tam_perfil] = vertice_aux;
        }
    }

    // ***** Caras longitudinales *****
    // Generación de las caras longitudinales
    // Cada iteración de este bucle genera todas las caras de una sección del polígono
    for (unsigned i = 0; i < num; i++) {
        // Cada iteracion de este bucle genera las caras en una sección del perfil
        for (unsigned j = 0; j < tam_perfil - 1; j++) {
            /**
             * Cara de indice par (cara superior):
             * x = superior der
             * y = superior izq % V
             * z = inferior izq % V
            */
            caras.push_back(_vertex3i(
                tam_perfil * i + j, 
               (tam_perfil * (i + 1) + j)       % num_vertices, 
               (tam_perfil * (i + 1) + (j + 1)) % num_vertices));
            /**
             * Cara de indice impar (inferior):
             * x = inferior izq % V = z(superior)
             * y = inferior der
             * z = superior der = x(superior)
            */
            caras.push_back(_vertex3i(
               (tam_perfil * (i + 1) + (j + 1)) % num_vertices,
                tam_perfil * i + j,
                tam_perfil * i + j + 1));
        }
    }

    // ***** Tapas *****
    // Generamos los vertices que necesitaremos para su generacion y los
    // incluimos con el resto de vertices
    // En caso de que ya esten incluidos en el perfil (X aprox. 0), los anadimos directamente
    // en caso contrario, los generamos a partir del primero y el ultimo modificando la coordenada X
    if (tapa_superior || tapa_inferior) {

        // Generacion de vertices
        if (tapa_superior) {
            _vertex3f v_superior;
            _vertex3f &primero = perfil[0];
            if (abs(primero.x) < _0)
                v_superior = primero;
            else {
                v_superior = _vertex3f(0, primero.y, 0);
            }
            vertices.push_back(v_superior);
        }

        if (tapa_inferior) {
            _vertex3f v_inferior;
            _vertex3f &ultimo = perfil[tam_perfil - 1];
            if (abs(ultimo.x) < _0)
                v_inferior = ultimo;
            else {
                v_inferior = _vertex3f(0, ultimo.y, 0);
            }
            vertices.push_back(v_inferior);
        }

        // Generacion de caras
        for (unsigned i = 0; i < num; i++) {

            // La variable num_vertices no varia, pero num_vertices != vertices.size()
            /**
             * Cara superior (vista desde arriba):
             * x = v_superior
             * y = superior der
             * z = superior izq (0)
             */
            if (tapa_superior) {
                _vertex3i cara_superior(
                    num_vertices,
                    (tam_perfil * (i + 1)) % num_vertices,
                    tam_perfil * i);
                caras.push_back(cara_superior);
            }

            /**
             * Cara inferior (vista desde abajo):
             * x = v_inferior
             * y = inferior der
             * z = inferior izq (0)
             */
            if (tapa_inferior) {
                _vertex3i cara_inferior(
                    // Si tambien dibujo la otra cara, entonces apuntamos al punto correspondiente
                    tapa_superior ? num_vertices + 1 : num_vertices,
                    ((tam_perfil - 1) + tam_perfil * (i + 1)) % num_vertices,
                    ((tam_perfil - 1) + tam_perfil * i) % num_vertices);
                caras.push_back(cara_inferior);
            }
        }
    }

    // Finalmente asignamos un color a cada cara
    colorear_caras();

}

_rotacion_ply::_rotacion_ply() {};

void _rotacion_ply::parametros(char* archivo, unsigned num, bool tapa_inferior, bool tapa_superior) {

    vector<_vertex3f> perfil;
    vector<float> perfil_plano;
    vector<int> _c;

    _file_ply::read(archivo, perfil_plano, _c);
    _c.clear();

    unsigned p_size = perfil_plano.size() / 3;

    // cout << "Número de vértices del perfil " << archivo << ": " << p_size << endl;

    perfil.resize(p_size);
    for (unsigned i = 0; i < perfil.size(); i++) {
        perfil[i] = _vertex3f(
            perfil_plano[3*i], perfil_plano[3*i+1], perfil_plano[3*i+2]
        );
    }

    // Llamamos a la clase padre
    _rotacion::parametros(perfil, num, tapa_inferior, tapa_superior);
}

_cono::_cono(float radio, float al, unsigned num, bool tapa_inferior, bool tapa_superior)
{

    if (num < 3) {
        num = 10;
        cout << "[!] Numero de vertices invalido, se ha reestablecido a " << num << endl;
    }

    if (radio < 0) {
        radio = 1;
        cout << "[!] Radio no valido, se ha reestablecido a " << radio << endl;
    }

    if (al < 0) {
        al = 1;
        cout << "[!] Altura no valida, se ha reestablecido a " << al << endl;
    }

    vector<_vertex3f> profile;
    profile.resize(3);
    profile[0] = _vertex3f(0,al/2,0);
    profile[1] = _vertex3f(radio, -al/2, 0);
    profile[2] = _vertex3f(0,-al/2,0);
    parametros(profile, num, tapa_inferior, tapa_superior);
}

_cilindro::_cilindro(float radio, float al, unsigned num, bool tapa_inferior, bool tapa_superior) {

    if (num < 3) {
        num = 10;
        cout << "[!] Numero de vertices invalido, se ha reestablecido a " << num << endl;
    }

    if (radio < 0) {
        radio = 1;
        cout << "[!] Radio no valido, se ha reestablecido a " << radio << endl;
    }

    if (al < 0) {
        al = 1;
        cout << "[!] Altura no valida, se ha reestablecido a " << al << endl;
    }

    // Generamos el perfil para el cilindro
    vector<_vertex3f> perfil;
    perfil.resize(2);
    perfil[0] = _vertex3f(radio, al/2, 0);
    perfil[1] = _vertex3f(radio, -al/2, 0);
    parametros(perfil, num, tapa_inferior, tapa_superior);
}

_esfera::_esfera(float radio, unsigned num_x, unsigned num_y, bool tapa_inferior, bool tapa_superior) {

    if (num_x < 3) {
        num_x = 10;
        cout << "[!] Numero de vertices en horizontal invalido, se ha reestablecido a " << num_x << endl;
    }

    if (num_y < 3) {
        num_y = 10;
        cout << "[!] Numero de vertices en horizontal invalido, se ha reestablecido a " << num_y << endl;
    }

    if (radio < 0) {
        radio = 1;
        cout << "[!] Radio no valido, se ha reestablecido a 1" << endl;
    }

    // Generamos el perfil para la esfera
    vector<_vertex3f> perfil;
    for (unsigned i=0; i < num_y; i++) {
        float angulo = (float)i / (float)(num_y - 1);
        float x = radio * sin(M_PI * angulo);
        float y = radio * cos(M_PI * angulo);
        perfil.push_back(_vertex3f(x, y, 0));
    }
    parametros(perfil, num_x, tapa_inferior, tapa_superior);
};

//************************************************************************
// objeto por extrusión
//************************************************************************

_extrusion::_extrusion(vector<_vertex3f> poligono, float x, float y, float z)
{
    int i;
    _vertex3f vertice_aux;
    _vertex3i cara_aux;
    int tam_poligono;

    // tratamiento de los vértice

    tam_poligono = poligono.size();
    vertices.resize(tam_poligono * 2);
    for (i = 0; i < tam_poligono; i++)
    {
        vertices[2 * i] = poligono[i];
        vertices[2 * i + 1].x = poligono[i].x + x;
        vertices[2 * i + 1].y = poligono[i].y + y;
        vertices[2 * i + 1].z = poligono[i].z + z;
    }

    // tratamiento de las caras

    caras.resize(tam_poligono * 2);
    int c = 0;
    for (i = 0; i < tam_poligono; i++)
    {
        caras[c]._0 = i * 2;
        caras[c]._1 = (i * 2 + 2) % (tam_poligono * 2);
        caras[c]._2 = i * 2 + 1;
        c = c + 1;
        caras[c]._0 = (i * 2 + 2) % (tam_poligono * 2);
        caras[c]._1 = (i * 2 + 2) % (tam_poligono * 2) + 1;
        caras[c]._2 = i * 2 + 1;
        c = c + 1;
    }

    colorear_caras_aleatorio();
}

// *********************************************************************************
//                          Practica 3 - Modelo Jerarquico
// UN AVION
// *********************************************************************************

_ala::_ala(float longitud, float grosor, float anchura, float apertura_aleron)
{
    this->longitud = longitud;
    this->grosor = grosor;
    this->anchura = anchura;
    this->apertura_aleron = abs(apertura_aleron);
    this->cubo = _cubo();
    this->aleron = _aleron();
}

void _ala::draw(_modo modo, float r, float g, float b, float grosor)
{
    const float PROPORCION_DELANTERA = 0.8;
    const float PROPORCION_INFERIOR = 0.6;
    const float PROPORCION_TRASERA = 1 - PROPORCION_DELANTERA;
    const float PROPORCION_SUPERIOR = 1 - PROPORCION_INFERIOR;
    glPushMatrix();
    glTranslatef(0, 0, this->anchura * PROPORCION_DELANTERA / 2);
    glScalef(this->longitud, this->grosor, this->anchura * PROPORCION_DELANTERA);
    cubo.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, 0, -this->anchura * PROPORCION_TRASERA / 2);
    glTranslatef(0, -this->grosor * PROPORCION_SUPERIOR / 2, 0);
    glScalef(this->longitud, this->grosor * PROPORCION_INFERIOR, this->anchura * PROPORCION_TRASERA);
    cubo.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();
    glRotatef(apertura_aleron, 1, 0, 0);
    glScalef(this->longitud, this->grosor * PROPORCION_SUPERIOR, this->anchura * PROPORCION_TRASERA);
    aleron.draw(modo, r, g, b, grosor);
    glPopMatrix();
}

_aleron::_aleron() {

}

void _aleron::draw(_modo modo, float r, float g, float b, float grosor)
{
    glPushMatrix();
    glTranslatef(0, 0.5, -0.5);
    cubo.draw(modo, r, g, b, grosor);
    glPopMatrix();
}

_aspa::_aspa(float longitud, float grosor, float anchura, float inclinacion) {
    this->longitud = longitud;
    this->grosor = grosor;
    this->anchura = anchura;
    this->inclinacion = inclinacion;
    this->cubo = _cubo();
}

void _aspa::draw(_modo modo, float r, float g, float b, float grosor)
{
    glPushMatrix();
    glRotatef(this->inclinacion, 0, 1, 0);
    glTranslatef(0, this->longitud / 2, 0);
    glScalef(this->anchura, this->longitud, this->grosor);
    cubo.draw(modo, r, g, b, grosor);
    glPopMatrix();
}

_helice::_helice(float longitud, float grosor, float anchura, float inclinacion, float margen, float num_helices, float longitud_brazo) {
    this->longitud = longitud;
    this->grosor = grosor;
    this->anchura = anchura;
    this->inclinacion = abs(inclinacion);
    this->margen = margen;
    this->num_helices = num_helices;
    this->longitud_brazo = longitud_brazo;
    this->eje = _cilindro(0.5, 1, 6);
    this->aspa = _aspa(this->longitud, this->grosor, this->anchura, this->inclinacion);
}

void _helice::draw(_modo modo, float r, float g, float b, float grosor)
{
    const float RAD_INCLINACION = (this->inclinacion / 360) * (2*M_PI);
    const float DIM_XZ = this->margen + this->anchura;
    const float DIM_Y = this->margen + (this->grosor * cos(RAD_INCLINACION)) + (this->anchura * sin(RAD_INCLINACION));
    
    glPushMatrix();
    glTranslatef(0, 0, longitud_brazo / 2);
    glRotatef(90, 1, 0, 0);
    glScalef(DIM_XZ / 2, longitud_brazo, DIM_XZ / 2);
    eje.draw(modo, r, g, b, grosor);
    glPopMatrix();
    
    glPushMatrix();
    glTranslatef(0, 0, longitud_brazo);
        glPushMatrix();
        glRotatef(90, 1, 0, 0);
        glScalef(DIM_XZ, DIM_Y, DIM_XZ);
        eje.draw(modo, r, g, b, grosor);
        glPopMatrix();

        glPushMatrix();
        for (unsigned short int i = 0; i < num_helices; i++) {
            glRotatef(360 / num_helices, 0, 0, 1);
            aspa.draw(modo, r, g, b, grosor);
        }
        glPopMatrix();
    glPopMatrix();
}

_tren_aterrizaje::_tren_aterrizaje(float diametro_rueda, float longitud_brazo) {
    this->diametro_rueda = diametro_rueda;
    this->longitud_brazo = longitud_brazo;
    this->cilindro = _cilindro();
}

void _tren_aterrizaje::draw(_modo modo, float r, float g, float b, float grosor) {

    glPushMatrix();
    glTranslatef(0, -this->longitud_brazo / 2, 0);
    glScalef(0.25, this->longitud_brazo, 0.25);
    cilindro.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, -this->longitud_brazo, 0);
    glRotatef(90, 0, 0, 1);
    glScalef(diametro_rueda, 0.5, diametro_rueda);
    cilindro.draw(modo, r, g, b, grosor);
    glPopMatrix();

}

_fuselaje::_fuselaje(float anchura, float altura, float longitud) {
    this->anchura = anchura;
    this->altura = altura;
    this->longitud = longitud;
    this->cubo = _cubo();
    this->retrovisor = _aleron();
}

void _fuselaje::draw(_modo modo, float r, float g, float b, float grosor) {

    glPushMatrix();
    glScalef(this->anchura, this->altura / 2, this->longitud);
    cubo.draw(modo, r, g, b, grosor);
    glPopMatrix();

    const float LONGITUD_RETROVISOR = this->longitud * 0.2;
    const int INCLINACION_RETROVISOR = 20;
    const float RAD_INCL = INCLINACION_RETROVISOR / 360.0 * 2 * M_PI;
    const float ALTURA_CABINA = 0.05 + LONGITUD_RETROVISOR * sin(RAD_INCL);
    glPushMatrix();
    glTranslatef(0, this->altura / 4, this->longitud / 2);
    glRotatef(INCLINACION_RETROVISOR, 1, 0, 0);
    glScalef(this->anchura, 0.05, LONGITUD_RETROVISOR);
    retrovisor.draw(modo, r, g, b, grosor);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0, this->altura / 4, this->longitud / 4 - LONGITUD_RETROVISOR * cos(RAD_INCL));
    glTranslatef(0, ALTURA_CABINA / 2, 0);
    glScalef(this->anchura, ALTURA_CABINA, this->longitud / 2);
    cubo.draw(modo, r, g, b, grosor);
    glPopMatrix();
}

_timon::_timon(float grosor, float altura, float longitud) {
    this->grosor = grosor;
    this->altura = altura;
    this->longitud = longitud;
    this->cubo = _cubo();
}

void _timon::draw(_modo modo, float r, float g, float b, float grosor) {

    glPushMatrix();
    glTranslatef(0, this->altura / 2, -this->longitud / 2);
    glScalef(this->grosor, this->altura, this->longitud);
    cubo.draw(modo, r, g, b, grosor);
    glPopMatrix();

}

_avion::_avion(float inclinacion_horizontal, float inclinacion_vertical, float angulo_direccion)
{
    this->inclinacion_horizontal = inclinacion_horizontal;
    this->inclinacion_vertical = inclinacion_vertical;
    this->angulo_direccion = angulo_direccion;
    this->fuselaje = _fuselaje();
    this->ala = _ala();
    this->tren_aterrizaje = _tren_aterrizaje();
    this->helice = _helice();
    this->timon = _timon();
}

void _avion::draw(_modo modo, float r, float g, float b, float grosor)
{
    const float ESCALA_HELICE = 0.08;
    const float ESCALA_TREN_AT = 0.2;

    glPushMatrix();
    glRotatef(inclinacion_horizontal, 0, 0, 1);
    glRotatef(angulo_direccion, 0, 1, 0);
    glRotatef(inclinacion_vertical, 1, 0, 0);

        // Fuselaje
        fuselaje.draw(modo, r, g, b, grosor);

        // Ala izquierda
        glPushMatrix();
        glTranslatef(-fuselaje.anchura / 2, 0, ala.anchura / 2);
        glRotatef(-5, 0, 0, 1);
        glTranslatef(-ala.longitud / 2, 0, 0);
        ala.draw(modo, r, g, b, grosor);
        glPopMatrix();

        // Ala derecha
        glPushMatrix();
        glTranslatef(fuselaje.anchura / 2, 0, ala.anchura / 2);
        glRotatef(5, 0, 0, 1);
        glTranslatef(ala.longitud / 2, 0, 0);
        ala.draw(modo, r, g, b, grosor);
        glPopMatrix();

        // Estabilizador izquierdo
        glPushMatrix();
        glTranslatef(-fuselaje.anchura / 2 - ala.longitud / 4, 0, ala.anchura / 4 - fuselaje.longitud / 2);
        glScalef(0.5, 1, 1);
        ala.draw(modo, r, g, b, grosor);
        glPopMatrix();

        // Estabilizador derecho
        glPushMatrix();
        glTranslatef(fuselaje.anchura / 2 + ala.longitud / 4, 0, ala.anchura / 4 - fuselaje.longitud / 2);
        glScalef(0.5, 1, 1);
        ala.draw(modo, r, g, b, grosor);
        glPopMatrix();

        // Timon
        glPushMatrix();
        glTranslatef(0, 0, timon.longitud - fuselaje.longitud / 2);
        // glRotatef(10, 0, 1, 0);
        timon.draw(modo, r, g, b, grosor);
        glPopMatrix();

        // Helice
        glPushMatrix();
        glTranslatef(0, 0, fuselaje.longitud / 2);
        glScalef(ESCALA_HELICE, ESCALA_HELICE, ESCALA_HELICE);
        helice.draw(modo, r, g, b, grosor);
        glPopMatrix();

        // Tren de aterrizaje frontal
        glPushMatrix();
        glTranslatef(0, -fuselaje.altura / 4, (fuselaje.longitud / 2) * 0.95);
        glScalef(ESCALA_TREN_AT, ESCALA_TREN_AT, ESCALA_TREN_AT);
        tren_aterrizaje.draw(modo, r, g, b, grosor);
        glPopMatrix();

        // Trenes de aterrizaje laterales
        glPushMatrix();
        glTranslatef(0.1, -fuselaje.altura / 4, 0);
        glRotatef(15, 1, 0, 0);
        glScalef(ESCALA_TREN_AT, ESCALA_TREN_AT, ESCALA_TREN_AT);
        tren_aterrizaje.draw(modo, r, g, b, grosor);
        glPopMatrix();

        glPushMatrix();
        glTranslatef(-0.1, -fuselaje.altura / 4, 0);
        glRotatef(15, 1, 0, 0);
        glScalef(ESCALA_TREN_AT, ESCALA_TREN_AT, ESCALA_TREN_AT);
        tren_aterrizaje.draw(modo, r, g, b, grosor);
        glPopMatrix();

    glPopMatrix();
}