#define _POSIX_C_SOURCE 200809L  // Para strdup() y getline
#define _XOPEN_SOURCE
#include <time.h>
#define TIME_FORMAT "%FT%T%z"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "strutil.h"
#include "heap.h"
#include "abb.h"
#include "hash.h"

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
bool agregar_archivo(char* nombre_archivo){
	
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
