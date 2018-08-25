//
//  tree.h
//  Lista(enDisco)
//
//  Created by Victorio Scafati on 8/24/17.
//  Copyright © 2017 Victorio Scafati. All rights reserved.
//

#ifndef tree_h
#define tree_h
#include <stdbool.h>

#define MAX 36000
#define PATH_ARBOL "files/arbolIndex.bin"
#define RAIZ_ARBOL 0      //POSICION DE LA RAIZ EN EL ARCHIVO

struct tipoE {long cuit, reg;};

// Estructura de Archivo Index
struct Nodo{
    long HD, HI;
    struct tipoE Datos; // Similar al tipoElemento de la TAD
    char marca;
};

typedef struct Nodo* PosicionArbol;

// Estructura del Arbol en runtime
struct Arbol {
    long q_items;
};

// Prototypes
extern bool esVacio(struct Arbol*);
extern bool esLleno(struct Arbol*);
extern void actualizarArbol(struct Arbol* A);
extern long busquedaBinaria(struct Arbol* A, long cuit); //Devuelve el numero de registro
extern void insertarNodo(struct Arbol* A, long , long);
extern void borrarNodo(struct Arbol*, long );
extern void inOrden(); // path arbol + path archivo abm
extern void treeView();

#endif /* tree_h */
