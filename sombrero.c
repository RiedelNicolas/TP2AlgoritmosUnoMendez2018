#include <stdio.h>
#include "sombrero.h" 

int condicion_de_superatributos (int ,int ,int ,int ,int ) ;                                                                            //declaro funciones
int verificador_rango(int) ;
void pedir_atributo(int* , char[] ) ;

char obtener_casa ()  {
	int fuerza = 0, valor = 0, inteligencia = 0, ingenio = 0, dedicacion = 0 ;                                                         //declaro mis variables
      char casa;
     
     do{
     	printf ("\nIngrese el valor de cada atributo del alumno \n");
     	printf ("en el caso que se asignen mas de 2 superatributos se reiniciaria el proceso \n");                                    //instrucciones para el user
     	printf ("los atributos a ingresar tienen que ser numeros del 1 al 10\n");

          pedir_atributo(&fuerza, "fuerza" );
          pedir_atributo(&valor, "valor");
          pedir_atributo(&inteligencia, "inteligencia");
          pedir_atributo(&ingenio, "ingenio");
          pedir_atributo(&dedicacion, "dedicacion");

     	if (condicion_de_superatributos(fuerza ,valor ,inteligencia ,ingenio ,dedicacion)){
     		printf("se ingresaron mas de 2 superatributos, se volvera a ejecutar el programa.\n");                                  // un pequeño aviso para que el user sepa porque se vuelve a iniciar
     	} 

          }while(condicion_de_superatributos(fuerza ,valor ,inteligencia ,ingenio ,dedicacion));                                        //si hay mas de 2 superatributos se repite el ciclo

     //post: condicinales que van a decirme a que casa pertenece, solo si la data ingresada es correcta.
     if ((fuerza > 8)&&(valor > 8)&&(ingenio < 5)){                                                                
     	printf ("ja otro Weasley , Gryffindor !!! \n");

          casa='G';

     }
     else if ((inteligencia > 8)&&(ingenio > 8)){
     	printf ("Tu casa es Ravenclaw \n");

          casa='R';

     }
     else if((inteligencia > 5)&&(ingenio > 8)&&(inteligencia < 8)){
     	printf ("veo sed para probar que eres digno... Slytherin \n");
          
          casa='S';
     }
     else{
     	printf ("condicional!!, perdon quise decir  Hufflepuff \n");
          casa='H';
          
     }

     return casa;

}

//post: la funcion  checkea que no haya mas de 2 superatributos.
int condicion_de_superatributos (int atributo_1 ,int atributo_2 ,int atributo_3 ,int atributo_4 ,int atributo_5) {  
     int contador_de_superatributos= 0 ;   
     if (atributo_1 > 8)
          contador_de_superatributos++ ;
    if (atributo_2 > 8 )
          contador_de_superatributos++ ;
    if (atributo_3 > 8 )
          contador_de_superatributos++ ;
     if (atributo_4 > 8 )
          contador_de_superatributos++ ;
     if (atributo_5 > 8 )
          contador_de_superatributos++ ;
          return (contador_de_superatributos <= 2 )?0 : 1;
      
     }

//post:funcion que verifica si esta dentro del rango requerido.
int verificador_rango(int atributo)  {      
return ((atributo <= 10) && (atributo >= 0))? 0 : 1;
}

//post:scanea cada atributo y le asigna el valor a la variable.
void pedir_atributo(int *valor_atributo, char nombre_atributo []) {
     do{
     printf ("ingrese atributo  %s\n" ,nombre_atributo);
     scanf  (" %d",&*valor_atributo);
}while (verificador_rango(*valor_atributo));
}
