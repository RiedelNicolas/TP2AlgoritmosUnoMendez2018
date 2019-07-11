#include "quidditch.h"
#include  "sombrero.h" 


void imprimir ( char campo [FILAS_CAMPO][COLUMNAS_CAMPO], juego_t juego );
void limpiar_pantalla();
void imprimir_casa(juego_t);
void imprimir_campo(char campo [FILAS_CAMPO][COLUMNAS_CAMPO]);
void bucle_juego( char campo [FILAS_CAMPO][COLUMNAS_CAMPO], juego_t *juego );

int main(){

  char campo[FILAS_CAMPO][COLUMNAS_CAMPO];
  juego_t juego;                                                                   //defino e inicio las variables.
  char casa=obtener_casa();
  inicializar_juego(campo, casa, &juego);

  bucle_juego(campo, &juego);
  mostrar_resultado(juego);

  return 0;
}


//hace el loop del juego, mostrando en pantalla.
void bucle_juego (char campo [FILAS_CAMPO][COLUMNAS_CAMPO], juego_t *juego) {
  char control;
  imprimir(campo,*juego);
  while ( !finalizo_juego(juego) ){  
    control = pedir_movimiento (juego->posicion_jugador);
    mover_jugador(campo,control,juego);

    if(juego->casa==HUFFLEPUFF && !finalizo_juego(juego)){
        imprimir(campo,*juego);
        control = pedir_movimiento (juego->posicion_jugador);
        mover_jugador(campo,control,juego);
    }

    if(!finalizo_juego(juego))
      mover_bludgers(campo,juego);

      if(!finalizo_juego(juego))
        mover_snitch( campo, juego);

    imprimir(campo,*juego);

  }

}


//limpia la consola de una manera muy rustica, no estoy seguro de usar system("clear"); ya que no se en que sistema operativo lo vas a correr.
void limpiar_pantalla(){

  for (int i = 0; i < ALTO_CONSOLA ; ++i){
   printf("\n");
  }

}

//una mezcla de imprimir el campo y los stats del juego, que pueden ser importantes para una mejor experiencia.
void imprimir ( char campo [FILAS_CAMPO][COLUMNAS_CAMPO], juego_t juego ) {

  imprimir_campo(campo);


  printf("Tu resistencia restante es %i \n",juego.resistencia);
  imprimir_casa(juego);

}


void imprimir_campo(char campo [FILAS_CAMPO][COLUMNAS_CAMPO]){
  limpiar_pantalla();

  printf("   ");
  for (int i=0;i<FILAS_CAMPO+5;i++){
    printf(" _");
  }
  printf("\n");


  for (int i = 0;i<FILAS_CAMPO;i++){
    printf("**|");  
    for (int j = 0;j<COLUMNAS_CAMPO;j++){
      printf("|");
      printf ("_");
      printf ("%c",campo[i][j]);
      printf("_");

    }
    printf("|**");
    printf ("\n");

  }

  printf ("\n");
}


void imprimir_casa(juego_t juego){
  char casa[MAX_CASA];
  switch(juego.casa){
    case HUFFLEPUFF:
      strcpy(casa,"Hufflepuff");
    break;

    case SLYTHERIN:
      strcpy(casa,"Slytherin");
    break;

    case RAVENCLAW:
      strcpy(casa,"Ravenclaw");
    break;

    case GRYFFINDOR:
      strcpy(casa,"Gryffindor");
    break;
  }

  printf("Tu casa es %s \n", casa);
}




