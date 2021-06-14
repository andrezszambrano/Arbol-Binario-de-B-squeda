#include <stdio.h>
#include "abb.h"
#include "tda_pruebas.h"
#include <string.h>
#define ERROR -1
#define EXITO 0


void destructor(void* elemento){
	return;
}

//Compara números enteros solamente.
//Devuelve 1 sí el elemento 1 es mayor, -1 sí el elemento 2 es mayor, 0 sí son iguales.
int abb_comparador_enteros(void* elemento1, void* elemento2){
	
	int numero1=*((int*)elemento1);
	int numero2=*((int*)elemento2);

	if(numero1==numero2)
		return 0;
	if(numero1>numero2)
		return 1;
	else
		return -1;
	return ERROR;
}

bool funcion_comparadora(void* elemento1, void* elemento2){
	
	int numero1=*((int*)elemento1);
	int numero2=*((int*)elemento2);
	printf("%i\n",numero1);
	
	if(numero1==numero2)
		return true;

	return false;
}


void probar_funcion_crear(tda_pruebas_t* pruebas){
	abb_t* arbol=arbol_crear(NULL, NULL);
	tda_pruebas_afirmar(pruebas, "Árbol se creó correctamente.",((arbol->nodo_raiz==NULL)&&(arbol->comparador==NULL)&&(arbol->destructor==NULL)));
	arbol_destruir(arbol);
}

void probar_funcion_insertar(tda_pruebas_t* pruebas){
	{
		int numero=2;
		tda_pruebas_afirmar(pruebas, "Al enviar árbol null, función insertar regresa ERROR.",arbol_insertar(NULL,&numero)==ERROR);
	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		int elemento=4;
		arbol_insertar(arbol, &elemento);
		printf("------------------------------------------------\nAgrego un elemento a árbol vacío\n");
		tda_pruebas_afirmar(pruebas, "Árbol tiene el elemento esperado.",*((int*)arbol->nodo_raiz->elemento)==elemento);
		int elemento2=2;
		int elemento3=7;
		printf("Se agregan otros dos elementos\n");
		arbol_insertar(arbol, &elemento2);
		arbol_insertar(arbol, &elemento3);
		tda_pruebas_afirmar(pruebas, "Árbol tiene los elementos correspondientes.",(*((int*)arbol->nodo_raiz->izquierda->elemento)==elemento2)&&(*((int*)arbol->nodo_raiz->derecha->elemento)==elemento3));
		printf("------------------------------------------------\n");
		arbol_destruir(arbol);
	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		printf("------------------------------------------------\nAgrego 20 elementos arbitrarios no repetidos a árbol vacío\n");
		int elementos[20]={5,2,10,54,7,6,52,1,40,60,69,8,11,9,15,3,4,56,19,91};
		for(int i=0;i<20;i++)
			arbol_insertar(arbol, &elementos[i]);
		tda_pruebas_afirmar(pruebas, "Árbol tiene elementos arbitrarios en posiciones respectivas.",(*((int*)arbol->nodo_raiz->izquierda->derecha->derecha->elemento)==elementos[16])&&(*((int*)arbol->nodo_raiz->derecha->derecha->izquierda->izquierda->izquierda->derecha->derecha->elemento)==elementos[18])&&(*((int*)arbol->nodo_raiz->derecha->derecha->izquierda->elemento)==elementos[6])&&(*((int*)arbol->nodo_raiz->derecha->derecha->derecha->derecha->elemento)==elementos[10]));
		printf("Agrego otro elemento\n");
		int elemento=55;
		arbol_insertar(arbol, &elemento);
		tda_pruebas_afirmar(pruebas, "Árbol tiene el elemento agregado en la posición correspondiente.",(*((int*)arbol->nodo_raiz->derecha->derecha->derecha->izquierda->izquierda->elemento)==elemento));
		printf("------------------------------------------------\n");
		arbol_destruir(arbol);
	}
}

