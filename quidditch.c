#include "quidditch.h"

//firmas de las funciones privadas

//uso compartido
bool esta_en_campo(int fila,int col);
bool coordenada_en_campo(coordenada_t pos);
bool esta_libre(char campo[FILAS_CAMPO][COLUMNAS_CAMPO],int fila,int columna);
bool coordenada_libre(char campo[FILAS_CAMPO][COLUMNAS_CAMPO], coordenada_t pos);
bool comparar_posiciones (coordenada_t pos1,coordenada_t pos2);
int distancia_manhattan(coordenada_t pos1,coordenada_t pos2);
void revisar_estado (juego_t *juego,char campo[FILAS_CAMPO][COLUMNAS_CAMPO]);


//movimiento del jugador
bool control_valido(char tecla);

//incializar
void limpiar_campo(char campo[FILAS_CAMPO][COLUMNAS_CAMPO]);
void inicializar_jugador(char campo[FILAS_CAMPO][COLUMNAS_CAMPO],char casa, juego_t *juego);
void inicializar_bludgers(char campo[FILAS_CAMPO][COLUMNAS_CAMPO],juego_t *juego);
void inicializar_snitch(char campo[FILAS_CAMPO][COLUMNAS_CAMPO],juego_t *juego);
void inicializar_dementores(char campo[FILAS_CAMPO][COLUMNAS_CAMPO],juego_t *juego);

//bludgers
void efecto_bludgers (juego_t *juego,char campo[FILAS_CAMPO][COLUMNAS_CAMPO]);
void cambio_direccion(char campo[FILAS_CAMPO][COLUMNAS_CAMPO],juego_t *juego);

//dementores
void efecto_dementores(juego_t *juego,char campo[FILAS_CAMPO][COLUMNAS_CAMPO]);

//snitch,(se que son muy largas las firmas. Pero no queria ponerle nombres muy cortos a las variables y/o constantes.)
void vectores_snitch(coordenada_t movimientos[DIRECCIONES_SNITCH],bool validez[DIRECCIONES_SNITCH],juego_t *juego,char campo[FILAS_CAMPO][COLUMNAS_CAMPO]);
void buscar_mejor_mov_snitch(bool validez[DIRECCIONES_SNITCH],coordenada_t movimientos[DIRECCIONES_SNITCH],coordenada_t *mejor_movimiento,juego_t *juego);

////////////////////////////////////////////////FUNCIONES PEDIDAS POR ENUNCIADO////////////////////////////////////////////////////////////////////////

void inicializar_juego(char campo[FILAS_CAMPO][COLUMNAS_CAMPO],char casa, juego_t *juego){
  
  limpiar_campo(campo);                                //matriz se pone en blanco.
  inicializar_jugador(campo, casa, juego);
  inicializar_bludgers(campo, juego);
  inicializar_dementores(campo, juego);
  inicializar_snitch(campo,juego);
  juego->estado=EN_JUEGO;

}


char pedir_movimiento(coordenada_t posicion_jugador){

  char direccion=' ';
  bool salio;
                                                                   
  do{
    salio=false;

    scanf(" %c", &direccion);

    if ( (direccion == ARRIBA) && (posicion_jugador.fila == 0) ){
      salio=true;
    }
    if ( (direccion == ABAJO) && (posicion_jugador.fila == FILAS_CAMPO-1) ){
      salio=true;
    }
    if ( (direccion == DERECHA) && (posicion_jugador.columna == COLUMNAS_CAMPO-1) ){
      salio=true;
    }
    if ( (direccion == IZQUIERDA) && (posicion_jugador.columna == 0) ){
      salio=true;

    }

  }while( !(control_valido( direccion)) || (salio) );  
    
   return direccion;

}


