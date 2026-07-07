#ifndef PERSISTENCIA_H
#define PERSISTENCIA_H

#include "Estructuras.h"

// Definición de las funciones encargadas de leer y escribir en disco duro
void guardarEnArchivo(Nodo *cabeza);
void cargarDesdeArchivo(Nodo **cabeza);

#endif