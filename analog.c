#define _POSIX_C_SOURCE 200809L  // Para strdup() y getline
#define _XOPEN_SOURCE
#include <time.h>
#define TIME_FORMAT "%FT%T%z"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

//HEADERS EXTERNOS
#include "strutil.h"
#include "heap.h"
#include "abb.h"
#include "hash.h"
#include "lista.h"

//CONSTANTES
#define DOS_CANT 5
#define DOS_TIME 2.0


//==========|Structs|============
typedef struct request {
	char* ip;
	time_t hora;
	char* metodo;
	char* recurso;	
}request_t;

//===========|Conversor de tiepo|===========

time_t iso8601_to_time(const char* iso8601){
    struct tm bktime = { 0 };
    strptime(iso8601, TIME_FORMAT, &bktime);
    return mktime(&bktime);
}
//==========|UTILIDADES|===========

//Cuenta la cantidad de elementos dentro de un strv
size_t count_strv(char** strv){
	size_t cont = 0;
	if(!strv)
		return 0;
	while(strv[cont] != NULL)
		cont++;
	return cont;
}

request_t request_crear(char* ip,time_t hora,char* metodo, char* recurso){
	request_t req = malloc(sizeof(request_t));
	if(!req)
		return NULL;
	req->ip = strdup(ip);
	req->hora = hora;
	req->metodo = strdup(metodo);
	req->recurso = strdup(recurso);
}
//==========|Func. Extras para DoS|==========
int comparar_ips(const char* ip1, const char* ip2){
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

//==========|FUNC. PARA ORDENAR ARCHIVO|==========
bool ordenar_archivo(char* nombre_entrada, char* nombre_salida){
	
}

//===========|FUNC PARA AGREGAR_ARCHIVO|==============
bool hash_guardar_tiempos(hash_t* hash_tiempos, const char* ip, time_t tiempo){
	if(!hash_tiempos)
		return false;
	if(hash_pertenece(hash_tiempos,ip)){ //si la ip ya esta guardada
		lista_t* lista_tiempos_ip = hash_obtener(hash_tiempos,ip); //obtengo la lista de tiempos de esa ip
		lista_insertar_ultimo(lista_tiempos_ip,(void*)tiempo); //inserto el nuevo tiempo al final
		return true;
	}
	if(!hash_pertenece(hash_tiempos,ip)){ //Si la ip no pertenece al hash creo lista nueva y la guardo
		lista_t* new_lista = lista_crear();
		lista_insertar_ultimo(new_lista, (void*)tiempo);
		hash_guardar(hash_tiempos,ip,new_lista);
		return true;
	}
	return false;

bool agregar_archivo(char* nombre_archivo){
	if(!nombre_archivo){
		return false;
	}
	FILE* archivo = fopen(nombre_archivo,"r");
	if(!file)
		return false;
	
	hash_t* hash_tiempos = hash_crear(wrapper_lista_destruir);
	if(!hash_tiempos){
		fclose(archivo);
		return false;
	}
	abb_t* arbol_dos = abb_crear(comparar_ips,free);
	if(!arbol_dos){
		hash_destruir(hash_tiempos);
		fclose(archivo)
		return false;
	}
	
	char* linea = NULL; size_t size = 0; ssize_t leidos;
	while((leidos  = getline(&linea,&size,archivo))>0){
		linea[leidos-1] = '\0';
		
		char** acceso = split(linea,'\t');
		char* ip = acceso[0];
		time_t hora = iso8601_to_time(acceso[1]);
		char* metodo = acceso[2];
		char* recurso = acceso[3];
		
		
	}
	
}

//==========|INTERFAZ|===========

void interfaz(char* comando){
	bool exit_flag = false;
	
	char** com_sep = split(comando, ' ');
	char* command = com_sep[0];
	
	if(strcmp(command, "ordenar_archivo") == 0){			//ORDENAR_ARCHIVO
		exit_flag = ordenar_archivo(com_sep[1],com_sep[2]);
		if(exit_flag){
			//free_strv(com_sep);
			fprintf(stdout,"OK\n");
		}
	}
	
	if(strcmp(command, "agregar_archivo") == 0){			//AGREGAR_ARCHIVO
		exit_flag = agregar_archivo(com_sep[1]);
		if(exit_flag){
			//free_strv(com_sep);
			fprintf(stdout,"OK\n");
		}	
	}	
	if(strcmp(command, "ver_visitantes") == 0){			//VER_VISITANTES
		exit_flag = ver_visitantes(com_sep[1],com_sep[2]);
		if(exit_flag){
			//free_strv(com_sep);
			fprintf(stdout,"OK\n");
		}	
	}	
	else{
		fprintf(stderr, "Error en comando %s",command);
	}
	free_strv(com_sep);
}
