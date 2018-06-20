#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "heap.h"




int func_comp(const void* valor_1, const void* valor_2){
	return(*(int*) valor_1 - *(int*)valor_2);	
}


void pruebas_heap_vacio(){
	heap_t* heap = heap_crear(func_comp);
	print_test("Se creo heap vacio",heap_esta_vacio(heap) == true);
	print_test("Cantidad de elementos debe ser cero",heap_cantidad(heap) == 0);
	print_test("Ver max debe ser NULL",heap_ver_max(heap) ==NULL);
	print_test("Desencolo debe ser falso",heap_desencolar(heap) ==NULL);
	heap_destruir(heap,NULL);
}

void pruebas_heap(){

	printf("-----PRUEBAS BASICAS-----\n");
	int uno =1;
	int dos =2;
	int tres =3;
	int cuatro =4;
	int cuarenta= 40;
	int cien=100;
	

	heap_t* heap = heap_crear(func_comp);
	print_test("Se creo heap vacio",heap_esta_vacio(heap) == true);
	print_test("Cantidad de elementos debe ser cero",heap_cantidad(heap) == 0);
	print_test("Ver max debe ser NULL",heap_ver_max(heap) ==NULL);
	print_test("Encolar 3",heap_encolar(heap,&tres) ==true);
	print_test("Encolar 1",heap_encolar(heap,&uno) ==true);
	print_test("Ver max debe ser 3",heap_ver_max(heap) == &tres);
	print_test("Encolar 3",heap_encolar(heap,&tres) ==true);
	print_test("Encolar 2",heap_encolar(heap,&dos) ==true);	
	print_test("Encolar 40",heap_encolar(heap,&cuarenta) ==true);
	print_test("Ver max debe ser 40",heap_ver_max(heap) ==&cuarenta);
	print_test("Encolar 100",heap_encolar(heap,&cien) ==true);
	print_test("Encolar cuatro",heap_encolar(heap,&cuatro) ==true);	
	print_test("Cantidad de elementos debe ser 7",heap_cantidad(heap) ==7);
	print_test("Desencolo debe ser 100",heap_desencolar(heap) == &cien);
	print_test("Desencolo debe ser 40",heap_desencolar(heap) == &cuarenta);
	print_test("Desencolo debe ser 4",heap_desencolar(heap) == &cuatro);
	print_test("Desencolo debe ser 3",heap_desencolar(heap) == &tres);
	print_test("Desencolo debe ser 3",heap_desencolar(heap) == &tres);
	print_test("Desencolo debe ser 2",heap_desencolar(heap) == &dos);
	print_test("Desencolo debe ser 1",heap_desencolar(heap) == &uno);
	print_test("Ver max debe ser NULL",heap_ver_max(heap) ==NULL);
	print_test("Cantidad de elementos debe ser 0",heap_cantidad(heap) ==0);
	print_test("Heap vacio",heap_esta_vacio(heap) == true);
	heap_destruir(heap,NULL);


}
void pruebas_heap_arr(){	
	printf("-------PRUEBAS HEAP ARR--------\n");
	int uno =1;
	int dos =2;
	int tres =3;
	int cuarenta= 40;
	int cien=100;
	int quince =15;
	int diez = 10;
	size_t tam = 7;
	void* arreglo[8] ={&uno,&cien,&dos,&tres,&cuarenta,&quince,&diez};
	heap_t* heap = heap_crear_arr(arreglo,tam,func_comp);
	print_test("Cantidad de elementos debe ser 7",heap_cantidad(heap) == 7);
	print_test("Ver max debe ser 100",heap_ver_max(heap) ==&cien);
	print_test("Encolar 1",heap_encolar(heap,&uno) ==true);
	print_test("Desencolo debe ser 100",heap_desencolar(heap) == &cien);
	print_test("Desencolo debe ser 40",heap_desencolar(heap) == &cuarenta);
	print_test("Cantidad de elementos debe ser 6",heap_cantidad(heap) ==6);
	print_test("Ver max debe ser 15",heap_ver_max(heap) ==&quince);
	print_test("Heap vacio debe ser falso",heap_esta_vacio(heap) == false);
	int mil = 1000;
	int nueve = 9;
	print_test("Encolar 1000",heap_encolar(heap,&mil) ==true);
	print_test("Encolar 9",heap_encolar(heap,&nueve) ==true);
	print_test("Ver max debe ser 1000",heap_ver_max(heap) ==&mil);
	print_test("Cantidad de elementos debe ser 8",heap_cantidad(heap) ==8);

	heap_destruir(heap,NULL);
}


void prueba_sort(){
	printf("-----PRUEBAS HEAP SORT----\n");
	int uno = 1;
	int dos = 2;
	int tres = 3;
	int cuarenta = 40;
	int cien =100;
	int quince = 15;
	int diez = 10;
	bool resultado = true;
	size_t tam = 7;
	void* arreglo[7] ={&quince,&dos,&uno,&diez,&tres,&cien,&cuarenta};
	heap_sort(arreglo,tam,func_comp);
	int * dato_ant = arreglo[0];
	int* dato_actual;
	for(size_t i=0;i<7;i++){
		dato_actual = arreglo[i]	;	
		if(*dato_actual < *dato_ant){			
			resultado= false;
		 	break;

		}
		dato_ant = dato_actual;
	}
	print_test("Los elementos se ordenaron correcamente",resultado);
}


