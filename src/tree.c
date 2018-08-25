//
//  tree.c
//  arbolEnDisco
//
//  Created by Victorio Scafati on 9/24/17.
//  Copyright © 2017 Victorio Scafati. All rights reserved.
//

// Implementacion
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <errno.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "abm.h"

bool esVacio(struct Arbol *A){
    return (A->q_items == 0);
}

bool esLleno(struct Arbol *A){
    return (A->q_items == MAX);
}

void actualizarArbol(struct Arbol* A) {
    PosicionArbol Q = (PosicionArbol) malloc(sizeof(struct Nodo));
    if (Q == NULL){
        exit(EXIT_FAILURE);
    }
    FILE* fa = fopen(PATH_ARBOL, "rb");
    fseek(fa, 0, SEEK_END);
    if (ftell(fa) == 0){ // La primera vez que se crea el Arbol
        A->q_items = 0;
    } else {            // Setea contador(q_items)
        int c = 0;
        fseek(fa, 0, SEEK_SET);
        fread(Q, sizeof(struct Nodo), 1, fa);
        if (Q->marca != '2') {
            fseek(fa, 0, SEEK_END);
            long k = (ftell(fa)/sizeof(struct Nodo));
            for (int i = 0; i < k; i++) {
                fseek(fa, sizeof(struct Nodo)*i, SEEK_SET);
                fread(Q, sizeof(struct Nodo), 1, fa);
                if (Q->marca != '2') c++;
            }
        }
        A->q_items = c;
    }
    free(Q);
    fclose(fa);
}

void crearNodo(struct tipoE X){
    struct Nodo P;
    FILE* fa = fopen(PATH_ARBOL, "ab+");
    P.Datos = X;
    P.HD = -1;
    P.HI = -1;
    P.marca = '1';
    fseek(fa, 0, SEEK_END);
    fwrite(&P,sizeof(struct Nodo),1,fa);
    fclose(fa);
}


void insertarNodo(struct Arbol* A, long cuit, long reg){
    PosicionArbol Q, P;
    struct Nodo t, j;
    Q = &t;
    P = &j;
    struct tipoE X;
    X.cuit = cuit;
    X.reg = reg;
    long Preg = 0;
    FILE* fa = fopen(PATH_ARBOL, "rb+");
    //actualizarArbol(A);
    if (esLleno(A)) {
        perror("Arbol Lleno");
        exit(EXIT_FAILURE);
    } else {
        if (esVacio(A)){ // Inserta la Raiz. Primer Nodo
            crearNodo(X);
            A->q_items++;
        } else {
            // Recorrida del Arbol comenzando por la raiz
            long i = RAIZ_ARBOL;
            while (i != -1){
                fseek(fa, sizeof(struct Nodo)*i, SEEK_SET);
                fread(Q, sizeof(struct Nodo), 1, fa);
                P = Q;                  //  Guardamos datos del padre
                Preg = i;       //
                if (X.cuit < Q->Datos.cuit){
                    i = Q->HI;
                } else {
                    i = Q->HD;
                }
            }
            crearNodo(X);
            A->q_items++;
            // Ultima Posicion del archivo
            fseek(fa, 0, SEEK_END);
            long p = ftell(fa)/sizeof(struct Nodo);
            if (X.cuit < P->Datos.cuit) {       //Enganchamos padre con hijo
                P->HI = p-1;
            } else {
                P->HD = p-1;
            }
            fseek(fa, sizeof(struct Nodo)*Preg, SEEK_SET);
            fwrite(P, sizeof(struct Nodo), 1, fa);
        }
    }
    fclose(fa);
    //actualizarArbol(A);  Sacar esto en la version final
}

long busquedaBinaria(struct Arbol* A, long cuit){
    PosicionArbol Q = (PosicionArbol) malloc(sizeof(struct Nodo));
    long i, k = 0;
    bool Encontre = false;
    FILE* fa = fopen(PATH_ARBOL, "rb");
    i = RAIZ_ARBOL;
    if (!esVacio(A)){
        while ((i != -1) && (!Encontre)){
            fseek(fa, sizeof(struct Nodo)*i, SEEK_SET);
            fread(Q, sizeof(struct Nodo), 1, fa);
            k = Q->Datos.reg;
            if (cuit < Q->Datos.cuit){
                i = Q->HI;
            } else if (cuit > Q->Datos.cuit) {
                i = Q->HD;
            } else {
                Encontre = true;
            }
        }
    }
    free(Q);
    fclose(fa);
    if (Encontre) return k;
    else return -1;
}


void ActualizarNodo(PosicionArbol P, long pos){
    FILE* fa = fopen(PATH_ARBOL, "rb+");
    fseek(fa, sizeof(struct Nodo)*pos, SEEK_SET);
    fwrite(P,sizeof(struct Nodo), 1, fa);
    fclose(fa);
}