void mover_jugador(char campo[FILAS_CAMPO][COLUMNAS_CAMPO], char direccion, juego_t *juego){
    
  campo[juego->posicion_jugador.fila][juego->posicion_jugador.columna]=' ';

  switch (direccion) {

    case ARRIBA :
      (juego->posicion_jugador.fila)--;
    break;

    case ABAJO :
      (juego->posicion_jugador.fila)++;   
    break;

    case IZQUIERDA :
      (juego->posicion_jugador.columna)--;
    break;

    case DERECHA :
      (juego->posicion_jugador.columna)++;
    break;
  }

  campo[juego->posicion_jugador.fila][juego->posicion_jugador.columna]=JUGADOR;
  (juego->resistencia)--; 

	efecto_dementores(juego, campo);                     //juego, pasa crudo sin * ya que seria la segunda referencia y se anularia con & .
  efecto_bludgers(juego, campo);
  revisar_estado (juego, campo);

}


  void mover_bludgers(char campo[FILAS_CAMPO][COLUMNAS_CAMPO],juego_t *juego){
  cambio_direccion(campo, juego);

  for (int i=0 ; i < juego->bludgers_en_juego; i++){

    campo[(juego->posicion_bludgers[i].fila)][(juego->posicion_bludgers[i].columna)]=' ';      //no se puede estar en 2 lugares al mismo tiempo
        
    if(juego->direccion == SENTIDO_DERECHO){
      (juego->posicion_bludgers[i].columna)++; 
    }

    if(juego->direccion == SENTIDO_IZQUIERDO){
      (juego->posicion_bludgers[i].columna)--;
    }

    campo[(juego->posicion_bludgers[i].fila)][(juego->posicion_bludgers[i].columna)]=BLUDGER; 
  }

  efecto_bludgers(juego, campo);
  revisar_estado (juego, campo);

}


void mover_snitch(char campo[FILAS_CAMPO][COLUMNAS_CAMPO], juego_t *juego){ 

	int distancia_inicial = distancia_manhattan(juego->posicion_jugador,juego->posicion_snitch);

  if( distancia_inicial <= DISTANCIA_PASIVA_SNITCH ){

    coordenada_t mejor_movimiento = juego->posicion_jugador;                  //inicializado en el peor caso posible.(min distancia)
    coordenada_t movimientos[DIRECCIONES_SNITCH];
    bool validez[DIRECCIONES_SNITCH];

    vectores_snitch(movimientos,validez,juego,campo);                       //inicio el vector de posiciones y reviso cuales son validas
    buscar_mejor_mov_snitch(validez,movimientos,& mejor_movimiento, juego); //busco la mejor.

    campo[juego->posicion_snitch.fila][juego->posicion_snitch.columna]=' ';
    juego->posicion_snitch=mejor_movimiento;
    campo[juego->posicion_snitch.fila][juego->posicion_snitch.columna]=SNITCH;

    revisar_estado (juego, campo);
                                   //reviso.porque puede pasar que sea RAVENCLAW y la snitch se meta adentro de mi rango
    }
}	


coordenada_t posicion_inicial_snitch(){

  coordenada_t posicion;
  int distancia;
  srand( (unsigned int) clock() );

  do{
    posicion.fila=rand()%FILAS_CAMPO;
    posicion.columna=rand()%COLUMNAS_CAMPO;
    distancia =abs(posicion.fila - FILA_INICIAL_JUGADOR)+ abs(posicion.columna - COLUMNA_INICIAL_JUGADOR);
  }while(distancia<=DIST_MIN_INICIAL_SNITCH_J);

  return posicion;
}


bool finalizo_juego(juego_t *juego){
  
  return ( (juego->estado==JUGADOR_GANO) || (juego->estado==JUGADOR_PERDIO) )  ;

}


void mostrar_resultado(juego_t juego){
  if(juego.estado == JUGADOR_GANO)
    printf ("\n Ganaste,atrapaste la snitch! \n ");
  else
    printf("\n Perdiste,segui practicando!! \n");

}


//-//-//-//-//-//-//-//-//-//-//FUNCIONES PRIVADAS-//-//-//-//-//-//-//-//-//-//-//-//-//


//devuelve true si las posiciones son iguales.
bool comparar_posiciones (coordenada_t pos1,coordenada_t pos2){

	return( (pos1.fila == pos2.fila) && (pos1.columna == pos2.columna) );

}


//recibe coordeandas_t
int distancia_manhattan(coordenada_t pos1,coordenada_t pos2){

  return ( abs(pos1.fila-pos2.fila) + abs(pos1.columna-pos2.columna) );

}


//hace todo el activo de los dementores, incluyendo borrar los mismos.
void efecto_dementores(juego_t *juego,char campo[FILAS_CAMPO][COLUMNAS_CAMPO]){

	if (juego->casa == GRYFFINDOR){                                                                   
    bool uso_especto=false;
    int pos;
    int i=0;

    while( (i<juego->dementores_en_juego) && !(uso_especto) ) {
      if(comparar_posiciones(juego->posicion_dementores[i],juego->posicion_jugador) ){
    	 uso_especto=true;
    	 pos=i;
      }
      i++;
    }

    if(uso_especto){                                                                           //borra dementores
      for(i=pos ; i< ((juego->dementores_en_juego)-1) ; i++)
        juego->posicion_dementores[i]=juego->posicion_dementores[i+1];
        (juego->dementores_en_juego)--;
    }


  }

  if(juego->casa!=GRYFFINDOR){
    for (int i=0; i<juego->dementores_en_juego; i++){
      if(comparar_posiciones(juego->posicion_jugador,juego->posicion_dementores[i]) ) {
        juego->resistencia=0;
      }
    }
  }

}


