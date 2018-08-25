//
//  main.c
//  Lista(enDisco)
//
//  Created by Victorio Scafati on 8/24/17.
//  Copyright © 2017 Victorio Scafati. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "tree.h"
#include "abm.h"

/*
    TO-DO list:
    
    SOLVED -    Agregar inOrden
    SOLVED -    Revisar las altas.
    SOLVED -    Revisar las bajas.
    SOLVED -    Hacer script para automatizar altas
    SOLVED -    Agregar un reset de los archivos para no generar conflictos
    SOLVED -    Revisar las opciones de las modificaciones
           -    Agregar treeView, muestre como esta formada la estructura del arbol
           -    Agregar busqueda individual de cuits
           -    Plantear solucion a la concurrencia
                (usando un lock a nivel de archivos, cambiando f-io por io sin buffer (ie. fopen por open), etc)
*/

void usage();
void reset();

int main(int argc, const char * argv[]) {
    # ifdef CODE_TEST
    freopen("output.txt", "w+", stdout);
    # endif
    struct Arbol A;
    actualizarArbol(&A);
    if (argc == 2){
        if ((argv[1][1] != 't') && (argv[1][1] != 'r'))  {
            usage();
            exit(1);
        }
    }
    
    if (argc >= 2){
        switch (argv[1][1]) {
            case 'a':
                alta(argv[2]);
                break;
            case 'b':
                baja(argv[2]);
                break;
            case 'm':
                modi(argv[2]);
                break;
            case 't':
                if (!esVacio(&A)) inOrden();
                else {
                        reset();
                        perror("Arbol Vacio.");
                }
                break;
            case 'r':
                reset();
                printf("Archivos reseteados correctamente.\n");
                break;
            default:
                usage();
                break;
        }
    } else {
        usage();
        exit(1);
    }
    # ifdef CODE_TEST
    fclose(stdout);
    # endif
    return 0;
}

void usage(){
    printf("Modo de Uso:\n");
    printf("\t-a <cuit>\tDar de alta un <cuit>\n");
    printf("\t-b <cuit>\tDar de baja un <cuit>\n");
    printf("\t-m <cuit>\tModificar un <cuit>\n");
    printf("\t-r       \tResetear los archivos.\n");
    printf("\t-t       \tMostrar archivo de datos.\n");
}

void reset(){
    FILE* fp = fopen(PATH_ABM,"w");
    FILE* fa = fopen(PATH_ARBOL,"w");
    fclose(fa);
    fclose(fp);
}