void BuscarClaveAReemplazar(long pos, long posPadre, PosicionArbol G, long* posA){
    PosicionArbol P = (PosicionArbol) malloc(sizeof(struct Nodo));
    PosicionArbol Q = (PosicionArbol) malloc(sizeof(struct Nodo));
    FILE* fa = fopen(PATH_ARBOL, "rb");
    fseek(fa, sizeof(struct Nodo)*pos, SEEK_SET);
    fread(P,sizeof(struct Nodo), 1, fa);
    if (P->HI != -1) BuscarClaveAReemplazar(P->HI, pos, G, posA);
    else {// Encontre el menor de los mayores
        if (posPadre != -1) {
            fseek(fa, sizeof(struct Nodo)*posPadre, SEEK_SET);
            fread(Q,sizeof(struct Nodo), 1, fa);
            Q->HI = P->HD;
            ActualizarNodo(Q, posPadre);
        }
        P->HD = G->HD;
        P->HI = G->HI;
        ActualizarNodo(P, pos);
        *posA = pos;
    }
    //free(P);
    //free(Q);
    fclose(fa);
}

void BuscarClaveAReemplazarRaiz(long pos, long posPadre, long* posA){
    PosicionArbol P = (PosicionArbol) malloc(sizeof(struct Nodo));
    PosicionArbol Q = (PosicionArbol) malloc(sizeof(struct Nodo));
    PosicionArbol R = (PosicionArbol) malloc(sizeof(struct Nodo));
    FILE* fa = fopen(PATH_ARBOL, "rb");
    fseek(fa, sizeof(struct Nodo)*pos, SEEK_SET);
    fread(P,sizeof(struct Nodo), 1, fa);
    if (P->HI != -1) BuscarClaveAReemplazarRaiz(P->HI, pos, posA);
    else { // Encontre el menor de los mayores
        fseek(fa, sizeof(struct Nodo)*0, SEEK_SET);
        fread(R,sizeof(struct Nodo), 1, fa);
        if (posPadre != -1) {
            fseek(fa, sizeof(struct Nodo)*posPadre, SEEK_SET);
            fread(Q,sizeof(struct Nodo), 1, fa);
            Q->HI = P->HD;
            ActualizarNodo(Q, posPadre);
        } else {
            R->HD = P->HD;
        }
        R->Datos = P->Datos;
        ActualizarNodo(R, 0);
        *posA = pos;
    }
    free(P);
    free(Q);
    free(R);
    fclose(fa);
}


void Eliminar(struct Arbol* A,long pos, long posPadre, long* cuit, bool* b){
    PosicionArbol Q = (PosicionArbol) malloc(sizeof(struct Nodo));
    PosicionArbol P = (PosicionArbol) malloc(sizeof(struct Nodo));
    long posClave, posAux = 0;
    FILE* fa = fopen(PATH_ARBOL, "rb");
    if (pos != -1) {
        fseek(fa, sizeof(struct Nodo)*pos, SEEK_SET);
        fread(Q,sizeof(struct Nodo), 1, fa);
        if (*cuit < Q->Datos.cuit){
            Eliminar(A, Q->HI, pos, cuit, b);
        } else if (*cuit > Q->Datos.cuit) {
            Eliminar(A, Q->HD, pos, cuit, b);
        } else {
            // ENCONTRé
            if (pos == 0) {             // el algoritmo se comporta distinto si borro la raiz
                if (A->q_items == 1){
                    printf("ENTROO\n");
                    Q->marca = '2';
                    ActualizarNodo(Q, 0);
                } else {
                    if (Q->HI == -1){ // Hijo Izquierdo Nulo
                        fseek(fa, sizeof(struct Nodo)*Q->HD, SEEK_SET);
                        fread(P, sizeof(struct Nodo), 1, fa);
                        posAux = Q->HD;// utilizo posAux para eliminar el nodo
                        Q->Datos = P->Datos;
                        Q->HD = P->HD;
                        Q->HI = P->HI;
                        ActualizarNodo(Q, pos);
                    } else if (Q->HD == -1) { // Hijo Derecho nulo
                        fseek(fa, sizeof(struct Nodo)*Q->HI, SEEK_SET);
                        fread(P, sizeof(struct Nodo), 1, fa);
                        posAux = Q->HI;
                        Q->Datos = P->Datos;
                        Q->HD = P->HD;
                        Q->HI = P->HI;
                        ActualizarNodo(Q, pos);
                    } else {// Tiene dos Hijos
                        BuscarClaveAReemplazarRaiz(Q->HD,-1, &posAux);
                        fseek(fa, sizeof(struct Nodo)*posAux, SEEK_SET);
                        fread(P, sizeof(struct Nodo), 1, fa);
                    }
                    P->marca = '2';
                    ActualizarNodo(P,posAux);
                    // Elimino el nodo logicamente
                }
                *b = true;
            } else {
                fseek(fa, sizeof(struct Nodo)*posPadre, SEEK_SET); // Leo el padre
                fread(P,sizeof(struct Nodo), 1, fa);
                if (Q->HI == -1){ // Hijo Izquierdo Nulo
                    if (P->Datos.cuit > Q->Datos.cuit) {
                        P->HI = Q->HD;
                    } else {
                        P->HD = Q->HD;
                    }
                } else if (Q->HD == -1) { // Hijo Derecho nulo
                    if (P->Datos.cuit > Q->Datos.cuit) {
                        P->HI = Q->HI;
                    } else {
                        P->HD = Q->HI;
                    }
                } else {// Tiene dos Hijos
                    BuscarClaveAReemplazar(Q->HD,-1,Q, &posClave);
                    //param:
                    //Hijo derecho de la clave que elimino. El Padre de la clave que buscamos, la inicializamos en -1 porque el primer "padre" no es valido. Posicion de la clave que elimino.

                    if (P->Datos.cuit > Q->Datos.cuit) {
                        P->HI = posClave;
                    } else {
                        P->HD = posClave;
                    }
                }
                // Actualizamos el padre
                ActualizarNodo(P,posPadre);

                // Elimino el nodo logicamente
                Q->marca = '2';
                ActualizarNodo(Q, pos);
            }
            *b = true;
        }
    }
    free(Q);
    free(P);
    fclose(fa);
}


