//**************************************************************************
// Práctica 2
//**************************************************************************

#include <GL/glut.h>
#include <ctype.h>
#include <math.h>
#include <vector>
#include "objetos_B4.h"

using namespace std;

// tipos
typedef enum
{
	CUBO,
	PIRAMIDE,
	CONO,
	CILINDRO,
	ESFERA,
	OBJETO_PLY,
	ROTACION,
	ROTACION_PLY,
	EXTRUSION,
	JERARQUICO
} _tipo_objeto;
_tipo_objeto t_objeto = JERARQUICO;
_modo modo = SOLID;

// variables que definen la posicion de la camara en coordenadas polares
GLfloat Observer_distance;
GLfloat Observer_angle_x;
GLfloat Observer_angle_y;
_vertex3f previous_observer_state;

// variables que controlan la ventana y la transformacion de perspectiva
GLfloat Size_x, Size_y, Front_plane, Back_plane;

// variables que determninan la posicion y tamaño de la ventana X
int Window_x = 50, Window_y = 50, Window_width = 650, Window_high = 650;

// objetos
_cubo cubo;
_piramide piramide;
_cono cono;
_cilindro cilindro;
_esfera esfera;
_objeto_ply ply;
_rotacion rotacion;
_rotacion_ply rotacion_ply;
_extrusion *extrusion;
_avion *jerarquico = new _avion(0, 0, 0);

typedef enum
{
	INICIO,
	ENCENDIDO_MOTOR,
	APERTURA_ALERONES,
	COMPROBACION_TIMON_1,
	COMPROBACION_TIMON_2,
	COMPROBACION_TIMON_3,
	ACELERACION_1,
	ACELERACION_2,
	ACELERACION_3,
	DESPEGUE,
	FINAL
} paso_animacion;
paso_animacion paso = INICIO;
bool reproducir_animacion = false;

//**************************************************************************
//
//***************************************************************************

void clean_window()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//**************************************************************************
// Funcion para definir la transformación de proyeccion
//***************************************************************************

void change_projection()
{

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	// formato(x_minimo,x_maximo, y_minimo, y_maximo,plano_delantero, plano_traser)
	//  plano_delantero>0  plano_trasero>PlanoDelantero)
	glFrustum(-Size_x, Size_x, -Size_y, Size_y, Front_plane, Back_plane);
}

//**************************************************************************
// Funcion para definir la transformación*ply1 de vista (posicionar la camara)
//***************************************************************************

void change_observer()
{
	// posicion del observador
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0, 0, -Observer_distance);
	glRotatef(Observer_angle_x, 1, 0, 0);
	glRotatef(Observer_angle_y, 0, 1, 0);
}

//**************************************************************************
// Funcion que dibuja los ejes utilizando la primitiva grafica de lineas
//***************************************************************************

void draw_axis()
{
	/**
	 * Al dibujar las aristas de una figura, hay que reestablecer el
	 * grosor de las líneas, si no, los ejes tendrán un grosor igual
	 * al de las aristas
	 */
	glLineWidth(1);

	glDisable(GL_LIGHTING);
	glLineWidth(2);
	glBegin(GL_LINES);
	// eje X, color rojo
	glColor3f(1, 0, 0);
	glVertex3f(-AXIS_SIZE, 0, 0);
	glVertex3f(AXIS_SIZE, 0, 0);
	// eje Y, color verde
	glColor3f(0, 1, 0);
	glVertex3f(0, -AXIS_SIZE, 0);
	glVertex3f(0, AXIS_SIZE, 0);
	// eje Z, color azul
	glColor3f(0, 0, 1);
	glVertex3f(0, 0, -AXIS_SIZE);
	glVertex3f(0, 0, AXIS_SIZE);
	glEnd();
}

//**************************************************************************
// Funcion que dibuja los objetos
//**************************************************************************

// Cambia la posicion de la camara
void change_observer_values(float x, float y, float d)
{
	Observer_angle_x = x;
	Observer_angle_y = y;
	Observer_distance = d;
}

// Realiza un incremento sobre los angulos y distancia de la camara
void iterate_observer(float x, float y, float d)
{
	Observer_angle_x += x;
	Observer_angle_y += y;
	Observer_distance += d;
}

// Restaura la posicion de la camara
void restore_observer()
{
	Observer_angle_x = previous_observer_state._0;
	Observer_angle_y = previous_observer_state._1;
	Observer_distance = previous_observer_state._2;
}