//hace todo el activo de bludgers, incluytendo borrar a las mismas.
void efecto_bludgers (juego_t *juego,char campo[FILAS_CAMPO][COLUMNAS_CAMPO]){

	if(juego->casa != SLYTHERIN){
    int pos;
    bool choco=false;
    for (int i=0; i<juego->bludgers_en_juego ; i++){
      if( comparar_posiciones(juego->posicion_bludgers[i],juego->posicion_jugador) ){               //resta resistencia  y borra bludgers.
        (juego->resistencia)-=GOLPE_DE_BLUDGER;
        pos=i;
        choco=true;
      }
    }

    if(choco){
      for(int i=pos; i< ((juego->bludgers_en_juego)-1); i++)
        juego->posicion_bludgers[i]=juego->posicion_bludgers[i+1];                       //borra bludgers.
        (juego->bludgers_en_juego)--;
    }
  }

}


// se fija si hay que cambiar el estado de juego o no.
void revisar_estado (juego_t *juego,char campo[FILAS_CAMPO][COLUMNAS_CAMPO]){

	if(juego->resistencia <= 0){
 		juego->estado=JUGADOR_PERDIO;
  }

  if( comparar_posiciones(juego->posicion_jugador, juego->posicion_snitch) ){
    juego->estado=JUGADOR_GANO;                                              //si se agoto la resistencia y atrapo snitch igual gano.
  }

  if(juego->casa == RAVENCLAW){
    for(int i=-1 ; i <= 1; i++){
      for(int j=-1; j <= 1; j++){
        if( esta_en_campo( ((juego->posicion_jugador.fila)+i) , ((juego->posicion_jugador.columna)+j)) ){
          if(campo[(juego->posicion_jugador.fila)+i][(juego->posicion_jugador.columna)+j]==SNITCH)      //habilidad de ravenclaw(olfebreria)
            juego->estado=JUGADOR_GANO;
        } 
      }   
    }
  }

  campo[juego->posicion_jugador.fila][juego->posicion_jugador.columna]=JUGADOR;  //fix "temporal" para que no se sobreescriban
  campo[juego->posicion_snitch.fila][juego->posicion_snitch.columna]=SNITCH;    

}


//dar valores validos a la memoria.
void limpiar_campo(char campo[FILAS_CAMPO][COLUMNAS_CAMPO]){
  for (int i=0; i < FILAS_CAMPO ; i++){
    for(int j=0 ;j < COLUMNAS_CAMPO; j++){                                                            
      campo[i][j]=' ';
    }
  }
}


//todas las "inicializar" a continuacion hacen lo mismo, cargan el campo y el estado de juego.
void inicializar_jugador(char campo[FILAS_CAMPO][COLUMNAS_CAMPO],char casa, juego_t *juego){

  juego->posicion_jugador.fila=FILA_INICIAL_JUGADOR;                                            
  juego->posicion_jugador.columna=COLUMNA_INICIAL_JUGADOR;
  juego->resistencia=RESISTENCIA_INICIAL;
  juego->casa=casa;
  campo [juego->posicion_jugador.fila][juego->posicion_jugador.columna]=JUGADOR;

}


void inicializar_bludgers(char campo[FILAS_CAMPO][COLUMNAS_CAMPO],juego_t *juego){

  juego->bludgers_en_juego=CANTIDAD_BLUDGERS_INICIAL;
  juego->direccion=SENTIDO_DERECHO;
  int j=0;

  for(int i=0; i < CANTIDAD_BLUDGERS_INICIAL ; i++){                                                //inicializar bludgers
    juego->posicion_bludgers[i].fila=(FILA_BLUDGER1+j);
    juego->posicion_bludgers[i].columna=0;
    campo [(juego->posicion_bludgers[i].fila)][0]=BLUDGER;
    j+=DISTANCIA_ENTRE_FILAS_BLUDGERS;
  } 

}


void inicializar_dementores(char campo[FILAS_CAMPO][COLUMNAS_CAMPO], juego_t *juego){

  juego->dementores_en_juego=CANTIDAD_DEMENTORES_INICIAL;

  for(int i=0; i < CANTIDAD_DEMENTORES_INICIAL; i++){
    juego->posicion_dementores[i].fila=FILAS_DEMENTORES[i];
    juego->posicion_dementores[i].columna=COLUMNAS_DEMENTORES[i];
    campo [juego->posicion_dementores[i].fila][juego->posicion_dementores[i].columna]=DEMENTOR;
  }

}


