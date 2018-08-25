
	1. ### Como compilar?
	```
			make arbolEnDisco
	```

	2. ### Modo de ejecución
	```
		./arbolEnDisco -opción

		-a <cuit>		Dar de alta un cuit.
		-b <cuit>		Dar de baja un cuit.
		-m <cuit>		Modificar un cuit.
		-r          Resetear los archivos. Sobrescribe todos los archivos
								tanto el de indice como los datos, dejándolos vacíos.
		-t          Mostrar archivo de datos. Realiza una recorrida inOrden sobre
								el árbol y muestra toda la información sobre cada registro.
	```
	
	3. ### Scripts Adicionales

	   Ejecutar el script “/scripts/testArbol.sh”, para ingresar cuits precargados.

		 ***ATENCION*** la carga no es automatica pero la facilita.

	4. ### Ha ocurrido algun error o quieres recompilarlo?
	 	```
	 			make clean
				make arbolEnDisco
	 	```
	——————————————————————————————————————————————————————————————————————————————————————

	@autor: Victorio Scafati
	@testeado en clang: clang-802.0.42, Target: x86_64-apple-darwin16.7.0
	@testeado en gcc: version 6.3.0 20170516 (Debian 6.3.0-18), Target: x86_64-linux-gnu
	@testeado en gcc: version 5.4.0 20160609 (Ubuntu 5.4.0-6ubuntu1~16.04.10)
