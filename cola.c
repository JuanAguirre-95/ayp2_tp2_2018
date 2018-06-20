#include "cola.h"
#include <stdio.h>
#include <stdlib.h>

struct nodo;
typedef struct nodo nodo_t;
nodo_t* nodo_crear(void* valor);
//************************************************************************

//Definicion del struct cola
struct cola{
	nodo_t* prim;
	nodo_t* ult;
};
//Definicion del struct nodo
struct nodo{
	void* dato;
	nodo_t* prox;
};

//************************************************************************
cola_t* cola_crear(void){
	cola_t* cola = malloc(sizeof(cola_t));
	if(cola==NULL)
		return NULL;	
	cola->prim = NULL;
	cola->ult = NULL;
	return cola;
}
void cola_destruir(cola_t *cola, void destruir_dato(void*)){

	while (!cola_esta_vacia(cola)){
		if(destruir_dato != NULL)
			destruir_dato(cola_desencolar(cola));
		else
			cola_desencolar(cola);
	}
	free(cola);
}
bool cola_esta_vacia(const cola_t *cola){
	return cola->prim == NULL && cola->ult == NULL;
}
bool cola_encolar(cola_t *cola, void* valor){
	nodo_t* nodo = nodo_crear(valor);
	if(nodo == NULL)
		return false;
	if(cola_esta_vacia(cola)){
		cola->prim = nodo;
		cola->ult = nodo;
		return true;
	}
	cola->ult->prox = nodo;
	cola->ult = nodo;
	return true;
	
}
void* cola_ver_primero(const cola_t *cola){
	return cola_esta_vacia(cola)?NULL:cola->prim->dato;
}
void* cola_desencolar(cola_t *cola){
	if(cola_esta_vacia(cola))
		return NULL;
	nodo_t* actual = cola->prim;		
	void* valor = actual->dato; 
	cola->prim =actual->prox;
	if(cola->prim == NULL)
		cola->ult= NULL;
	free(actual);
	return valor;
	
	
}
//Crea un nuevo nodo para la cola
//PRE: Recibe un puntero a void para asignarle a dato
//POST: Devuelve un puntero al nuevo nodo creado, o NULL si falla.
nodo_t* nodo_crear(void* valor){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(nodo==NULL)
		return NULL;
	nodo->dato = valor;
	nodo->prox = NULL;
	return nodo;
}