// Reproduce la animacion si esta detenida, y la detiene si esta en curso
// Restaura los valores del modelo
// Si la animacion va a detenerse, restaura la camara a como estaba antes de empezar la animacion
void alternar_animacion()
{
	// Si la animacion esta en curso, restauramos la camara
	if (reproducir_animacion)
	{
		std::cout << "\t--- ANIMACION DETENIDA ---\n";
		restore_observer();
	}
	// Al iniciar/detener la animacion, restauramos los valores del modelo
	jerarquico->reset_model_values();
	reproducir_animacion = !reproducir_animacion;
	//// std::cout << reproducir_animacion << "\n";
	paso = INICIO;
}

// Rota la helice 'angulo' grados
// TODO: Pasar este metodo junto con otros a la clase _avion
void rotar_helice(float angulo)
{
	jerarquico->rotacion_helice = int(jerarquico->rotacion_helice + angulo) % 360;
}

// Funcion de animacion del modelo
static void animate()
{
	// Apaga la animacion si esta en curso y se ha cambiado el objeto a dibujar
	if (t_objeto != JERARQUICO && reproducir_animacion) {
		alternar_animacion();
	}

	else if (reproducir_animacion)
	{
		switch (paso)
		{
		// Usamos este paso para salvaguardar los valores de la camara
		// y despues poder modificarla para la animacion
		case INICIO:
			std::cout << "\t--- ANIMACION INICIADA ---\n";
			previous_observer_state(Observer_angle_x, Observer_angle_y, Observer_distance);
			change_observer_values(-1, 0, 4);
			std::cout << "Encendiendo motores...\n";
			paso = ENCENDIDO_MOTOR;
			break;

		// La helice empieza a girar un total de 3 veces antes del siguiente paso
		case ENCENDIDO_MOTOR:
			rotar_helice(3);
			Observer_distance += 0.005;
			if (Observer_distance >= 5)
			{
				std::cout << "Abriendo alerones...\n";
				paso = APERTURA_ALERONES;
				change_observer_values(5, -73, 9);
			}
			break;

		// Los alerones se abren 45 grados antes de pasar al siguiente paso
		case APERTURA_ALERONES:
			// A partir de ahora rotaremos la helice mas rapido en toda la animacion
			rotar_helice(5);
			jerarquico->apertura_alerones += 0.25;
			iterate_observer(-0.01, -0.25, -0.01);
			if (jerarquico->apertura_alerones >= 45)
			{
				std::cout << "Comprobando timon por la izquierda...\n";
				paso = COMPROBACION_TIMON_1;
				change_observer_values(17, -180, 4);
			}
			break;

		case COMPROBACION_TIMON_1:
			rotar_helice(6);
			jerarquico->direccion_timon += 0.5;
			Observer_distance += 0.01;
			if (jerarquico->direccion_timon >= 20)
			{
				std::cout << "Comprobando timon por la derecha...\n";
				paso = COMPROBACION_TIMON_2;
			}
			break;

		case COMPROBACION_TIMON_2:
			rotar_helice(9);
			jerarquico->direccion_timon -= 0.5;
			Observer_distance += 0.01;
			if (jerarquico->direccion_timon <= -20)
			{
				std::cout << "Centrando timon...\n";
				paso = COMPROBACION_TIMON_3;
			}
			break;

		case COMPROBACION_TIMON_3:
			rotar_helice(10);
			jerarquico->direccion_timon += 0.5;
			Observer_distance += 0.01;
			if (jerarquico->direccion_timon == 0)
			{
				std::cout << "Preparando para el despegue...\n";
				paso = ACELERACION_1;
				jerarquico->posicion(0, 0, -5);
				change_observer_values(0, 0, 8);
			}
			break;

		case ACELERACION_1:
			rotar_helice(12);
			jerarquico->posicion.z += 0.02;
			if (jerarquico->posicion.z >= -1)
			{
				std::cout << "Despegando, guardando trenes de aterrizaje..." << std::endl;
				paso = ACELERACION_2;
				change_observer_values(0, -90, 8);
			}
			break;

		// El avion despega y se guarda el tren de aterrizaje
		case ACELERACION_2:
			rotar_helice(15);
			jerarquico->posicion.z += 0.04;
			jerarquico->posicion.y += 0.01;
			jerarquico->apertura_tren += 1;
			jerarquico->inclinacion_vertical -= 0.1;
			if (jerarquico->posicion.z >= 4 && jerarquico->apertura_tren >= 45)
			{
				paso = ACELERACION_3;
				change_observer_values(-5, -180, 8);
			}
			break;

		// Por ultimo, el avion empieza a dirigirse hacia el horizonte
		case ACELERACION_3:
			rotar_helice(15);
			jerarquico->posicion.z += 0.08;
			jerarquico->posicion.y += 0.04;
			jerarquico->inclinacion_vertical -= 0.1;
			if (jerarquico->posicion.z >= 20)
			{
				paso = FINAL;
			}
			break;

		// Final de la animacion, se realizan las operaciones necesarias
		case FINAL:
			alternar_animacion();
			break;

		default:
			break;
		}
	}
	glutPostRedisplay();
}

