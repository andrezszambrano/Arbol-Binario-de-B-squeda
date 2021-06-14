#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "abb.h"

#define ERROR -1
#define EXITO 0
#define POSICION_IZQUIERDA 2
#define POSICION_DERECHA 3
#define AUX_BUSCAR -4
#define AUX_BORRAR -3
#define HIJOS_MAXIMOS 2
#define AUXILIAR_DERECHO 4
#define AUXILIAR_IZQUIERDO -2
#define AUXILIAR_AMBOS_VACIOS 2

typedef bool (*funcion_iterador)(void*,void*);

typedef struct campo_iterador{
	funcion_iterador funcion;
	void* extra;
	bool elemento_encontrado;
}campo_iterador_t;

typedef struct rellenar_array{
	void** array;
	int tamanio_array;
	int posicion;
}campos_array_t;


abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){
	abb_t* arbol=NULL;
	arbol=calloc(1, sizeof(abb_t));

	if(!arbol){
		printf("Error reservando memoria.\n");
		return NULL;
	}

	arbol->comparador=comparador;
	arbol->destructor=destructor;
	return arbol;
}

nodo_abb_t* nodo_crear(){
	nodo_abb_t* nodo=NULL;
	nodo=calloc(1, sizeof(nodo_abb_t));
	
	if(!nodo)
		printf("Error reservando memoria para el nodo.\n");
	return nodo;
}

int insetar_en_raiz(abb_t* arbol, void* elemento){
	nodo_abb_t* nodo=nodo_crear();
	if(!nodo)
		return ERROR;

	nodo->elemento=elemento;
	arbol->nodo_raiz=nodo;
	return EXITO;
}

int insertar_nodo_en_derecha(abb_t* arbol, void* elemento, nodo_abb_t* cadena_nodos){
	nodo_abb_t* nodo_nuevo=nodo_crear();
	if(!nodo_nuevo)
		return ERROR;

	nodo_nuevo->elemento=elemento;
	cadena_nodos->derecha=nodo_nuevo;
	return EXITO;
}

int insertar_nodo_en_izquierda(abb_t* arbol, void* elemento, nodo_abb_t* cadena_nodos){
	nodo_abb_t* nodo_nuevo=nodo_crear();
	if(!nodo_nuevo)
		return ERROR;

	nodo_nuevo->elemento=elemento;
	cadena_nodos->izquierda=nodo_nuevo;
	return EXITO;
}


int insertar_en_hoja(abb_t* arbol, void* elemento, nodo_abb_t* cadena_nodos){
	if((*arbol->comparador)(cadena_nodos->elemento, elemento)==1){

		if(!cadena_nodos->izquierda)
			return insertar_nodo_en_izquierda(arbol, elemento, cadena_nodos);
		return insertar_en_hoja(arbol, elemento, cadena_nodos->izquierda);
	
	}else{

		if(!cadena_nodos->derecha)
			return insertar_nodo_en_derecha(arbol, elemento, cadena_nodos);
		return insertar_en_hoja(arbol, elemento, cadena_nodos->derecha);
	
	}
}


bool arbol_vacio(abb_t* arbol){
	if(!arbol)
		return true;

	if(!arbol->nodo_raiz)
		return true;

	return false;
}

int arbol_insertar(abb_t* arbol, void* elemento){
 	if(!arbol)
 		return ERROR;

 	if(arbol_vacio(arbol))
 		return insetar_en_raiz(arbol, elemento);

 	else 
 		return insertar_en_hoja(arbol, elemento, arbol->nodo_raiz);
 }

int usar_destructor(abb_t* arbol, nodo_abb_t* nodo){
	
	arbol->destructor(nodo->elemento);
	free(nodo);
	return EXITO;

}

void cambiar_numero_de_hijos(nodo_abb_t* nodo, int* numero_hijos, int* auxiliar){
	
	if(nodo->izquierda){
		(*numero_hijos)++;
		(*auxiliar)=AUXILIAR_IZQUIERDO;
	}

	if(nodo->derecha)
		(*numero_hijos)++;
}


bool hijo_tiene_elemento(abb_comparador comparador, nodo_abb_t* nodo_hijo, void* elemento){
	if((*comparador)(nodo_hijo->elemento, elemento)==0)
		return true;
	return false;
}


