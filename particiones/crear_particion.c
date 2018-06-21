#define _POSIX_C_SOURCE  200809L
#define _XOPEN_SOURCE
#define TIME_FORMAT "%FT%T%z"
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "strutil.h"
#include "heap.h"
#define TAM_MAX_LINEA 135




 
/*STRING TO TIME_T*/
time_t iso8601_to_time(const char* iso8601){
    struct tm bktime = { 0 };
    strptime(iso8601, TIME_FORMAT, &bktime);
    return mktime(&bktime);
}

/*STRUCT RESGISTRO_T*/
typedef struct registro{
 char linea[TAM_MAX_LINEA];
 size_t n_particion;
}registro_t;

/*FUNCION COMPARACION IPS*/
int comparar_ips(char* ip1,char* ip2){
    int flag = 0;
    char** ip1_partes = split(ip1, '.');
    int ip1_campos[4];
    ip1_campos[0] = atoi(ip1_partes[0]);
    ip1_campos[1] = atoi(ip1_partes[1]);
    ip1_campos[2] = atoi(ip1_partes[2]);
    ip1_campos[3] = atoi(ip1_partes[3]);
    char** ip2_partes = split(ip2, '.');
    int ip2_campos[4];
    ip2_campos[0] = atoi(ip2_partes[0]);
    ip2_campos[1] = atoi(ip2_partes[1]);
    ip2_campos[2] = atoi(ip2_partes[2]);
    ip2_campos[3] = atoi(ip2_partes[3]);

    for(int i = 0; i < 4; i++){
        if(ip1_campos[i] > ip2_campos[i]){
            flag = 1;
            break;
        }

        if(ip1_campos[i] < ip2_campos[i]){
            flag = -1;
            break;
        }
    }
    free_strv(ip1_partes);
    free_strv(ip2_partes);

    return flag;
}

/*CREAR REGISTRO*/
registro_t* crear_registro(char* linea, size_t n_particion){
  registro_t* registro = calloc(1,sizeof(registro_t));
  if(!registro){
    return NULL;
  }
  strcpy(registro->linea,linea);
  registro->n_particion = n_particion;
  return registro;
}

/*COMPARACION DE DOS LINEAS -- FUNCIONA CORRECTAMENTE*/
int comparar_lineas(char* linea1, char* linea2){
  char** campos1 = split(linea1,'	');
  char** campos2 = split(linea2,'	');

  time_t tiempo1 = iso8601_to_time(campos1[1]);
  time_t tiempo2 = iso8601_to_time(campos2[1]);
  if(difftime(tiempo1,tiempo2) > 0){
    return 1;
  }
  if(difftime(tiempo1,tiempo2) < 0){
    return -1;
  }
  size_t resultado = comparar_ips(campos1[0],campos2[0]);
  if(resultado == 1){
    return 1;
  }
  if(resultado == -1){
    return -1;
  }
  return strcmp(campos1[3],campos2[3]);
}

int func_comp(const void* a, const void* b){
 	return comparar_lineas((char*)a,(char*)b);
}

/*crea una particion*/
bool crear_particion(FILE* archivo_log, size_t tam_particion, size_t n_particion,char* nombre_salida){ //Crear comparador
  FILE* archivo_salida = fopen(nombre_salida,"wb");
  if(!archivo_salida){
    return false;
  }
  heap_t* heap = heap_crear(func_comp);
  if(!heap){
    return false;
  }
  char* linea_archivo=NULL;
  size_t capacidad =0,leidos=0;
  for(int i = 0 ; i < tam_particion ; i++){
    leidos = getline(&linea_archivo,&capacidad,archivo_log);
    if(leidos != -1){
      linea_archivo[leidos-1] = '\0';
      char* linea =  strdup(linea_archivo);
      heap_encolar(heap,linea);
    }else{
      break;
    }
  }
  /* cambio lista por heap? */

  /*ordenar lista, crear funcion de comparacion */
  /*supongo que tengo lista ordenada*/

  while(!heap_esta_vacio(heap)){
    char* linea = heap_desencolar(heap);
    registro_t* registro = crear_registro(linea,n_particion);
    fwrite(registro,sizeof(registro_t),1,archivo_salida);
    free(registro);
    free(linea);
  }
  fclose(archivo_salida);
  free(linea_archivo);
  heap_destruir(heap,NULL);
  return true;
}
int main(){

  FILE* archivo = fopen("access001.log","r");
  crear_particion(archivo,2000,1,"archivo_salida.log");
  fclose(archivo);


/*
  char* linea1= "66.249.73.135	2015-05-17T10:05:15+00:00	GET	/blog/tags/muaaaaaan";
  char* linea2 = "66.249.73.135	2015-05-17T10:05:15+00:00	GET	/blog";

  int resultado = comparar_lineas(linea1,linea2);
  if(resultado == 1){
    printf("linea 1 es mas grande\n");
  }
  else if(resultado == -1){
    printf("linea 2 es mas grande\n" );
  }
  else{
    printf("son iguales\n" );
  }
  */



}
