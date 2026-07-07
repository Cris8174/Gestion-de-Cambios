#ifndef LISTA_H
#define LISTA_H

#include "Estructuras.h"

// Prototipos necesarios para administrar la topología de la lista doblemente enlazada
Nodo* crearNodo(Cambio c);
void insertarAlFinal(Nodo **cabeza, Cambio c);
void liberarLista(Nodo *cabeza);
int validarCodigoUnico(Nodo *cabeza, const char *codigo);

#endif