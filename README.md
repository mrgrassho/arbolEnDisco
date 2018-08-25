# ¿Que hace esto?

Es un programa que a partir de una estructura hardcodeada construye un indice (sobre un archivo) con una estructura de [árbol binario AVL](https://es.wikipedia.org/wiki/%C3%81rbol_AVL).

### Modo de compilación
```make
make arbolEnDisco
```
Dentro de este __makefile__ esta toda la configuración necesaria.

### Modo de ejecución
```bash
./arbolEnDisco -opcion
```
### Opciones(-opcion):
*  `-a <cuit>` Dar de alta un cuit.
*  `-b <cuit>` Dar de baja un cuit.
*  `-m <cuit>` Modificar un cuit.
*  `-r` Resetear los archivos binarios. Sobrescribe todos los archivos tanto el de indice como los datos, dejándolos vacíos.
*  `-t` Mostrar archivo de datos. Realiza una recorrida inOrden sobre el árbol y muestra toda la información sobre cada registro.

### Scripts Adicionales

* **testArbol.sh** ingresa cuits precargados. ***ATENCION*** la carga no es automatica, para cada cuit es necesario ingresar los datos requeridos.

#### Ejecucion
```
chmod 755 testArbol.sh
./testArbol.sh
```

* **testBajas** elimina los cuits ingresados por el script **testArbol.sh**

#### Ejecucion
```
chmod 755 testBajas.sh
./testBajas.sh
```

### ¿PROBLEMAS? ¿Ha ocurrido algún error o queres recompilarlo?
```make
make clean
make arbolEnDisco
```

### TO-DO list:

- [**DONE**] Agregar inOrden
- [**DONE**] Revisar las altas.
- [**DONE**] Revisar las bajas.
- [**DONE**] Hacer script para automatizar altas
- [**DONE**] Agregar un reset de los archivos para no generar conflictos
- [**DONE**]  Revisar las opciones de las modificaciones
- [**TO-DO**] Agregar treeView, muestre como esta formada la estructura del arbol
- [**TO-DO**] Agregar busqueda individual de cuits
- [**TO-DO**] Plantear solucion a la concurrencia
						(usando un lock a nivel de archivos, cambiando f-io por io sin buffer (e.g. fopen por open), etc)
- [**TO-DO**] Hacer que el ingreso de sea desde el cmd, sin necesidad de utilizar funciones como scanf().
- [**TO-DO**] Realizar una estrcutura adicional que sea adaptativa. Meta data sobre la estructura almacenada.

# Información

@autor: mr.grassho
@testeado en clang: clang-802.0.42, Target: x86_64-apple-darwin16.7.0
@testeado en gcc: version 6.3.0 20170516 (Debian 6.3.0-18), Target:
x86_64-linux-gnu
@testeado en gcc: version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.10)
