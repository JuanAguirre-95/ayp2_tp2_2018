#include "lista.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct nodo{
	void* dato;
	struct nodo* nodo_sig;
}nodo_t;

struct lista{
	nodo_t* nodo_prim;
	nodo_t* nodo_ult;
	size_t largo;
};
struct lista_iter{
	nodo_t* act;
	nodo_t* ant;
	lista_t* lista;
};

//FUNCIONES AUXILIARES.

nodo_t* nodo_crear(void* dato){
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if(!nodo){
		return NULL;
	}
	nodo->nodo_sig = NULL;
	nodo->dato = dato;
	return nodo;
}

//La utilizo en iterador
//Pre: Existe una lista
//Post: Elimina ultimo elemento
void* lista_borrar_final(lista_t* lista){
	nodo_t* nodo = lista->nodo_prim;
	while(nodo->nodo_sig != lista->nodo_ult){
		nodo = nodo->nodo_sig;
	}
	nodo_t* aux =lista->nodo_ult;
	void* dato = aux->dato;
	lista->nodo_ult = nodo;
	lista->largo--;
	free(aux);
	return dato;
}


//PRIMITIVAS DE LA LISTA.

lista_t *lista_crear(void){
	lista_t* lista = malloc(sizeof(lista_t));
	if(!lista){
		return NULL;
	}
	lista->nodo_prim = NULL;
	lista->nodo_ult = NULL;
	lista->largo = 0;
	return lista;
}

bool lista_esta_vacia(const lista_t *lista){
	return (lista->largo == 0);
}

bool lista_insertar_primero(lista_t *lista, void *dato){
	nodo_t* nodo = nodo_crear(dato);
	if(!nodo){
		return false;
	}
	if(lista_esta_vacia(lista)){
		lista->nodo_ult = nodo;
		lista->largo = 1;
	}else{
		nodo->nodo_sig = lista->nodo_prim;
		lista->largo++;
	}
	lista->nodo_prim = nodo;
	return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
	nodo_t* nodo = nodo_crear(dato);
	if(!nodo){
		return false;
	}
	if(lista_esta_vacia(lista)){
		lista->nodo_prim = nodo;
		lista->largo = 1;
	}else{
		lista->nodo_ult->nodo_sig = nodo;
		//VER 
		lista->largo ++;
	}
	lista->nodo_ult = nodo;
	return true;
}

void *lista_borrar_primero(lista_t *lista){
	if(lista_esta_vacia(lista)){
		return NULL;
	}
	nodo_t* nodo_aux = lista->nodo_prim;
	void* dato = nodo_aux->dato;
	lista->nodo_prim = nodo_aux->nodo_sig;
	if(lista->largo == 1){
		lista->nodo_ult = NULL;		
	}
	free(nodo_aux);
	lista->largo--;
	return dato;
}

void *lista_ver_primero(const lista_t *lista){
	if(lista_esta_vacia(lista)){
		return NULL;
	}
	return lista->nodo_prim->dato;
}

void *lista_ver_ultimo(const lista_t* lista){
	if(lista_esta_vacia(lista)){
		return NULL;
	}
	return lista->nodo_ult->dato;
}

size_t lista_largo(const lista_t *lista){
	return lista->largo;
}

void lista_destruir(lista_t *lista, void destruir_dato(void *)){	
	while(!lista_esta_vacia(lista)){
		void* dato = lista_borrar_primero(lista);
		if(destruir_dato){
			destruir_dato(dato);		
		}		
	}	
	free(lista);
}	


//iterador interno

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
	nodo_t* actual = lista->nodo_prim;
	while(actual){
		if(!visitar(actual->dato,extra)){
			return;
		}
		actual = actual->nodo_sig;
	}
}
//ITERADOR EXTERNO.

lista_iter_t *lista_iter_crear(lista_t *lista){
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if (!iter) {
        return NULL;
    }
    iter->ant = NULL;
    iter->lista = lista;	
	iter->act = lista->nodo_prim;
	return iter;
}
 
bool lista_iter_al_final(const lista_iter_t *iter){	
	return (!iter->act);
}
void *lista_iter_ver_actual(const lista_iter_t *iter){	
	if (iter->act)
		return iter->act->dato;
	return NULL;
}
void lista_iter_destruir(lista_iter_t *iter){	
	free(iter);
}

bool lista_iter_avanzar(lista_iter_t *iter){	
	if(lista_iter_al_final(iter))		
		return false;
	iter->ant = iter->act;
	iter->act = iter->act->nodo_sig;
	return true;
}

bool esta_primero(lista_iter_t *iter){
	return (!iter->ant);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
	if(lista_iter_al_final(iter)){
		lista_insertar_ultimo(iter->lista,dato);
		iter->act = iter->lista->nodo_ult;										
	}
	else if(esta_primero(iter)){		
		lista_insertar_primero(iter->lista,dato);
		iter->act = iter->lista->nodo_prim;		
	}
	else{
		nodo_t* nodo_nuevo = nodo_crear(dato);
		iter->ant->nodo_sig = nodo_nuevo;
		nodo_nuevo->nodo_sig = iter->act;
		iter->act = iter->ant->nodo_sig;
		iter->lista->largo++;
	}
	return iter->act;
}

void *lista_iter_borrar(lista_iter_t *iter){
	if(lista_iter_al_final(iter) || lista_esta_vacia(iter->lista)){
		return NULL;
	}
	void* dato = iter->act->dato;
	if(esta_primero(iter) && !lista_esta_vacia(iter->lista)){		
		lista_borrar_primero(iter->lista);		
		iter->act = iter->lista->nodo_prim;		
	}else if(iter->act->nodo_sig == NULL){		
		lista_borrar_final(iter->lista);
		iter->act = NULL;
	}else{
		iter->ant->nodo_sig = iter->act->nodo_sig;
		nodo_t* aux = iter->act;
		iter->act = iter->act->nodo_sig;
		dato = aux->dato;
		free(aux);
		iter->lista->largo--;
	}
	return dato;
}
