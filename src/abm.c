//
//  abm.c
//  arbolEnDisco
//
//  Created by Victorio Scafati on 9/24/17.
//  Copyright © 2017 Victorio Scafati. All rights reserved.
//

#include <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stddef.h>
#include <ctype.h>
#include <locale.h>
#include <string.h>
#include "tree.h"
#include "abm.h"

bool esCorrectoFormato(char* cuit){
    bool flag = true;
    if (strlen(cuit) == 13){
        for (int i = 0; i < strlen(cuit); i++) {
            if ((i == 2) || (i == 11)){
                if (cuit[i] != '-') flag = false;
            } else
                if (!isdigit(cuit[i])) flag = false;
        }
    }
    return flag;
}

long leerCuit(const char* clave){
    char cuit[strlen(clave) + 1];
    strcpy(cuit, clave);
    char tmp[14];
    char* atencion = "\t\t\t\t[ATENCION]\n\tNo utilice espacios en los datos solicitados.";
    char* msj = "\nIngrese un cuit (ej. de formato: 30-34923823-9): ";
    printf("%s", atencion);
    while (!esCorrectoFormato(cuit)) {
        printf("\n[ERROR] Formato incorrecto, porfavor siga el formato establecido.%s", msj);
        scanf("%s", cuit);
    }
    strncpy(tmp, cuit, 2);
    strncpy(tmp+2, cuit+3, 8);
    strncpy(tmp+10, cuit+12, 1);
    tmp[13] = '\0';
    printf(" Cuit parseado: %s\n\n", tmp);
    return strtol(tmp, NULL, 10);
}

void leerCampo(char* campo, char* msj, int strMax, bool isNum, bool isMail){
    printf("\nIngrese %s: ", msj);
    scanf("%s", campo);
    bool flag = false;
    if (isNum) {
        while (!flag) {
            flag = true;
            for (int i = 0; i < strlen(campo); i++) {
                if (!isdigit(campo[i])) {
                    flag = false;
                    printf("[ERROR] Reingrese %s: ", msj);
                    scanf("%s", campo);
                    break;
                }
            }
            
        }
    } else if (isMail) { // Validacion para el mail y para domicilio no desarrollada
        while ((strlen(campo) > strMax)) {
            printf("[ERROR] Reingrese %s: ", msj);
            scanf("%s", campo);
        }
    } else {
        while ((!flag) || (strlen(campo) > strMax)){
            flag = true;
            for (int i = 0; i < strlen(campo); i++) {
                
                if (!isalpha(campo[i]) && !isdigit(campo[i])) {
                    flag = false;
                    printf("[ERROR NO ES ALFANUM o LONGITUD MAX ALCANZADA] Reingrese %s: ", msj);
                    scanf("%s", campo);
                    break;
                }
            }
        }
    }
}

void leerCliente(Cliente* cliente){
    cliente->dom[0] = '\0';
    cliente->localidad[0] = '\0';
    cliente->email[0] = '\0';
    cliente->tel = 0;
    cliente->cel = 0;
    char tmpTel[20] = "";
    char tmpCel[20] = "";
    leerCampo(cliente->razonSocial, "Razon Social", 50, false, false);
    leerCampo(cliente->dom, "Domicilio", 50, false, true);
    leerCampo(cliente->localidad, "Localidad", 30, false, false);
    leerCampo(tmpTel, "Telefono", 0, true, false);
    leerCampo(tmpCel, "Celular", 0, true, false);
    cliente->tel = strtol(tmpTel, NULL, 10);
    cliente->cel = strtol(tmpCel, NULL, 10);
    leerCampo(cliente->email, "e-mail", 100, false, true);//*/
}