void pruebas_volumen(size_t tam){
	printf("-----PRUEBAS DE VOLUMEN HEAP-----\n");
	
	size_t elementos_encolados = 0;
	heap_t* heap = heap_crear(func_comp);
	int datos[tam];
	bool resultado_encolar = false;
	srand((unsigned int)time(NULL));
	bool resultado_desencolar= true;
	bool resultado_max = true;	
	
	for(size_t i = 0; i<tam ;i++){
		int dato =  rand() % 1001; 		
		datos[i]= dato;					
	}
	for(int i = 0; i<tam; i++){
		resultado_encolar = heap_encolar(heap,&datos[i]);		
		elementos_encolados = i;
		if(resultado_encolar == false){			
			break;
		} 
	}
	print_test("Se encolaron correctamente todos los elementos\n", resultado_encolar);	

	int * dato_ant =  heap_ver_max(heap);
	int* dato_actual;
	for(size_t i=0;i<=elementos_encolados;i++){
		dato_actual = heap_ver_max(heap);			 
		if (heap_desencolar(heap)  != dato_actual){
		 	resultado_desencolar = false;
		 	break;
		}
		if(*dato_actual > *dato_ant){			
			resultado_max= false;
		 	break;

		}
		dato_ant = dato_actual;
		
	}
	print_test("Se desencolaron correctamente todos los elementos\n", resultado_desencolar);		
	print_test("Ver max funciona correctamente\n", resultado_max);		
	print_test("Heap esta vacio",heap_esta_vacio(heap) == true);
	
	heap_destruir(heap,NULL);	
}

void pruebas_volumen_arr(size_t tam){	

	printf("-----PRUEBAS DE VOLUMEN HEAP ARR-----\n");	
	void* arreglo[tam];
	srand((unsigned int)time(NULL));
	bool resultado_desencolar= true;
	bool resultado_max = true;	
	int datos[tam];
	for(size_t i = 0; i<tam ;i++){

		   		
		datos[i]= (rand() % 1001);					
	}
	for(size_t i = 0; i<tam ;i++){		   		
		arreglo[i]= &datos[i];				
	}
	heap_t* heap = heap_crear_arr(arreglo,tam,func_comp);		

	int * dato_ant =  heap_ver_max(heap);
	int* dato_actual;
	for(size_t i=0;i<tam;i++){
		dato_actual = heap_ver_max(heap);		
		if (heap_desencolar(heap)  != dato_actual){
		 	resultado_desencolar = false;
		 	break;
		}
		if(*dato_actual > *dato_ant){			
			resultado_max= false;
		 	break;

		}
		dato_ant = dato_actual;		
	}		
	print_test("Se desencolaron correctamente todos los elementos\n", resultado_desencolar);
	print_test("Ver max funciona correctamente\n", resultado_max);		
	print_test("Heap esta vacio",heap_esta_vacio(heap) == true);
	heap_destruir(heap,NULL);
}



void pruebas_sort_volumen(size_t tam){
	bool resultado = true;
	printf("-----PRUEBAS SORT VOLUMEN----\n");
	void* arreglo[tam];
	srand((unsigned int)time(NULL));	
	int datos[tam];
	for(size_t i = 0; i<tam ;i++){		   		
		datos[i]= (rand() % 1001);					
	}
	for(size_t i = 0; i<tam ;i++){		   		
		arreglo[i]= &datos[i];				
	}
	heap_sort(arreglo,tam,func_comp);

	int * dato_ant = arreglo[0];
	int* dato_actual;
	for(size_t i=0;i<tam;i++){
		dato_actual = arreglo[i]	;	
		if(*dato_actual < *dato_ant){			
			resultado= false;
		 	break;

		}
		dato_ant = dato_actual;
	}
	print_test("Heap sort funciona correcamente",resultado);
}

void destruir_dinamico_heap(){
	printf("-----PRUEBAS DESTRUIR DINAMICO-----\n");
	int* dato1 = malloc(sizeof(int));
	int* dato2= malloc(sizeof(int));
	int* dato3 =  malloc(sizeof(int));
	int* dato4 =  malloc(sizeof(int));	
	*dato1 = 1;
	*dato2 = 2;
	*dato3 = 3;
	*dato4= 4;
	heap_t* heap = heap_crear(func_comp);
	print_test("Encolar dato1",heap_encolar(heap,dato1) ==true);
	print_test("Ver max debe ser dato1",heap_ver_max(heap) == dato1);
	print_test("Encolar dato3",heap_encolar(heap,dato3) ==true);
	print_test("Encolar dato 4",heap_encolar(heap,dato4) ==true);		
	print_test("Encolar dato2",heap_encolar(heap,dato2) ==true);	
	print_test("Ver max debe ser 4",heap_ver_max(heap) ==dato4);	
	print_test("Cantidad de elementos debe ser 4",heap_cantidad(heap) ==4);
	print_test("Desencolo debe ser 4",heap_desencolar(heap) == dato4);
	print_test("Cantidad de elementos debe ser 3",heap_cantidad(heap) ==3);
	print_test("Ver max debe ser 3",heap_ver_max(heap) ==dato3);
	free(dato4);
	heap_destruir(heap,free);
}






void pruebas_heap_alumno(){
	pruebas_heap_vacio();
	pruebas_heap();
	pruebas_heap_arr();
	prueba_sort();
	pruebas_volumen(500);
	pruebas_volumen_arr(500);
	destruir_dinamico_heap();
	pruebas_sort_volumen(500);


}