void probar_funcion_esta_vacio(tda_pruebas_t* pruebas){
	{
		tda_pruebas_afirmar(pruebas, "Función está vacío regresa vacío al enviar árbol NULL.",arbol_vacio(NULL));
	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		tda_pruebas_afirmar(pruebas, "Función regresa true cuando no hay nodo raiz.", arbol_vacio(arbol));
		arbol_destruir(arbol);
	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		int elemento=4;
		arbol_insertar(arbol, &elemento);
		tda_pruebas_afirmar(pruebas, "Función regresa false cuando hay elementos", !arbol_vacio(arbol));
		arbol_destruir(arbol);
	}
}



void probar_funcion_recorrer_inorden(tda_pruebas_t* pruebas){
	{
		void** vector=(void**)calloc(2,sizeof(void*));
		tda_pruebas_afirmar(pruebas, "Al enviar árbol null, función recorrer inorden regresa cero.",arbol_recorrido_inorden(NULL,vector,10)==0);
		free(vector);
	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		printf("------------------------------------------------\nAgrego 2 elementos a árbol vacío, lleno vector inorder con...\n");
		int elemento1=4;
		int elemento2=5;
		arbol_insertar(arbol, &elemento1);
		arbol_insertar(arbol, &elemento2);
		void** vector=(void**)calloc(2,sizeof(void*));
		int tamanio_vector=arbol_recorrido_inorden(arbol, vector, 0);
		tda_pruebas_afirmar(pruebas, "0 elementos, función devuelve 0.",tamanio_vector==0);
		
		tamanio_vector=arbol_recorrido_inorden(arbol, vector, -2);
		tda_pruebas_afirmar(pruebas, "cantidad negativa de elementos, función devuelve 0.",tamanio_vector==0);
		
		tamanio_vector=arbol_recorrido_inorden(arbol, vector, 1);
		int contador=0;
		if(*((int*)vector[0])==elemento1)
			contador++;
		tda_pruebas_afirmar(pruebas, "1 elemento, función devuelve 1 y elemento en vector es correspondiente.",(tamanio_vector==1)&&(contador==1));

		tamanio_vector=arbol_recorrido_inorden(arbol, vector, 10);
		contador=0;
		int elementos[2]={4,5};
		for(int i=0;i<2;i++){
			if(*((int*)vector[i])==elementos[i])
				contador++;
		}
		tda_pruebas_afirmar(pruebas, "cantidad mayor a 2, función devuelve 2 y elementos en vector son correspondientes.", (tamanio_vector==2)&&(contador==2));
		
		free(vector);


		printf("------------------------------------------------\n");	
		arbol_destruir(arbol);
	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		printf("------------------------------------------------\nAgrego 12 elementos a árbol vacío, recorro árbol inorder y lleno vector\n");
		
		printf("Pido 12 elementos\n");
		int elementos[12]={10,9,20,8,9,1,14,23,11,15,21,25};
		for(int i=0;i<12;i++)
			arbol_insertar(arbol, &elementos[i]);
		void** vector=(void**)calloc(12, sizeof(void*));
		int tamanio_vector=arbol_recorrido_inorden(arbol, vector, 12);
		int contador=0;
		int array_in_order[12]={1,8,9,9,10,11,14,15,20,21,23,25};
		for(int i=0;i<12;i++){
			if(*((int*)vector[i])==array_in_order[i])
				contador++;
		}
		tda_pruebas_afirmar(pruebas, "Vector y su tope son correctos.",(contador==12)&&(tamanio_vector==12));
		
		printf("Pido 100 elementos\n");
		tamanio_vector=arbol_recorrido_inorden(arbol, vector, 100);
		contador=0;
		for(int i=0;i<12;i++){
			if(*((int*)vector[i])==array_in_order[i])
				contador++;
		}
		tda_pruebas_afirmar(pruebas, "Vector y su tope son correctos.",(contador==12)&&(tamanio_vector==12));

		printf("Pido 7 elementos\n");
		tamanio_vector=arbol_recorrido_inorden(arbol, vector, 7);
		contador=0;
		for(int i=0;i<7;i++){
			if(*((int*)vector[i])==array_in_order[i])
				contador++;
		}
		tda_pruebas_afirmar(pruebas, "Vector y su tope son correctos.",(contador==7)&&(tamanio_vector==7));

		free(vector);
		printf("------------------------------------------------\n");
		arbol_destruir(arbol);
	}
}