int borrar_hoja(abb_t* arbol, nodo_abb_t* nodo_padre, int auxiliar_hijo){
	
	if(auxiliar_hijo==AUXILIAR_IZQUIERDO){

		usar_destructor(arbol, nodo_padre->izquierda);
		nodo_padre->izquierda=NULL;
		return EXITO;

	}else{

		usar_destructor(arbol, nodo_padre->derecha);
		nodo_padre->derecha=NULL;
		return EXITO;

	}

	return ERROR;
}

int borrar_rama_con_hijo(abb_t* arbol, nodo_abb_t* cadena_nodos, int auxiliar_cual_hijo, int auxiliar_un_hijo){

	if(auxiliar_cual_hijo==AUXILIAR_IZQUIERDO){
		nodo_abb_t* nodo_auxiliar=cadena_nodos->izquierda;

		if(auxiliar_un_hijo==AUXILIAR_IZQUIERDO)
			cadena_nodos->izquierda=cadena_nodos->izquierda->izquierda;

		else
			cadena_nodos->izquierda=cadena_nodos->izquierda->derecha;

		return usar_destructor(arbol, nodo_auxiliar);		

	}else{
		nodo_abb_t* nodo_auxiliar=cadena_nodos->derecha;

		if(auxiliar_un_hijo==AUXILIAR_IZQUIERDO)
			cadena_nodos->derecha=cadena_nodos->derecha->izquierda;

		else
			cadena_nodos->derecha=cadena_nodos->derecha->derecha;

		return usar_destructor(arbol, nodo_auxiliar);		
	}

	return ERROR;
}

nodo_abb_t* buscar_mayor_de_los_menores(nodo_abb_t* cadena_nodos){

	if(!cadena_nodos->derecha->derecha){
		nodo_abb_t* nodo_aux=cadena_nodos->derecha;
		int hijos_nodo_mayor=0;
		int auxiliar=0;
		cambiar_numero_de_hijos(cadena_nodos->derecha, &hijos_nodo_mayor, &auxiliar);
		if(hijos_nodo_mayor!=0)
			cadena_nodos->derecha=nodo_aux->izquierda;
		else
			cadena_nodos->derecha=NULL;
		return nodo_aux;
	}

	return buscar_mayor_de_los_menores(cadena_nodos->derecha);
}	


int borrar_rama_con_hijos(abb_t* arbol, nodo_abb_t* cadena_nodos, int auxiliar_cual_hijo){

	if(auxiliar_cual_hijo==AUXILIAR_IZQUIERDO){
		nodo_abb_t* nodo_auxiliar=cadena_nodos->izquierda;

		if(!cadena_nodos->izquierda->izquierda->derecha){
			cadena_nodos->izquierda=cadena_nodos->izquierda->izquierda;
			cadena_nodos->izquierda->derecha=nodo_auxiliar->derecha;

		}else{

			nodo_abb_t* nodo_mayor_de_menores=buscar_mayor_de_los_menores(cadena_nodos->izquierda->izquierda);
			cadena_nodos->izquierda=nodo_mayor_de_menores;
			nodo_mayor_de_menores->izquierda=nodo_auxiliar->izquierda;
			nodo_mayor_de_menores->derecha=nodo_auxiliar->derecha;

		}

		return usar_destructor(arbol, nodo_auxiliar);

	}else{
		nodo_abb_t* nodo_auxiliar=cadena_nodos->derecha;

		if(!cadena_nodos->derecha->izquierda->derecha){
			cadena_nodos->derecha=cadena_nodos->derecha->izquierda;
			cadena_nodos->derecha->derecha=nodo_auxiliar->derecha;

		}else{

			nodo_abb_t* nodo_mayor_de_menores=buscar_mayor_de_los_menores(cadena_nodos->derecha->izquierda);
			cadena_nodos->derecha=nodo_mayor_de_menores;
			nodo_mayor_de_menores->izquierda=nodo_auxiliar->izquierda;
			nodo_mayor_de_menores->derecha=nodo_auxiliar->derecha;

		}	

		return usar_destructor(arbol, nodo_auxiliar);
	}
}