void alta(const char* cuit){
    struct Arbol A;
    struct Cliente cliente;
    FILE* fp = fopen(PATH_ABM,"ab+");
    actualizarArbol(&A);                // actualiza info del arbol, raiz e items
    long clave = leerCuit(cuit);
    if (busquedaBinaria(&A, clave) == -1){
        cliente.cuit = clave;
        leerCliente(&cliente);
        cliente.marca = '1';
        fseek(fp, 0, SEEK_END);
        fwrite(&cliente, sizeof(struct Cliente), 1, fp);
        fseek(fp, 0, SEEK_END);
        long reg = ftell(fp)/sizeof(struct Cliente);
        reg = reg - 1;
        printf("Reg archivo num: %ld, tamano archivo: %ld, tamano reg: %ld\n",reg, ftell(fp), sizeof(struct Cliente));
        insertarNodo(&A, clave, reg);
        printf("El registro con el cuit %ld fue dado de alta correctamente.\n", clave);
    } else {
        printf("[Error] El registro con el cuit %ld ya existe.\n", clave);
    }
    fclose(fp);
}

void baja(const char* cuit){
    struct Arbol A;
    struct Cliente cliente;
    long reg;
    FILE* fp = fopen(PATH_ABM,"rb+");
    actualizarArbol(&A);                // actualiza info del arbol, raiz e items
    long clave = leerCuit(cuit);
    if ((reg = busquedaBinaria(&A, clave)) != -1){
        fseek(fp, sizeof(struct Cliente)*reg, SEEK_SET);
        fread(&cliente, sizeof(struct Cliente), 1, fp);
        cliente.marca = '0';
        fseek(fp, sizeof(struct Cliente)*reg, SEEK_SET);
        fwrite(&cliente, sizeof(struct Cliente), 1, fp);
        borrarNodo(&A, clave);
        printf("[+]El registro con el cuit %ld fue dado de baja correctamente.\n", clave);
    } else {
        printf("[Error] El registro con el cuit %ld NO existe.\n", clave);
    }
    fclose(fp);
}

void modificarCampo(struct Cliente* cliente){
    char tmpTel[20] = "", tmpCel[20] = "";
    printf("\t------------------------------------------------------------\n");
    printf("\t\t\t\tOPCIONES\n");
    printf("\t------------------------------------------------------------\n");
    printf(" 1 - Razon Social:\n");
    printf(" 2 - Localidad:\n");
    printf(" 3 - Domicilio:\n");
    printf(" 4 - Telefono:\n");
    printf(" 5 - Celular:\n");
    printf(" 6 - E-mail:\n");
    printf("   - Presione cualquier tecla para salir.\n");
    char opcion;
    scanf("%c", &opcion);
    switch (opcion) {
        case '1': leerCampo(cliente->razonSocial, "Razon Social", 50, false, false);
            break;
        case '2': leerCampo(cliente->localidad, "Localidad", 30, false, false);
            break;
        case '3': leerCampo(cliente->dom, "Domicilio", 50, false, false);
            break;
        case '4': leerCampo(tmpTel, "Telefono", 0, true, false);
            cliente->tel = strtol(tmpTel, NULL, 10);
            break;
        case '5': leerCampo(tmpCel, "Celular", 0, true, false);
            cliente->cel = strtol(tmpCel, NULL, 10);
            break;
        case '6': leerCampo(cliente->email, "e-mail", 100, false, true);
            break;
        default:
            perror("Usted salio del menu.");
            exit(EXIT_FAILURE);
            break;
    }
    
}

void modi(const char* cuit){
    struct Arbol A;
    struct Cliente cliente;
    long clave, reg;
    FILE* fp = fopen(PATH_ABM,"rb+");
    actualizarArbol(&A);                // actualiza info del arbol, raiz e items
    clave = leerCuit(cuit);
    if ((reg = busquedaBinaria(&A, clave)) != -1){
        fseek(fp, sizeof(struct Cliente)*reg, SEEK_SET);
        fread(&cliente, sizeof(struct Cliente), 1, fp);
        modificarCampo(&cliente);
        fseek(fp, sizeof(struct Cliente)*reg, SEEK_SET);
        fwrite(&cliente, sizeof(struct Cliente), 1, fp);
        printf("[+]El registro con el cuit %s fue modificado correctamente.\n", cuit);
    } else {
        printf("[Error] El registro con el cuit %ld NO existe.\n", clave);
    }
    fclose(fp);
}