void draw_objects()
{
	// Si se va a cambiar de objeto cuando se este reproduciendo la animacion
	// entonces se detendra automaticamente
	if (t_objeto != JERARQUICO && reproducir_animacion) {
		alternar_animacion();
	}

	switch (t_objeto)
	{
	case CUBO:
		cubo.draw(modo, 1.0, 0.0, 0.0, 5);
		break;
	case PIRAMIDE:
		piramide.draw(modo, 1.0, 0.0, 0.0, 5);
		break;
	case CONO:
		cono.draw(modo, 1.0, 0.0, 0.0, 5);
		break;
	case CILINDRO:
		cilindro.draw(modo, 1.0, 0.0, 0.0, 5);
		break;
	case ESFERA:
		esfera.draw(modo, 1.0, 0.0, 0.0, 5);
		break;
	case OBJETO_PLY:
		ply.draw(modo, 1.0, 0.6, 0.0, 5);
		break;
	case ROTACION_PLY:
		rotacion_ply.draw(modo, 1.0, 0.6, 0.0, 5);
		break;
	case ROTACION:
		rotacion.draw(modo, 1.0, 0.0, 0.0, 5);
		break;
	case EXTRUSION:
		extrusion->draw(modo, 1.0, 0.0, 0.0, 5);
		break;
	case JERARQUICO:
		jerarquico->draw(modo, 1.0, 0.0, 0.0, 5);
		break;
	}
}

//**************************************************************************
//
//***************************************************************************

void draw(void)
{
	clean_window();
	change_observer();
	draw_axis();
	draw_objects();
	glutSwapBuffers();
}

//***************************************************************************
// Funcion llamada cuando se produce un cambio en el tamaño de la ventana
//
// el evento manda a la funcion:
// nuevo ancho
// nuevo alto
//***************************************************************************

void change_window_size(int Ancho1, int Alto1)
{
	float Aspect_ratio;

	Aspect_ratio = (float)Alto1 / (float)Ancho1;
	Size_y = Size_x * Aspect_ratio;
	change_projection();
	glViewport(0, 0, Ancho1, Alto1);
	glutPostRedisplay();
}

//***************************************************************************
// Funcion llamada cuando se aprieta una tecla normal
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton
//***************************************************************************

void normal_key(unsigned char Tecla1, int x, int y)
{
	switch (toupper(Tecla1))
	{
	case 'Q':
		exit(0);
	case '1':
		modo = POINTS;
		break;
	case '2':
		modo = EDGES;
		break;
	case '3':
		modo = SOLID;
		break;
	case '4':
		modo = SOLID_COLORS;
		break;
	case 'P':
		t_objeto = PIRAMIDE;
		break;
	case 'C':
		t_objeto = CUBO;
		break;
	case 'T':
		t_objeto = CONO;
		break;
	case 'Y':
		t_objeto = CILINDRO;
		break;
	case 'U':
		t_objeto = ESFERA;
		break;
	case 'O':
		t_objeto = OBJETO_PLY;
		break;
	case 'R':
		t_objeto = ROTACION;
		break;
	case 'L':
		t_objeto = ROTACION_PLY;
		break;
	case 'X':
		t_objeto = EXTRUSION;
		break;
	case 'A':
		t_objeto = JERARQUICO;
		break;
	case 'S':
		if (t_objeto == JERARQUICO)
			alternar_animacion();
		break;
	case '\\':
		std::cout << "Observer_angle_x: " << Observer_angle_x
				  << "\nObserver_angle_y: " << Observer_angle_y
				  << "\nObserver_distance: " << Observer_distance << "\n";
		break;
	}
	glutPostRedisplay();
}

//***************************************************************************
// Funcion l-olamada cuando se aprieta una tecla especial
//
// el evento manda a la funcion:
// codigo de la tecla
// posicion x del raton
// posicion y del raton

//***************************************************************************