int borrar_elemento(abb_t* arbol, nodo_abb_t* cadena_nodos,int numero_hijos, int auxiliar_cual_hijo, int auxiliar_un_hijo){
	if(numero_hijos==0){
			return borrar_hoja(arbol, cadena_nodos, auxiliar_cual_hijo);
	}
	else if(numero_hijos==1)
		return borrar_rama_con_hijo(arbol, cadena_nodos, auxiliar_cual_hijo, auxiliar_un_hijo);
	else
		return borrar_rama_con_hijos(arbol, cadena_nodos,auxiliar_cual_hijo);
	return ERROR;
}

int procedimiento_borrar_elemento(abb_t* arbol, nodo_abb_t* cadena_nodos, int auxiliar_cual_hijo){
	int numero_hijos_de_elem=0;
	int auxiliar_un_hijo=0;

	if(auxiliar_cual_hijo==AUXILIAR_IZQUIERDO)
		cambiar_numero_de_hijos(cadena_nodos->izquierda,&numero_hijos_de_elem,&auxiliar_un_hijo);
	else
		cambiar_numero_de_hijos(cadena_nodos->derecha,&numero_hijos_de_elem,&auxiliar_un_hijo);

	return borrar_elemento(arbol, cadena_nodos, numero_hijos_de_elem, auxiliar_cual_hijo, auxiliar_un_hijo);
}

int borrar_elemento_recursivamente(abb_t* arbol, nodo_abb_t* cadena_nodos, void* elemento){

	int auxiliar_comparador=((*arbol->comparador)(cadena_nodos->elemento, elemento));

	if((auxiliar_comparador<0)&&(cadena_nodos->derecha)){

		if(((*arbol->comparador)(cadena_nodos->derecha->elemento, elemento))==0)
			return procedimiento_borrar_elemento(arbol, cadena_nodos,AUXILIAR_DERECHO);

		else
			return borrar_elemento_recursivamente(arbol, cadena_nodos->derecha, elemento);

	}else if((auxiliar_comparador>0)&&(cadena_nodos->izquierda)){

		if(((*arbol->comparador)(cadena_nodos->izquierda->elemento, elemento))==0)
			return procedimiento_borrar_elemento(arbol, cadena_nodos,AUXILIAR_IZQUIERDO);

		else
			return borrar_elemento_recursivamente(arbol, cadena_nodos->izquierda, elemento);

	}

	return ERROR;
}


int borrar_raiz_sin_hijos(abb_t* arbol){

	nodo_abb_t* nodo_auxiliar=arbol->nodo_raiz;
	arbol->nodo_raiz=NULL;
	return usar_destructor(arbol, nodo_auxiliar);

}

int borrar_raiz_con_hijo(abb_t* arbol, int auxiliar_un_hijo){
	nodo_abb_t* nodo_auxiliar=arbol->nodo_raiz;

	if(auxiliar_un_hijo==AUXILIAR_IZQUIERDO)
		arbol->nodo_raiz=arbol->nodo_raiz->izquierda;

	else
		arbol->nodo_raiz=arbol->nodo_raiz->derecha;

	return usar_destructor(arbol, nodo_auxiliar);
}

int borrar_raiz_con_hijos(abb_t* arbol){	
	nodo_abb_t* nodo_auxiliar=arbol->nodo_raiz;

	if(!arbol->nodo_raiz->izquierda->derecha)
		arbol->nodo_raiz=arbol->nodo_raiz->izquierda;

	else{
		nodo_abb_t* nuevo_nodo_raiz=buscar_mayor_de_los_menores(arbol->nodo_raiz->izquierda);	
		arbol->nodo_raiz=nuevo_nodo_raiz;
		arbol->nodo_raiz->izquierda=nodo_auxiliar->izquierda;
	}

	arbol->nodo_raiz->derecha=nodo_auxiliar->derecha;
	return usar_destructor(arbol, nodo_auxiliar);
}

int borrar_nodo_raiz(abb_t* arbol, void* elemento){

	int numero_hijos=0;
	int auxiliar_un_hijo=0;
	cambiar_numero_de_hijos(arbol->nodo_raiz, &numero_hijos, &auxiliar_un_hijo);

	if(numero_hijos==0)
		return borrar_raiz_sin_hijos(arbol);

	else if(numero_hijos==1)
		return borrar_raiz_con_hijo(arbol, auxiliar_un_hijo);

	else
		return borrar_raiz_con_hijos(arbol);

	
	return ERROR;

}


