#ifndef LISTA_H
#define LISTA_H
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La lista enlazada esta planteada como una cola de punteros genéricos. */

typedef struct lista lista_t;
typedef struct lista_iter lista_iter_t;
/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

// Crea una lista
// Post: devuelve una nueva lista vacía, retorna NULL si la lista no se pudo crear

lista_t *lista_crear(void); 

// Devuelve verdadero o falso, según si la fila tiene o no elementos
// Pre: la fila fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento a la lista . Devuelve falso en caso de error.
// Pre: la cola fue creada.
// Post: se agregó un nuevo elemento al principio de la lista
bool lista_insertar_primero(lista_t *lista, void *dato); 

// Agrega un nuevo elemento a la lista . Devuelve falso en caso de error.
// Pre: la cola fue creada.
// Post: se agregó un nuevo elemento al final de la lista
bool lista_insertar_ultimo(lista_t *lista, void *dato);


// Saca el primer elemento de la lista. Si la listatiene elementos, se quita el
// primero de la lista, y se devuelve su valor, si está vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista); 

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

//Devuelve el largo de la lista
//Pre: la lista fue creada
//Post: Se devolvio el largo de la lista, si esta vacia devuelve 0
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));//LISTO

//ITERADOR INTERNO

//Se crea un iterador interno que recorre la lista
//Pre: Existe una lista a recorrer
//Post: Recorre la lista aplicandole la funcion visitar hasta que esta devuelva false
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

//ITERADOR EXTERNO

//Crea un iterador
//Pre: Recibe una lista a iterar
//Post: Se crea el iterador vinculado a la lista, retorna NULL si el iterador no se puede crear
lista_iter_t *lista_iter_crear(lista_t *lista);

//Pre: Recibe un iterador creado
//Post: Avanza de a un elemento en la lista
//devuelve false si llego al final de la lista
bool lista_iter_avanzar(lista_iter_t *iter);


//Pre: Recibe un iterador creado
//Post: Muestra el elemento actual donde el iterador esta posicionado
void *lista_iter_ver_actual(const lista_iter_t *iter);

//Pre: Recibe un iterador creado
//Post: Devuelve true si el iterador esta al final de la lista
bool lista_iter_al_final(const lista_iter_t *iter);

//Pre: Recibe un iterador creado
//Post: Destruye la estructura iterador
void lista_iter_destruir(lista_iter_t *iter);
//Pre: recibe un iterador creado y un elemento a insertar
//Post: Lo inserta en la posicion donde el iterador se encuentra
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

//Pre: recibe un iterador creado
//Post: Elimina el elemento donde el iterador fue creado
//queda apuntando al proximo elemento
void *lista_iter_borrar(lista_iter_t *iter);

//PRUEBAS ALUMNO
void pruebas_lista_alumno(void);

#endif // LISTA_H