void special_key(int Tecla1, int x, int y)
{

	// Los controles de camara y de articulaciones
	// permaneceran bloqueados durante la animacion
	if (reproducir_animacion) return;

	switch (Tecla1)
	{
	case GLUT_KEY_LEFT:
		Observer_angle_y--;
		break;
	case GLUT_KEY_RIGHT:
		Observer_angle_y++;
		break;
	case GLUT_KEY_UP:
		Observer_angle_x--;
		break;
	case GLUT_KEY_DOWN:
		Observer_angle_x++;
		break;
	case GLUT_KEY_PAGE_UP:
		Observer_distance *= 1.2;
		break;
	case GLUT_KEY_PAGE_DOWN:
		Observer_distance /= 1.2;
		break;
	case GLUT_KEY_F1:
		if (jerarquico->apertura_alerones >= 0 && jerarquico->apertura_alerones < 45)
			jerarquico->apertura_alerones += 1;
		break;
	case GLUT_KEY_F2:
		if (jerarquico->apertura_alerones > 0 && jerarquico->apertura_alerones <= 45)
			jerarquico->apertura_alerones -= 1;
		break;
	case GLUT_KEY_F3:
		jerarquico->rotacion_helice = int(jerarquico->rotacion_helice + 5) % 360;
		break;
	case GLUT_KEY_F4:
		jerarquico->rotacion_helice = int(jerarquico->rotacion_helice - 5) % 360;
		break;
	case GLUT_KEY_F5:
		if (jerarquico->apertura_tren >= -15 && jerarquico->apertura_tren < 90)
			jerarquico->apertura_tren += 1;
		break;
	case GLUT_KEY_F6:
		if (jerarquico->apertura_tren > -15 && jerarquico->apertura_tren <= 90)
			jerarquico->apertura_tren -= 1;
		break;
	case GLUT_KEY_F7:
		if (jerarquico->direccion_timon >= -20 && jerarquico->direccion_timon < 20)
			jerarquico->direccion_timon += 1;
		break;
	case GLUT_KEY_F8:
		if (jerarquico->direccion_timon > -20 && jerarquico->direccion_timon <= 20)
			jerarquico->direccion_timon -= 1;
		break;
	}
	glutPostRedisplay();
}

//***************************************************************************
// Funcion de incializacion
//***************************************************************************

// Muestra por pantalla los controles del programa
void print_controls() {
	/**
	 * ----- MAPA DE TECLAS -----
	 * Q: Salir
	 * 1: Modo de puntos
	 * 2: Modo de aristas
	 * 3: Modo de color solido
	 * 4: Modo de color aleatorio
	 * P: Piramide
	 * C: Cubo
	 * T: Cono
	 * Y: Cilindro
	 * U: Esfera
	 * O: Objeto PLY
	 * R: Objeto por rotacion
	 * L: Objeto por rotacion mediante perfil PLY
	 * X: Objeto por extrusion
	 * A: Objeto jerarquico
	 * S: Animacion modelo jerarquico
	 * ---------------------------
	 */
	std::cout << "\t\t---------- CONTROLES ----------\t\t\n"
			  << "\t--- Controles generales ---\n"
			  << "Flechas\t->\tMoverse alrededor del origen\n"
			  << "PgUp\t->\tAcercar camara\n"
			  << "PgDown\t->\tAlejar camara\n"
			  << "[1]\t->\tDibujado de puntos\n"
			  << "[2]\t->\tDibujado de aristas\n"
			  << "[3]\t->\tDibujado de color solido\n"
			  << "[4]\t->\tDibujado de color aleatorio\n"
			  << "[P]\t->\tDibujar piramide\n"
			  << "[C]\t->\tDibujar cubo\n"
			  << "[T]\t->\tDibujar cono\n"
			  << "[Y]\t->\tDibujar cilindro\n"
			  << "[U]\t->\tDibujar esfera\n"
			  << "[O]\t->\tDibujar objeto PLY\n"
			  << "[R]\t->\tDibujar objeto por rotacion\n"
			  << "[L]\t->\tDibujar objeto por mediante perfil PLY\n"
			  << "[X]\t->\tDibujar objeto por mediante perfil PLY\n"
			  << "[A]\t->\tDibujar objeto jerarquico (avion)\n"
			  << "[S]\t->\tIniciar/detener animacion (objeto jerarquico)\n"
			  << "\t--- Controles del objeto jerarquico ---\n"
			  << "[F1]\t->\tAbrir alerones\n"
			  << "[F2]\t->\tCerrar alerones\n"
			  << "[F3]\t->\tRotar helice en sentido horario\n"
			  << "[F4]\t->\tRotar helice en sentido antihorario\n"
			  << "[F5]\t->\tGuardar trenes de aterrizaje\n"
			  << "[F6]\t->\tDesplegar trenes de aterrizaje\n"
			  << "[F7]\t->\tAjustar timon hacia la izquierda\n"
			  << "[F8]\t->\tAjustar timon hacia la derecha\n"
			  << "\t--- Utilidades ---\n"
			  << "[\\]\t->\tMostrar coordenadas de camara\n"
			  << "\n";
}

