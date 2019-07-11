#ifndef QUIDDITCH_H
#define QUIDDITCH_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

//campo
#define FILAS_CAMPO 25
#define COLUMNAS_CAMPO 15

//bludgers
#define FILA_BLUDGER1 2
#define DISTANCIA_ENTRE_FILAS_BLUDGERS 5
#define CANTIDAD_BLUDGERS_INICIAL 5
#define GOLPE_DE_BLUDGER 10
#define BLUDGER   'B'
#define SENTIDO_DERECHO  'D'
#define SENTIDO_IZQUIERDO 'I'

//dementores
#define CANTIDAD_DEMENTORES_INICIAL 4
#define DEMENTOR  'D'
static const char FILAS_DEMENTORES[]={5, 5, 19, 19};                         //anibal,de la forma que me lo explicaste recibia errores.
static const char COLUMNAS_DEMENTORES[]={3, 11, 3 , 11};                     //encontre esta forma en internet,no tenia tiempo para consultarte.

//snitch
#define SNITCH    'S'
#define DIST_MIN_INICIAL_SNITCH_J 5                                   //distancia minima de la que se genera la snitch respecto del jugador
#define DISTANCIA_PASIVA_SNITCH 10
#define DIRECCIONES_SNITCH 4

//jugador
#define COLUMNA_INICIAL_JUGADOR  7
#define FILA_INICIAL_JUGADOR    12
#define RESISTENCIA_INICIAL     50
#define JUGADOR   'J'

//estado de juego
#define JUGADOR_GANO     1
#define JUGADOR_PERDIO   2
#define EN_JUEGO         0

//controles
#define ARRIBA    'w'
#define ABAJO     's'
#define IZQUIERDA 'a'
#define DERECHA   'd'

//casas
#define GRYFFINDOR 'G'
#define SLYTHERIN  'S'
#define RAVENCLAW  'R'
#define HUFFLEPUFF 'H'


//print
#define ALTO_CONSOLA 132
#define MAX_CASA 15


typedef struct coordenada {
	int fila;
	int columna;
} coordenada_t;

typedef struct juego {
	coordenada_t posicion_jugador;
	char casa;
	int resistencia;
	coordenada_t posicion_snitch;

	int bludgers_en_juego; // tope del vector posicion_bludgers
	coordenada_t posicion_bludgers[CANTIDAD_BLUDGERS_INICIAL];
	char direccion; // 'D': derecha – 'I': izquierda
	int dementores_en_juego; // tope del vector posicion_dementores
	coordenada_t posicion_dementores[CANTIDAD_DEMENTORES_INICIAL];
	
	int estado; // 0: en ejecucion – 1: jugador gano – 2: jugador perdio
} juego_t;

/*
 Posiciona los elementos en la matriz y da los valores iniciales al registro juego.
 */
void inicializar_juego(char campo[FILAS_CAMPO][COLUMNAS_CAMPO], char casa, juego_t *juego);

/*
  Pide un movimiento por teclado, no es necesaria que sea valido ya que se volvera a pedir
 hasta que se obtenga uno,devuelve un moviemiento valido.
 */
char pedir_movimiento(coordenada_t posicion_jugador);

/*
  Recibe un movimiento valido y mueve al jugador tanto en la matriz como el registro
 ,verificando el estado de juego.

 */
void mover_jugador(char campo[FILAS_CAMPO][COLUMNAS_CAMPO], char direccion, juego_t *juego);

/*
   con los datos de la posicion del jugador y los elementos en el campo,
   mueve la snitch a la posicion mas optima para escapar del jugador.
 */
void mover_snitch(char campo[FILAS_CAMPO][COLUMNAS_CAMPO], juego_t *juego);

/*
 mueve bludger en la matriz y actualiza el registro, verifica el estado de juego.
 Todas las bludgers deben compartir sentido y n° colummna.
 */
void mover_bludgers(char campo[FILAS_CAMPO][COLUMNAS_CAMPO], juego_t *juego);

/*
 genera una posicion aleatoria para la snitch, esta posicion se encuentra adentro
 del campo y a una distancia mayor o igual de 5(mahattan) de la posicion inicial
 del jugador.
 */
coordenada_t posicion_inicial_snitch();

/*
 recibe el estado de juego y devuelve true si finalizo,o false si el juego
 continua.

 */
bool finalizo_juego(juego_t *juego);

/*
Ejecutar una vez que el juego termino,dependiendo del resultado mostrara por 
pantalla si el jugador gano o perdio la partida.
 */
void mostrar_resultado(juego_t juego);


#endif /* QUIDDITCH_H */