void probar_funcion_recorrer_preorden(tda_pruebas_t* pruebas){
	{
		void** vector=(void**)calloc(2,sizeof(void*));
		tda_pruebas_afirmar(pruebas, "Al enviar árbol null, función recorrer preorden regresa cero.",arbol_recorrido_preorden(NULL,vector,10)==0);
		free(vector);
	}

	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		printf("------------------------------------------------\nAgrego 2 elementos a árbol vacío, lleno vector preorder con...\n");
		int elemento1=4;
		int elemento2=5;
		arbol_insertar(arbol, &elemento1);
		arbol_insertar(arbol, &elemento2);
		void** vector=(void**)calloc(2,sizeof(void*));
		int tamanio_vector=arbol_recorrido_preorden(arbol, vector, 0);
		tda_pruebas_afirmar(pruebas, "0 elementos, función devuelve 0.",tamanio_vector==0);
		
		tamanio_vector=arbol_recorrido_preorden(arbol, vector, -2);
		tda_pruebas_afirmar(pruebas, "cantidad negativa de elementos, función devuelve 0.",tamanio_vector==0);
		
		tamanio_vector=arbol_recorrido_preorden(arbol, vector, 1);
		int contador=0;
		if(*((int*)vector[0])==elemento1)
			contador++;
		tda_pruebas_afirmar(pruebas, "1 elemento, función devuelve 1 y elemento en vector es correspondiente.",(tamanio_vector==1)&&(contador==1));

		tamanio_vector=arbol_recorrido_preorden(arbol, vector, 10);
		contador=0;
		int elementos[2]={4,5};
		for(int i=0;i<2;i++){
			if(*((int*)vector[i])==elementos[i])
				contador++;
		}
		tda_pruebas_afirmar(pruebas, "cantidad mayor a 2, función devuelve 2 y elementos en vector son correspondientes.", (tamanio_vector==2)&&(contador==2));
		
		free(vector);

		printf("------------------------------------------------\n");	
		arbol_destruir(arbol);
	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		printf("------------------------------------------------\nAgrego 12 elementos a árbol vacío, recorro árbol preorder y lleno vector\n");
		
		printf("Pido 12 elementos\n");
		int elementos[12]={10,9,20,8,9,1,14,23,11,15,21,25};
		for(int i=0;i<12;i++)
			arbol_insertar(arbol, &elementos[i]);
		void** vector=(void**)calloc(12, sizeof(void*));
		int tamanio_vector=arbol_recorrido_preorden(arbol, vector, 12);
		int contador=0;
		int array_pre_order[12]={10, 9, 8, 1, 9, 20, 14, 11, 15, 23, 21, 25};
		for(int i=0;i<12;i++){
			if(*((int*)vector[i])==array_pre_order[i])
				contador++;
		}
		tda_pruebas_afirmar(pruebas, "Vector y su tope son correctos.",(contador==12)&&(tamanio_vector==12));
		
		printf("Pido 100 elementos\n");
		tamanio_vector=arbol_recorrido_preorden(arbol, vector, 100);
		contador=0;
		for(int i=0;i<12;i++){
			if(*((int*)vector[i])==array_pre_order[i])
				contador++;
		}
		tda_pruebas_afirmar(pruebas, "Vector y su tope son correctos.",(contador==12)&&(tamanio_vector==12));

		printf("Pido 10 elementos\n");
		tamanio_vector=arbol_recorrido_preorden(arbol, vector, 10);
		contador=0;
		for(int i=0;i<10;i++){
			if(*((int*)vector[i])==array_pre_order[i])
				contador++;
		}
		tda_pruebas_afirmar(pruebas, "Vector y su tope son correctos.",(contador==10)&&(tamanio_vector==10));

		free(vector);
		printf("------------------------------------------------\n");
		arbol_destruir(arbol);
	}
}