int arbol_borrar(abb_t* arbol, void* elemento){
	if(!arbol)
		return ERROR;

	if(!arbol->nodo_raiz)
		return ERROR;

	else if(!(*arbol->comparador)(arbol->nodo_raiz->elemento,elemento))
		return borrar_nodo_raiz(arbol, elemento);

	else
		return borrar_elemento_recursivamente(arbol, arbol->nodo_raiz, elemento);

	return ERROR;
}


void* buscar_elemento_recursivamente(abb_t* arbol, nodo_abb_t* cadena_nodos, void* elemento){

	int auxiliar_comparador=((*arbol->comparador)(cadena_nodos->elemento, elemento));

	if(auxiliar_comparador==0)
		return cadena_nodos->elemento;

	else if((auxiliar_comparador<0)&&(cadena_nodos->derecha))
		return buscar_elemento_recursivamente(arbol, cadena_nodos->derecha, elemento);

	else if((auxiliar_comparador>0)&&(cadena_nodos->izquierda))
		return buscar_elemento_recursivamente(arbol, cadena_nodos->izquierda, elemento);

	return NULL;
}

void* arbol_buscar(abb_t* arbol, void* elemento){
	if(!arbol)
		return NULL;

	return buscar_elemento_recursivamente(arbol, arbol->nodo_raiz, elemento);

}


void* arbol_raiz(abb_t* arbol){
	if(!arbol)
		return NULL;

	if(arbol_vacio(arbol))
		return NULL;

	return arbol->nodo_raiz->elemento;
}

void rellenar_array(void* elemento, campos_array_t* campo_array){
	
	campo_array->array[campo_array->posicion]=elemento;
	(campo_array->posicion)++;
	(campo_array->tamanio_array)--;

}

bool funcion_auxiliar_campo_iterador(void* elemento, campo_iterador_t* campo_iterador){

	if((*campo_iterador->funcion)(elemento,campo_iterador->extra)){
		campo_iterador->elemento_encontrado=true;
		return true;
	}

	return false;
}

campos_array_t* crear_campo_array(void** array, int tamanio_array){
	campos_array_t* campo=(campos_array_t*)malloc(sizeof(campos_array_t));
	campo->array=array;
	campo->tamanio_array=tamanio_array;
	campo->posicion=0;
	return campo;
}

bool debo_regresar(campos_array_t* campo_array, campo_iterador_t* campo_iterador){
	if(campo_array){
		if(campo_array->tamanio_array==0)
			return true;
	}
	else if(campo_iterador){
		if(campo_iterador->elemento_encontrado)
			return true;
	}
	return false;

}

void recorrer_inorden(nodo_abb_t* cadena_nodos, campos_array_t* campo_array, campo_iterador_t* campo_iterador){
	if(debo_regresar(campo_array, campo_iterador))
			return;

	if(cadena_nodos->izquierda)
		recorrer_inorden(cadena_nodos->izquierda, campo_array, campo_iterador);

	if(debo_regresar(campo_array, campo_iterador))
		return;
	

	if(campo_array)
		rellenar_array(cadena_nodos->elemento, campo_array);
	else if(campo_iterador){
		if(funcion_auxiliar_campo_iterador(cadena_nodos->elemento, campo_iterador))
			return;
	}

	if(debo_regresar(campo_array, campo_iterador))
			return;

	if(cadena_nodos->derecha)
		recorrer_inorden(cadena_nodos->derecha, campo_array, campo_iterador);	

	return;
}

int arbol_recorrido_inorden(abb_t* arbol, void** array, int tamanio_array){
	
	if(!arbol)
		return 0;

	if(tamanio_array<=0)
		return 0;

	campos_array_t* campo_array=crear_campo_array(array, tamanio_array);

	recorrer_inorden(arbol->nodo_raiz, campo_array, NULL);

	int numero_elementos=campo_array->posicion;

	free(campo_array);

	return numero_elementos;
}

