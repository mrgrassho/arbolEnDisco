#!/bin/bash

echo "Creando carpetas..."
if
  	mkdir files;
then
	echo "Carpetas creadas";
	touch files/abm.bin;
	touch files/arbolIndex.bin;
fi

echo "Compilando..."

if
	make;
	make arbolEnDisco;
then
	gcc -print-prog-name=arbolEnDisco;
	echo "Compilado correctamente."
fi