void probar_funcion_recorrer_postorden(tda_pruebas_t* pruebas){
	{
		void** vector=(void**)calloc(2,sizeof(void*));
		tda_pruebas_afirmar(pruebas, "Al enviar árbol null, función recorrer postorden regresa cero.",arbol_recorrido_postorden(NULL,vector,10)==0);
		free(vector);
	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		printf("------------------------------------------------\nAgrego 2 elementos a árbol vacío, lleno vector postorder con...\n");
		int elemento1=4;
		int elemento2=5;
		arbol_insertar(arbol, &elemento1);
		arbol_insertar(arbol, &elemento2);
		void** vector=(void**)calloc(2,sizeof(void*));
		int tamanio_vector=arbol_recorrido_postorden(arbol, vector, 0);
		tda_pruebas_afirmar(pruebas, "0 elementos, función devuelve 0.",tamanio_vector==0);
		
		tamanio_vector=arbol_recorrido_postorden(arbol, vector, -2);
		tda_pruebas_afirmar(pruebas, "cantidad negativa de elementos, función devuelve 0.",tamanio_vector==0);
		
		tamanio_vector=arbol_recorrido_postorden(arbol, vector, 1);
		int contador=0;
		if(*((int*)vector[0])==elemento2)
			contador++;
		tda_pruebas_afirmar(pruebas, "1 elemento, función devuelve 1 y elemento en vector es correspondiente.",(tamanio_vector==1)&&(contador==1));

		tamanio_vector=arbol_recorrido_postorden(arbol, vector, 10);
		contador=0;
		int elementos[2]={5,4};
		for(int i=0;i<2;i++){
			if(*((int*)vector[i])==elementos[i])
				contador++;
		}
		tda_pruebas_afirmar(pruebas, "cantidad mayor a 2, función devuelve 2 y elementos en vector son correspondientes.", (tamanio_vector==2)&&(contador==2));
		
		free(vector);

		printf("------------------------------------------------\n");	
		arbol_destruir(arbol);
	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		printf("------------------------------------------------\nAgrego 12 elementos a árbol vacío, recorro árbol postorder y lleno vector\n");
		
		printf("Pido 12 elementos\n");
		int elementos[12]={10,9,20,8,9,1,14,23,11,15,21,25};
		for(int i=0;i<12;i++)
			arbol_insertar(arbol, &elementos[i]);
		void** vector=(void**)calloc(12, sizeof(void*));
		int tamanio_vector=arbol_recorrido_postorden(arbol, vector, 12);
		int contador=0;
		int array_post_order[12]={1,8,9,9,11,15,14,21,25,23,20,10};
		for(int i=0;i<12;i++){
			if(*((int*)vector[i])==array_post_order[i])
				contador++;
		}
		tda_pruebas_afirmar(pruebas, "Vector y su tope son correctos.",(contador==12)&&(tamanio_vector==12));
		
		printf("Pido 100 elementos\n");
		tamanio_vector=arbol_recorrido_postorden(arbol, vector, 100);
		contador=0;
		for(int i=0;i<12;i++){
			if(*((int*)vector[i])==array_post_order[i])
				contador++;
		}
		tda_pruebas_afirmar(pruebas, "Vector y su tope son correctos.",(contador==12)&&(tamanio_vector==12));

		printf("Pido 5 elementos\n");
		tamanio_vector=arbol_recorrido_postorden(arbol, vector, 5);
		contador=0;
		for(int i=0;i<5;i++){
			if(*((int*)vector[i])==array_post_order[i])
				contador++;
		}
		tda_pruebas_afirmar(pruebas, "Vector y su tope son correctos.",(contador==5)&&(tamanio_vector==5));

		free(vector);
		printf("------------------------------------------------\n");
		arbol_destruir(arbol);
	}
}

