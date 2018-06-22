#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "strutil.h"
#define M 150

/*DEFINICION DE LA ESTRUCTURA REGISTRO DEBE SER DINAMICA???*/
typedef struct registro{
 char linea[M];  // char linea[TAM_MAX_REGISTRO]???
 size_t n_particion;
}registro_t;


registro_t* registro_crear(char* linea, size_t n_particion){
  registro_t* registro = malloc(sizeof(registro_t));
  if(!registro){
    return NULL;
  }
  strcpy(registro->linea,linea);
  registro->n_particion = n_particion;
  return registro;
}

size_t bytes_ultimo_campo(char** spliteado){
  size_t i = 0;
  while(spliteado[i]){
    i++;
  }
  size_t bytes_campo = strlen(spliteado[i-1]);
  spliteado[i-1] = NULL;
  return bytes_campo;
}



void  crear_particiones(size_t kbs_disponibles){

  size_t byte = kbs_disponibles*1000;
  FILE* archivo = fopen("access001.log","r");
  char* nombre_salida;

  char* linea = malloc(byte);
  size_t leyo;
  size_t proviene = 1;
  while(!feof(archivo)){
    leyo = fread(linea,byte,1,archivo);
    if(leyo == 0){
      break;
    }
    char** spliteado = split(linea,'\n');
    size_t bytes_retroceder =  bytes_ultimo_campo(spliteado);
    fseek(archivo,-1*(bytes_retroceder), SEEK_CUR);
    size_t i = 0;
    FILE* particion = fopen("")
    while(spliteado[i]){

      registro_t* registro = registro_crear(spliteado[i],proviene);
      printf("Se guardo la la linea%s\n", spliteado[i]);
      fwrite(registro,1,sizeof(registro_t),destino);
      i++;
    }
    proviene++;
}



  FILE* leer = fopen("access002.log","rb");
  FILE* guardar = fopen("access002p.log","w");

  registro_t* registro = malloc(sizeof(registro_t));
  size_t leidos;
  while(!feof(leer) ){
    leidos = fread(registro,sizeof(registro_t),1,leer);
    if(leidos == 0){
      break;
    }
    fprintf(guardar,"%s\n",registro->linea);
   printf("La linea del archivo leido es: %s\n",registro->linea );
   printf("Proviene de la particion es: %i\n",registro->n_particion );
  }
}
