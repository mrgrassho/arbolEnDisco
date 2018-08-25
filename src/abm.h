//
//  abm.h
//  Arbol en disco
//
//  Created by Victorio Scafati  on 9/17/17.
//  Copyright © 2017 Victorio Scafati. All rights reserved.
//

#ifndef abm_h
#define abm_h

#define PATH_ABM "files/abm.bin"

// Estructura de Archivo de Datos utilizada para la lectura en profundidad

struct Cliente {
    long cuit;
    char razonSocial[50];
    char dom[30];
    char localidad[30];
    char email[100];
    long tel, cel;
    char marca;
};

typedef struct Cliente Cliente;

extern void alta(const char* cuit);
extern void baja(const char* cuit);
extern void modi(const char* cuit);
extern void leerCliente(Cliente* );


#endif /* abm_h */