void probar_funcion_borrar(tda_pruebas_t* pruebas){
	{
		int numero=2;
		tda_pruebas_afirmar(pruebas, "Al enviar árbol null, función borrar regresa ERROR.",arbol_borrar(NULL,&numero)==ERROR);
	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		int numero=2;
		tda_pruebas_afirmar(pruebas, "No hay nada que borrar, función regresa error.",arbol_borrar(arbol, &numero)==ERROR);
		arbol_destruir(arbol);
	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		int numero=2;
		arbol_insertar(arbol, &numero);
		tda_pruebas_afirmar(pruebas, "Elimina raíz que no tiene hijos.",(arbol_borrar(arbol, &numero)==EXITO)&&(!arbol->nodo_raiz));
		arbol_destruir(arbol);	
	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		int numero[]={2,1,5};
		arbol_insertar(arbol, &numero[0]);
		arbol_insertar(arbol, &numero[1]);
		tda_pruebas_afirmar(pruebas, "Elimina raíz que tiene un hijo en su izquierda, correspondientemente.",(arbol_borrar(arbol, &numero[0])==EXITO)&&(arbol->nodo_raiz->elemento==&numero[1]));
		arbol_insertar(arbol, &numero[2]);
		tda_pruebas_afirmar(pruebas, "Elimina raíz que tiene un hijo en su derecha, correspondientemente.",(arbol_borrar(arbol, &numero[1])==EXITO)&&(arbol->nodo_raiz->elemento==&numero[2]));
		arbol_destruir(arbol);	
	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		int numero[]={2,-3,5,4,9,0,-10,7};
		for(int i=0; i<8; i++)
			arbol_insertar(arbol, &numero[i]);
		tda_pruebas_afirmar(pruebas, "Elimina raíz que tiene hijos en ambos lados, correspondientemente.",(arbol_borrar(arbol, &numero[0])==EXITO)&&(arbol->nodo_raiz->elemento==&numero[5])&&(!arbol->nodo_raiz->izquierda->derecha)&&(arbol->nodo_raiz->izquierda->elemento==&numero[1])&&(arbol->nodo_raiz->derecha->elemento==&numero[2]));
		tda_pruebas_afirmar(pruebas, "Elimina nueva raíz que tiene hijos en ambos lados, correspondientemente.",(arbol_borrar(arbol, &numero[5])==EXITO)&&(arbol->nodo_raiz->elemento==&numero[1])&&(arbol->nodo_raiz->izquierda->elemento==&numero[6])&&(arbol->nodo_raiz->derecha->elemento==&numero[2]));
		tda_pruebas_afirmar(pruebas, "Función regresa error al no encontrar elemento a borrar.",(arbol_borrar(arbol, &numero[0])==ERROR));
		arbol_destruir(arbol);	
	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		int numero[]={50,40,60,45,30,47,46};
		for(int i=0; i<7; i++)
			arbol_insertar(arbol, &numero[i]);
		tda_pruebas_afirmar(pruebas, "Elimina raíz que tiene hijos en ambos lados (caso especial), correspondientemente.",(arbol_borrar(arbol, &numero[0])==EXITO)&&(arbol->nodo_raiz->elemento==&numero[5])&&(arbol->nodo_raiz->izquierda->derecha->derecha->elemento==&numero[6]));
		arbol_destruir(arbol);	
	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		int numero[]={50,40,60,30,45,55,70,20,35,42,47,52,57,65,80};
		for(int i=0; i<15; i++)
			arbol_insertar(arbol, &numero[i]);
		printf("------------------------------------------------\nAgrego 15 elementos a árbol vacío\n");
		tda_pruebas_afirmar(pruebas, "Elimina hoja que no tiene hijos, correspondientemente.",(arbol_borrar(arbol, &numero[7])==EXITO)&&(!arbol->nodo_raiz->izquierda->izquierda->izquierda));
		printf("Cubro todos los casos de borrar una rama con un hijo\n");

		tda_pruebas_afirmar(pruebas, "Elimina rama que tiene un hijo, correspondientemente.",(arbol_borrar(arbol, &numero[3])==EXITO)&&(arbol->nodo_raiz->izquierda->izquierda->elemento==&numero[8]));
		
		tda_pruebas_afirmar(pruebas, "Elimina hoja que no tiene hijos, correspondientemente.",(arbol_borrar(arbol, &numero[10])==EXITO)&&(!arbol->nodo_raiz->izquierda->derecha->derecha));
		tda_pruebas_afirmar(pruebas, "Elimina rama que tiene un hijo, correspondientemente.",(arbol_borrar(arbol, &numero[4])==EXITO)&&(arbol->nodo_raiz->izquierda->derecha->elemento==&numero[9]));
	
		tda_pruebas_afirmar(pruebas, "Elimina hoja que no tiene hijos, correspondientemente.",(arbol_borrar(arbol, &numero[12])==EXITO)&&(!arbol->nodo_raiz->derecha->izquierda->derecha));
		tda_pruebas_afirmar(pruebas, "Elimina rama que tiene un hijo, correspondientemente.",(arbol_borrar(arbol, &numero[5])==EXITO)&&(arbol->nodo_raiz->derecha->izquierda->elemento==&numero[11]));
	
		tda_pruebas_afirmar(pruebas, "Elimina hoja que no tiene hijos, correspondientemente.",(arbol_borrar(arbol, &numero[13])==EXITO)&&(!arbol->nodo_raiz->derecha->derecha->izquierda));
		tda_pruebas_afirmar(pruebas, "Elimina rama que tiene un hijo, correspondientemente.",(arbol_borrar(arbol, &numero[6])==EXITO)&&(arbol->nodo_raiz->derecha->derecha->elemento==&numero[14]));
		printf("------------------------------------------------\n");		
		arbol_destruir(arbol);

	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		int numero[]={50,40,60,30,45,55,70,20,35,42,47,52,57,65,80};
		for(int i=0; i<15; i++)
			arbol_insertar(arbol, &numero[i]);
		printf("------------------------------------------------\nAgrego 15 elementos a árbol vacío, pruebo los casos de borrar rama con dos hijos\n");
		tda_pruebas_afirmar(pruebas, "Elimina rama que tiene dos hijos, correspondientemente.",(arbol_borrar(arbol, &numero[1])==EXITO)&&(arbol->nodo_raiz->izquierda->elemento==&numero[8])&&(arbol->nodo_raiz->izquierda->izquierda->elemento==&numero[3])&&(arbol->nodo_raiz->izquierda->derecha->elemento==&numero[4])&&(!arbol->nodo_raiz->izquierda->izquierda->derecha));
		tda_pruebas_afirmar(pruebas, "Elimina rama que tiene dos hijos, correspondientemente.",(arbol_borrar(arbol, &numero[2])==EXITO)&&(arbol->nodo_raiz->derecha->elemento==&numero[12])&&(arbol->nodo_raiz->derecha->izquierda->elemento==&numero[5])&&(arbol->nodo_raiz->derecha->derecha->elemento==&numero[6])&&(!arbol->nodo_raiz->derecha->izquierda->derecha));
		tda_pruebas_afirmar(pruebas, "Elimina rama que tiene dos hijos, correspondientemente.",(arbol_borrar(arbol, &numero[8])==EXITO)&&(arbol->nodo_raiz->izquierda->elemento==&numero[3])&&(arbol->nodo_raiz->izquierda->izquierda->elemento==&numero[7])&&(arbol->nodo_raiz->izquierda->derecha->elemento==&numero[4]));
		tda_pruebas_afirmar(pruebas, "Elimina rama que tiene dos hijos, correspondientemente.",(arbol_borrar(arbol, &numero[12])==EXITO)&&(arbol->nodo_raiz->derecha->elemento==&numero[5])&&(arbol->nodo_raiz->derecha->izquierda->elemento==&numero[11])&&(arbol->nodo_raiz->derecha->derecha->elemento==&numero[6]));
		tda_pruebas_afirmar(pruebas, "Elimina rama que tiene dos hijos, correspondientemente.",(arbol_borrar(arbol, &numero[4])==EXITO)&&(arbol->nodo_raiz->izquierda->derecha->elemento==&numero[9])&&(arbol->nodo_raiz->izquierda->derecha->derecha->elemento==&numero[10])&&(!arbol->nodo_raiz->izquierda->derecha->izquierda));
		tda_pruebas_afirmar(pruebas, "Elimina rama que tiene dos hijos, correspondientemente.",(arbol_borrar(arbol, &numero[6])==EXITO)&&(arbol->nodo_raiz->derecha->derecha->elemento==&numero[13])&&(arbol->nodo_raiz->derecha->derecha->derecha->elemento==&numero[14])&&(!arbol->nodo_raiz->derecha->derecha->izquierda));

		printf("------------------------------------------------\n");		
		arbol_destruir(arbol);
	}

}

