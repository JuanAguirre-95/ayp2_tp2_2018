#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
size_t cantidad_palabras_string(const char* str, char sep){
	size_t i =0;
	size_t elementos =0;
	while(str[i] != '\0'){
		if(str[i] == sep){
			elementos++;
		}
		i++;
	}
	return elementos+1;
}
void free_strv(char* strv[]){
	size_t i= 0;
	while(strv[i] != NULL){
		free(strv[i]);
		i++;
	}	
	free(strv); 
}


size_t cantidad_letras_strv( char** strv){
	size_t i = 0;
	size_t contador =0;	
	if(!strv[0] ){
		return 1;
	}
	while(strv[i] != NULL){
		contador = contador+ strlen(strv[i]);
		i++;
	}
	return contador;
}

size_t cantidad_separadores_strv( char** strv){
	size_t contador = 0;	
	while(strv[contador]){
		contador++;
	}
	return contador;
}

char** split(const char* str, char sep){
	if(sep == '\0'){
		return NULL;
	}
	size_t largo = cantidad_palabras_string(str,sep);
	char cadena[strlen(str)];
	char** cadena_split = malloc(sizeof(char*) * (largo+1));
	if(!cadena_split){
		return NULL;
	}
	char caracter_act = str[0];
	size_t indice =0,total =0, cantidad=0;
	while(str[total]!= '\0'){ 
		caracter_act=str[total];
		if(caracter_act != sep){
			cadena[indice] = caracter_act;
			indice++;						
		}else{			
			cadena[indice] = '\0'; // LE AGREGO EL \0 A LA CADENA A ARREGLAR			
			cadena_split[cantidad] = strdup(cadena);
			if(!cadena_split[cantidad]){
				free_strv(cadena_split);
				return NULL;
			}		
			cantidad++;
			indice = 0;
		}
		total++;
	}
	cadena[indice] = '\0';
	cadena_split[cantidad] = strdup(cadena);
	if(!cadena_split[cantidad]){
		free_strv(cadena_split);
		return NULL;
	}	
	cadena_split[cantidad+1] = NULL;
	return cadena_split;	
}






char* join(char** strv, char sep){	
	size_t cantidad_let = cantidad_letras_strv(strv)+ cantidad_separadores_strv(strv);	
	char* cadena = malloc(sizeof(char)* cantidad_let);
	if(!cadena){
		return NULL;
	}	
	size_t i = 0; //cantidad de palabras en strv
	size_t contador =0; //posicion de letra en cadena
	while(strv[i]){		
		size_t len_strvi =	strlen(strv[i])	;	
		for(size_t j =0; j< len_strvi; j++){			
			cadena[contador] = strv[i][j];			
			contador++;
		}
		i++;
		if(strv[i]){				
			cadena[contador] = sep;			
			contador++;
		}else
			cadena[contador]='\0';
	}	
	return cadena;
}





	



	

	