void borrarNodo(struct Arbol* A, long cuit){
    if (esVacio(A)) {
        perror("Arbol Vacio.\n");
        exit(EXIT_FAILURE);
    } else {
        bool b = false;
        Eliminar(A,RAIZ_ARBOL,-1,&cuit,&b);
        if (!b){
            perror("Error al eliminar Nodo.\n");
            exit(EXIT_FAILURE);
        } else {
            A->q_items--;
        }
    }
}

void preOrden(long pos, FILE* fa, char* dist, char* tmp, int c){
    PosicionArbol P = (PosicionArbol) malloc(sizeof(struct Nodo));
    if (pos == -1) {
        printf("%s|\n%s +-----||\n", dist, dist);
        c++;
        if (c == 2) {
            dist = "";
            c = 0;
        }
    } else {
        fseek(fa, sizeof(struct Nodo)*pos, SEEK_SET); // Leo el nodo
        fread(P,sizeof(struct Nodo), 1, fa);
        if (pos == 0){
            printf("[%-11ld]\n", P->Datos.cuit);
        } else {
            printf("%s|\n%s +-----[%-11ld]\n", dist, dist, P->Datos.cuit);
        }
        tmp = dist;
        strcat(dist, "      ");
        preOrden(P->HI, fa, dist, tmp, c);
        preOrden(P->HD, fa, dist, tmp, c);
    }
    free(P);
}

void treeView(){
    FILE* fa = fopen(PATH_ARBOL, "rb");
    char dist[90] = "\0";
    char tmp[90] = "";
    preOrden(RAIZ_ARBOL,fa,dist,tmp, 0);
    fclose(fa);
}

void inOrd(long pos, FILE* fa, FILE* fd){
    PosicionArbol P = (PosicionArbol) malloc(sizeof(struct Nodo));
    struct Cliente* C = malloc(sizeof(struct Cliente));
    if (pos != -1){
        fseek(fa, sizeof(struct Nodo)*pos, SEEK_SET); // Leo el padre
        fread(P,sizeof(struct Nodo), 1, fa);
        fseek(fd, sizeof(struct Cliente)*P->Datos.reg, SEEK_SET);
        fread(C, sizeof(struct Cliente), 1, fd);
        inOrd(P->HI,fa,fd);
        printf("%-6ld  %-11ld\t%-12s\t%-9s\t%-9s\t%-10ld\t%-10ld\t%-20s\n",P->Datos.reg,C->cuit,C->razonSocial,C->localidad,C->dom,C->tel,C->cel,C->email);
        inOrd(P->HD,fa,fd);
    }
    free(P);

}

void inOrden(){
    FILE* fa = fopen(PATH_ARBOL, "rb");
    FILE* fd = fopen(PATH_ABM, "rb");
    printf("NroReg\tCuit\t\tRazon Social\tLocalidad\tDomicilio\tTelefono\tCelular\t\tE-mail\n");
    inOrd(RAIZ_ARBOL,fa,fd);
    fclose(fa);
    fclose(fd);
}