void recorrer_preorden(nodo_abb_t* cadena_nodos, campos_array_t* campo_array, campo_iterador_t* campo_iterador){

	if(debo_regresar(campo_array, campo_iterador)){
		if(!campo_array->tamanio_array)
			return;
	}

	if(campo_array)
		rellenar_array(cadena_nodos->elemento, campo_array);
	else if(campo_iterador){
		if(funcion_auxiliar_campo_iterador(cadena_nodos->elemento, campo_iterador))
			return;
	}

	if(debo_regresar(campo_array, campo_iterador)){
		return;
	}


	if(cadena_nodos->izquierda)
		recorrer_preorden(cadena_nodos->izquierda, campo_array, campo_iterador);

	if(debo_regresar(campo_array, campo_iterador)){
		return;
	}


	if(cadena_nodos->derecha)
		recorrer_preorden(cadena_nodos->derecha, campo_array, campo_iterador);

	return;
}

int arbol_recorrido_preorden(abb_t* arbol, void** array, int tamanio_array){
	
	if(!arbol)
		return 0;

	if(tamanio_array<=0)
		return 0;

	campos_array_t* campo_array=crear_campo_array(array, tamanio_array);

	recorrer_preorden(arbol->nodo_raiz, campo_array, NULL);

	int numero_elementos=campo_array->posicion;

	free(campo_array);

	return numero_elementos;
}

void recorrer_postorden(nodo_abb_t* cadena_nodos, campos_array_t* campo_array, campo_iterador_t* campo_iterador ){
	
	if(debo_regresar(campo_array, campo_iterador))
		return;

	if(cadena_nodos->izquierda)
		recorrer_postorden(cadena_nodos->izquierda, campo_array, campo_iterador);

	if(debo_regresar(campo_array, campo_iterador))
		return;

	if(cadena_nodos->derecha)
		recorrer_postorden(cadena_nodos->derecha, campo_array, campo_iterador);
	
	if(debo_regresar(campo_array, campo_iterador))
		return;
	
	if(campo_array)
		rellenar_array(cadena_nodos->elemento, campo_array);
	else if(campo_iterador){
		if(funcion_auxiliar_campo_iterador(cadena_nodos->elemento, campo_iterador))
			return;
	}

	return;

}

int arbol_recorrido_postorden(abb_t* arbol, void** array, int tamanio_array){

	if(!arbol)
		return 0;

	if(tamanio_array<=0)
		return 0;

	campos_array_t* campo_array=crear_campo_array(array, tamanio_array);

	recorrer_postorden(arbol->nodo_raiz, campo_array, NULL);

	int numero_elementos=campo_array->posicion;

	free(campo_array);

	return numero_elementos;

}

void borrar_todo_el_arbol(abb_t* arbol, nodo_abb_t* cadena_nodos){

	if(cadena_nodos->izquierda)
		borrar_todo_el_arbol(arbol, cadena_nodos->izquierda);

	if(cadena_nodos->derecha)
		borrar_todo_el_arbol(arbol, cadena_nodos->derecha);

	usar_destructor(arbol, cadena_nodos);

}

void arbol_destruir(abb_t* arbol){
	if(!arbol)
		return;

	if(arbol->nodo_raiz)
		borrar_todo_el_arbol(arbol, arbol->nodo_raiz);
	
	free(arbol);
}


campo_iterador_t* rellenar_struct_parametros(bool (*funcion)(void*, void*), void* extra){
	campo_iterador_t* campos=(campo_iterador_t*)malloc(sizeof(campo_iterador_t));
	campos->funcion=funcion;
	campos->extra=extra;
	campos->elemento_encontrado=false;
	return campos;

}

void abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){
	
	if(!arbol||!extra)
		return;

	if(arbol_vacio(arbol))
		return;

	campo_iterador_t* campo_iterador=rellenar_struct_parametros(funcion, extra);

	if(recorrido==ABB_RECORRER_INORDEN)
		recorrer_inorden(arbol->nodo_raiz, NULL, campo_iterador);

	else if(recorrido==ABB_RECORRER_PREORDEN)
		recorrer_preorden(arbol->nodo_raiz, NULL, campo_iterador);

	else if(recorrido==ABB_RECORRER_POSTORDEN)
		recorrer_postorden(arbol->nodo_raiz, NULL, campo_iterador);

	free(campo_iterador);

}
