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
  size_t resultado = comparar_ips(campos1[0],campos2[0]);
  int retorno;

  time_t tiempo1 = iso8601_to_time(campos1[1]);
  time_t tiempo2 = iso8601_to_time(campos2[1]);
  if(difftime(tiempo1,tiempo2) > 0){
    retorno = 1;
  }
  else if(difftime(tiempo1,tiempo2) < 0){
    retorno = -1;
  }
  else if(resultado == 1){
    retorno = 1;
  }
  else if(resultado == -1){
    retorno = -1;
  }
  else retorno = strcmp(campos1[3],campos2[3]);
  free_strv(campos1);
  free_strv(campos2);
  return  -1*retorno;
}

int func_comp(const void* a, const void* b){
 	return comparar_lineas((char*)a,(char*)b);
}
int func_comp_registro(const void* a, const void* b){
  registro_t* registro_1= (registro_t*)a;
  registro_t* registro_2= (registro_t*)b;
 	return comparar_lineas(registro_1->linea,registro_2->linea);

}
/*crea una particion*
retorna la cantidad de lineas leidas*/
size_t crear_particion(FILE* archivo_log, size_t tam_particion, size_t n_particion,char* nombre_salida){
  size_t total_lineas = 0;
  heap_t* heap = heap_crear(func_comp);
  if(!heap){
    return -1;
  }
  char* linea_archivo=NULL;
  size_t capacidad =0,leidos=0;
  for(int i = 0 ; i < tam_particion ; i++){
    leidos = getline(&linea_archivo,&capacidad,archivo_log);
    if(leidos != -1){
      total_lineas++;
      linea_archivo[leidos-1] = '\0';
      char* linea =  strdup(linea_archivo);
      heap_encolar(heap,linea);
    }else{
      break;
    }
  }
  FILE* archivo_salida = fopen(nombre_salida,"wb");
  if(!archivo_salida){
    return -1;
  }
  while(!heap_esta_vacio(heap)){
    char* linea = heap_desencolar(heap);
    registro_t* registro = crear_registro(linea,n_particion);
    if(!registro){
      return -1;
    }
    fwrite(registro,sizeof(registro_t),1,archivo_salida);
    free(registro);
    free(linea);
  }
  fclose(archivo_salida);
  free(linea_archivo);
  heap_destruir(heap,NULL);
  return total_lineas;
}

/*memoria disponibles son kb por eso multiplico por mil
retorna la cantidad de particiones que hubo*/
size_t crear_particiones(char* nombre_archivo, size_t memoria_disponible){
  size_t cant_lineas = (memoria_disponible*1000)/TAM_MAX_LINEA;
  FILE* archivo = fopen(nombre_archivo,"r");
  if(!archivo){
    return -1 ;
  }
  char nombre_archivo_salida[15];
  size_t cant_particiones = 1;
  size_t lineas_guardadas;
  while(!feof(archivo)){
    sprintf(nombre_archivo_salida,"archivo%i.log",cant_particiones);
    lineas_guardadas = crear_particion(archivo,cant_lineas,cant_particiones,nombre_archivo_salida);
    if(lineas_guardadas > 0){
      cant_particiones++;
    }else{
      /*por como trabaja feof se ejecuta una vez de mas el while y
      se crea la misma particion final, por eso borro el ultimo archivo*/
      remove(nombre_archivo_salida);
      break;
    }
  }
  fclose(archivo);
  return cant_particiones;
}

int main(){

  /*AGRER VALIDACIONES DE ABRIR ARCHIVO Y MALLOCS*/
   char nombre_archivo_particion[15];
   size_t cant_particiones =  crear_particiones("access001.log", 10);
   printf(" Se crearon %i\n", cant_particiones);

  heap_t* heap = heap_crear(func_comp_registro);
   FILE** archivos = calloc(cant_particiones,sizeof(FILE*));




   for(int i = 0 ; i < cant_particiones-1 ; i++){
      sprintf(nombre_archivo_particion,"archivo%i.log",i+1);
      FILE* archivo = fopen(nombre_archivo_particion,"rb");
      archivos[i] = archivo;

      registro_t* registro = malloc(sizeof(registro_t));
      fread(registro,sizeof(registro_t),1,archivo);

      heap_encolar(heap,registro);
   }


   FILE* salida = fopen("archivo_salida.log", "w");

   while(!heap_esta_vacio(heap)){

     registro_t* registro = heap_desencolar(heap);
     //printf("se desencolo: %s\n",registro->linea);

     fprintf(salida,"%s\n",registro->linea);

     size_t proviene = registro->n_particion;
     size_t resultado = fread(registro,sizeof(registro_t),1,archivos[proviene-1]);
     if(resultado > 0){
       heap_encolar(heap,registro);
     }

   }




 }
