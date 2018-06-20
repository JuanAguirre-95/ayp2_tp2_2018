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
#define DOS_CANT 5 //la cantidad de accesos en x tiempo para determinar un DoS
#define DOS_TIME 2.0 //el tiempo entre accesos para determinar un DoS


//==========|Structs|============
typedef struct request { //NO VA A HACER FALTA
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

//Wrapper para lista_destruir
void wrapper_lista_destruir(void* lista){
	lista_t* lista_ = lista;
	lista_destruir(lista_,NULL);
}
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
int comparar_ips(const char* ip1, const char* ip2){		//TODO TRATAR DE MEJORAR ESTA NEGRADA
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
bool ordenar_archivo(size_t M, char* nombre_entrada, char* nombre_salida){
	//TODO	implementar esto!
	printf("ORDENE ARCHIVOS IMAGINARIOS\n");
	return true;
}

//===========|FUNC PARA AGREGAR_ARCHIVO|==============
bool hash_guardar_tiempos(hash_t* hash_tiempos, const char* ip, time_t tiempo){
	if(!hash_tiempos)
		return false;
	if(hash_pertenece(hash_tiempos, ip)){ //si la ip ya esta guardada
		lista_t* lista_tiempos_ip = hash_obtener(hash_tiempos, ip); //obtengo la lista de tiempos de esa ip
		lista_insertar_ultimo(lista_tiempos_ip, (void*)tiempo); //inserto el nuevo tiempo al final
		return true;
	}
	if(!hash_pertenece(hash_tiempos, ip)){ //Si la ip no pertenece al hash creo lista nueva y la guardo
		lista_t* new_lista = lista_crear();
		lista_insertar_ultimo(new_lista, (void*)tiempo);
		hash_guardar(hash_tiempos, ip, new_lista);
		return true;
	}
	return false;
	
}
bool verificar_accesos(lista_t* lista_tiempo){
	lista_iter_t* iter1 = lista_iter_crear(lista_tiempo);
	lista_iter_t* iter2 = lista_iter_crear(lista_tiempo);
	int i = 0;
	while(i<DOS_CANT-1){ //Avanzo el iter2 N posiciones
		lista_iter_avanzar(iter2);
		
		if(lista_iter_al_final(iter2)){
			lista_iter_destruir(iter1);
			lista_iter_destruir(iter2);
			return false;
		}
		i++;
	}
	while(!lista_esta_vacia(lista_tiempo)){
	
		time_t tiempo1 = (time_t)lista_iter_ver_actual(iter1);
		time_t tiempo2 = (time_t)lista_iter_ver_actual(iter2);
		if(difftime(tiempo2,tiempo1) <= DOS_TIME){

			lista_iter_destruir(iter1);
			lista_iter_destruir(iter2);
			return true;
		}
		lista_iter_avanzar(iter1);
		lista_iter_avanzar(iter2);
		if(lista_iter_al_final(iter2))
			break;
		
	}
	lista_iter_destruir(iter1);
	lista_iter_destruir(iter2);
	return false;

}

abb_t* check_dos(hash_t* hash_tiempos){			//Devuelve un arbol con las ips que causaron DoS.
	abb_t* arbol_dos = abb_crear(comparar_ips,free);
	if(!arbol_dos)
		return NULL;
	
	hash_iter_t* iter = hash_iter_crear(hash_tiempos);
	if(!iter){
		abb_destruir(arbol_dos);
		return NULL;
	}
	
	while(!hash_iter_al_final(iter)){	
		const char* ip = hash_iter_ver_actual(iter);
		lista_t* lista = hash_obtener(hash_tiempos,ip);
		if(verificar_accesos(lista)){
			abb_guardar(arbol_dos,ip,NULL);
		}
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
	return arbol_dos;
}
bool imprimir_ip_dos(const char* clave, void* dato,void* extra){
	fprintf(stdout,"DoS: %s\n",clave);
	return true;
}

void imprimir_dos(abb_t* arbol_dos){
	abb_in_order(arbol_dos, imprimir_ip_dos, NULL);
}

bool agregar_archivo(char* nombre_archivo, abb_t* arbol_ips){
	if(!nombre_archivo){
		return false;
	}
	FILE* archivo = fopen(nombre_archivo,"r");
	if(!archivo)
		return false;
	
	hash_t* hash_tiempos = hash_crear(wrapper_lista_destruir);
	if(!hash_tiempos){
		fclose(archivo);
		return false;
	}

	char* linea = NULL; size_t size = 0; ssize_t leidos;
	while((leidos  = getline(&linea,&size,archivo))>0){
		linea[leidos-1] = '\0';
		
		char** acceso = split(linea,'\t');
		
		char* ip = acceso[0];
		time_t hora = iso8601_to_time(acceso[1]);
		//char* metodo = acceso[2];
		//char* recurso = acceso[3];
		
		if(!abb_pertenece(arbol_ips,ip)){
			abb_guardar(arbol_ips,ip,NULL);
		}
		hash_guardar_tiempos(hash_tiempos,ip,hora);
		
		free_strv(acceso);
	}
	abb_t* arbol_dos = check_dos(hash_tiempos);
	if(!arbol_dos){
		hash_destruir(hash_tiempos);
		fclose(archivo);
		free(linea);
		return false;
	}
	if(abb_cantidad(arbol_dos) != 0){
		imprimir_dos(arbol_dos);
	}
	
	hash_destruir(hash_tiempos);
	abb_destruir(arbol_dos);
	free(linea);
	fclose(archivo);
	return true;
	
}
//==========|FUNC PARA VER VISITANTES|===========
bool imprimir_ip_rango(const char* clave, void* dato,void* extra){
	char** rango = extra;
	if(!(comparar_ips(clave,rango[0])<0) && !(comparar_ips(clave,rango[1])>0)) //si comparar ips devuelve 0 y esta dentro de rango imprime
		printf("\t%s\n",clave);
	return true;
}

bool ver_visitantes(abb_t* arbol_ips, char* desde, char* hasta){
	if(abb_cantidad(arbol_ips) == 0){
		return false;
	}
	
	char** rango = malloc(sizeof(char*)*2);
	rango[0] = desde;
	rango[1] = hasta; 
	printf("Visitantes:\n");
	abb_in_order(arbol_ips, imprimir_ip_rango,(void*)rango);
	//fprintf(stdout,"OK\n" );
	free(rango);
	return true;
}


//==========|INTERFAZ|===========

bool interfaz(char** comando, size_t M, abb_t* arbol_ips){
	bool exit_flag = false;
	//printf("Arranco\n");
	//char** com_sep = split(comando, ' ');
	size_t cant_args = count_strv(comando);
	char* command = comando[0];

	if(strcmp(command, "ordenar_archivo") == 0){					//ORDENAR_ARCHIVO
		if(cant_args != 3){
			return exit_flag;
		}
		exit_flag = ordenar_archivo(M,comando[1],comando[2]);
	}
	
	if(strcmp(command, "agregar_archivo") == 0){					//AGREGAR_ARCHIVO
		if(cant_args != 2){
			return exit_flag;
		}
		exit_flag = agregar_archivo(comando[1],arbol_ips);
	}	
	if(strcmp(command, "ver_visitantes") == 0){					//VER_VISITANTES
		if(cant_args != 3){
			return exit_flag;
		}
		exit_flag = ver_visitantes(arbol_ips,comando[1],comando[2]);
	}	
	
	//abb_destruir(arbol_ips);
	return exit_flag;
}