void inicializar_snitch(char campo[FILAS_CAMPO][COLUMNAS_CAMPO],juego_t *juego){
  juego->posicion_snitch = posicion_inicial_snitch();
  campo [juego->posicion_snitch.fila][juego->posicion_snitch.columna]=SNITCH;     

}


//si es necesario,cambia la direccion con la que se mueven TODAS las bludgers.
void cambio_direccion(char campo[FILAS_CAMPO][COLUMNAS_CAMPO],juego_t *juego){

  if( (juego->direccion==SENTIDO_DERECHO) && (juego->bludgers_en_juego >= 1) ){
    if( juego->posicion_bludgers[0].columna == (COLUMNAS_CAMPO-1) ){          //movimiento solidario para todas las bludgers, si una se mueve derecha  todas lo hacen.
      juego->direccion=SENTIDO_IZQUIERDO;
    }   
  }

  if( (juego->direccion == SENTIDO_IZQUIERDO) && (juego->bludgers_en_juego >= 1 ) ){
    if(juego->posicion_bludgers[0].columna == 0){                         
      juego->direccion=SENTIDO_DERECHO;
    }   
  }
}

//recibe int
bool esta_en_campo(int fila,int col){
  bool estado=true;
  if( (fila >= (FILAS_CAMPO) ) || (fila < 0) ){
    estado=false;
  }

  if( (col>=COLUMNAS_CAMPO) || (col < 0) ) {
    estado=false;
  }
    
  return estado;
}


//recibe coordenada
bool coordenada_en_campo(coordenada_t pos){

 return(esta_en_campo (pos.fila, pos.columna ) );

}


bool control_valido(char control){
  if( (control == ARRIBA) || (control == ABAJO) || (control == IZQUIERDA) || (control == DERECHA) )
    return true;

  return false;

}


//devuelve true si esa posicion  de el campo esta "vacio".
bool esta_libre(char campo[FILAS_CAMPO][COLUMNAS_CAMPO], int fila, int columna){

  return ( campo[fila][columna] == ' ') ;

}

//mismo que la anterior pero con coordenadas.
bool coordenada_libre(char campo[FILAS_CAMPO][COLUMNAS_CAMPO], coordenada_t pos) {

  return( campo[pos.fila][pos.columna] == ' ');

}


//carga los vectores con los que se opera en mover_snitch.
void vectores_snitch(coordenada_t movimientos[DIRECCIONES_SNITCH],bool validez[DIRECCIONES_SNITCH],juego_t *juego,char campo[FILAS_CAMPO][COLUMNAS_CAMPO]){
    int prioridad=0;                                                            //orden de movimientos segun enunciado

  for(int j=-1; j<= 1 ; j+=2 ){
    for(int k=-1 ; k<= 1 ; k+=2 ){
      movimientos[prioridad].fila=(juego->posicion_snitch.fila)+j;
      movimientos[prioridad].columna=(juego->posicion_snitch.columna)+k;
      prioridad++;                                                            //la prioridad siempre llega a 4, no se pasa.
    }
  }

  for(int i=0;i<DIRECCIONES_SNITCH;i++){                                      //mi forma de ver si los movimientos son validos o no.
    if( ( coordenada_en_campo(movimientos[i]) ) && ( coordenada_libre (campo, movimientos[i]) ) )
      validez[i]=true;
    else validez[i]=false;
  }

}



//buscar la mejor posicion para la snitch, dentro de las validas
void buscar_mejor_mov_snitch(bool validez[DIRECCIONES_SNITCH],coordenada_t movimientos[DIRECCIONES_SNITCH],coordenada_t *mejor_movimiento,juego_t *juego){
   
   bool hay_disponible=false;    

  for(int i=0;i<DIRECCIONES_SNITCH;i++){
    if( distancia_manhattan(movimientos[i], juego->posicion_jugador) > distancia_manhattan(*mejor_movimiento, juego->posicion_jugador) ){ 
      if( validez[i] ){                                         //va cambiado por la posicion que lo aleja,solo si es valida.
        (*mejor_movimiento)=movimientos[i];                            
        hay_disponible=true;
      }                             
    } 
  }

  if( !hay_disponible )
    (*mejor_movimiento)=juego->posicion_snitch;                           //si no hay posiciones validas, se queda quieto


}