void initialize(void)
{
	// se inicalizan la ventana y los planos de corte
	Size_x = 0.5;
	Size_y = 0.5;
	Front_plane = 1;
	Back_plane = 1000;

	// se incia la posicion del observador, en el eje z
	Observer_distance = 8 * Front_plane;
	Observer_angle_x = 30;
	Observer_angle_y = -45;

	// se indica el color para limpiar la ventana	(r,v,a,al)
	// blanco=(1,1,1,1) rojo=(1,0,0,1), ...
	glClearColor(.1, .1, .1, 1);

	// se habilita el z-bufer
	glEnable(GL_DEPTH_TEST);
	change_projection();
	glViewport(0, 0, Window_width, Window_high);
}

//***************************************************************************
// Programa principal
//
// Se encarga de iniciar la ventana, asignar las funciones e comenzar el
// bucle de eventos
//***************************************************************************

int main(int argc, char *argv[])
{
	print_controls();

	// perfil

	vector<_vertex3f> perfil, poligono;
	_vertex3f aux;

	aux.x = 1.0;
	aux.y = -1.0;
	aux.z = 0.0;
	perfil.push_back(aux);
	aux.x = 1.0;
	aux.y = 1.0;
	aux.z = 0.0;
	perfil.push_back(aux);

	rotacion.parametros(perfil, 6, true, false);

	aux.x = 1.0;
	aux.y = 0.0;
	aux.z = 1.0;
	poligono.push_back(aux);
	aux.x = 1.0;
	aux.y = 0.0;
	aux.z = -1.0;
	poligono.push_back(aux);
	aux.x = -1.2;
	aux.y = 0.0;
	aux.z = -1.0;
	poligono.push_back(aux);
	aux.x = -0.8;
	aux.y = 0.0;
	aux.z = 0.0;
	poligono.push_back(aux);
	aux.x = -1.2;
	aux.y = 0.0;
	aux.z = 1.0;
	poligono.push_back(aux);

	extrusion = new _extrusion(poligono, 0.25, 1.0, 0.25);

	// se llama a la inicialización de glut
	glutInit(&argc, argv);

	// se indica las caracteristicas que se desean para la visualización con OpenGL
	// Las posibilidades son:
	// GLUT_SIMPLE -> memoria de imagen simple
	// GLUT_DOUBLE -> memoria de imagen doble
	// GLUT_INDEX -> memoria de imagen con color indizado
	// GLUT_RGB -> memoria de imagen con componentes rojo, verde y azul para cada pixel
	// GLUT_RGBA -> memoria de imagen con componentes rojo, verde, azul y alfa para cada pixel
	// GLUT_DEPTH -> memoria de profundidad o z-bufer
	// GLUT_STENCIL -> memoria de estarcido_rotation Rotation;
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	// posicion de la esquina inferior izquierdad de la ventana
	glutInitWindowPosition(Window_x, Window_y);

	// tamaño de la ventana (ancho y alto)
	glutInitWindowSize(Window_width, Window_high);

	// llamada para crear la ventana, indicando el titulo (no se visualiza hasta que se llama
	// al bucle de eventos)
	glutCreateWindow("Practica 3 IG - Luis Miguel Guirado Bautista");

	// asignación de la funcion llamada "dibujar" al evento de dibujo
	glutDisplayFunc(draw);
	// asignación de la funcion llamada "change_window_size" al evento correspondiente
	glutReshapeFunc(change_window_size);
	// asignación de la funcion llamada "normal_key" al evento correspondiente
	glutKeyboardFunc(normal_key);
	// asignación de la funcion llamada "tecla_Especial" al evento correspondiente
	glutSpecialFunc(special_key);

	glutIdleFunc(animate);

	// funcion de inicialización
	initialize();

	// creación del objeto ply
	ply.parametros(argv[1]);

	rotacion_ply.parametros("peon.perfil", 10);

	// ply = new _objeto_ply(argv[1]);

	// inicio del bucle de eventos
	glutMainLoop();
	
	return 0;
}