void probar_funcion_buscar(tda_pruebas_t* pruebas){
	{
		int numero=2;
		tda_pruebas_afirmar(pruebas, "Al enviar árbol null, función borrar regresa ERROR.",arbol_borrar(NULL,&numero)==ERROR);
	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		int numero[]={50,40,60,30,45,55,70,20,35,42,47,52,57,65,80};
		for(int i=0; i<15; i++)
			arbol_insertar(arbol, &numero[i]);
		int elemento=4;
		printf("------------------------------------------------\nAgrego 15 elementos a árbol vacío, busco elementos arbitrarios\n");
		tda_pruebas_afirmar(pruebas, "Encuentro el elemento que pedí.", arbol_buscar(arbol, &numero[0])==&numero[0]);
		tda_pruebas_afirmar(pruebas, "Encuentro el elemento que pedí.", arbol_buscar(arbol, &numero[12])==&numero[12]);
		tda_pruebas_afirmar(pruebas, "Encuentro el elemento que pedí.", arbol_buscar(arbol, &numero[5])==&numero[5]);
		tda_pruebas_afirmar(pruebas, "No encuentro elemento inexistente, regresa NULL.", arbol_buscar(arbol, &elemento)==NULL);
		arbol_destruir(arbol);
	}
}

void probar_funcion_iterador(tda_pruebas_t* pruebas){
	{
		int numero=2;
		tda_pruebas_avisar(pruebas, "Mando NULL como parámetro de árbol a iterador interno.");
		abb_con_cada_elemento(NULL, 0, &funcion_comparadora, &numero);
	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		int numero[]={50,40,60,30,45,55,70,20,35,42,47,52,57,65,80};
		for(int i=0; i<15; i++)
			arbol_insertar(arbol, &numero[i]);
		tda_pruebas_avisar(pruebas, "Mando NULL como elemento a iterador interno");
		abb_con_cada_elemento(arbol, 0, &funcion_comparadora, NULL);
		arbol_destruir(arbol);
	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		int numero=4;
		tda_pruebas_avisar(pruebas, "Mando árbol vacío a iterador interno.");
		abb_con_cada_elemento(arbol, 0, &funcion_comparadora, &numero);
		arbol_destruir(arbol);
	}
	{
		abb_t* arbol=arbol_crear(&abb_comparador_enteros, &destructor);
		int numero[]={50,40,60,30,45,55,70,20,35,42,47,52,57,65,80};
		for(int i=0; i<15; i++)
			arbol_insertar(arbol, &numero[i]);
		tda_pruebas_avisar(pruebas, "Hago distintos recorridos con iterador interno");
		abb_con_cada_elemento(arbol, 0, &funcion_comparadora, &numero[4]);
		printf("-\n");
		abb_con_cada_elemento(arbol, 1, &funcion_comparadora, &numero[4]);
		printf("-\n");
		abb_con_cada_elemento(arbol, 2, &funcion_comparadora, &numero[4]);
		printf("-\n");
		arbol_destruir(arbol);
	}
}

void probar_funciones_arbol(tda_pruebas_t* pruebas){
	probar_funcion_crear(pruebas);
	probar_funcion_insertar(pruebas);
	probar_funcion_esta_vacio(pruebas);
	probar_funcion_recorrer_inorden(pruebas);
	probar_funcion_recorrer_preorden(pruebas);
	probar_funcion_recorrer_postorden(pruebas);
	probar_funcion_buscar(pruebas);
	probar_funcion_borrar(pruebas);
	probar_funcion_iterador(pruebas);
}



int main(){
	tda_pruebas_t* pruebas=tda_pruebas_crear();
	probar_funciones_arbol(pruebas);
	printf("------------------------------------------------\n");
	tda_pruebas_mostrar_estadisticas(pruebas);
	tda_pruebas_destruir(pruebas);
	return 0;
}