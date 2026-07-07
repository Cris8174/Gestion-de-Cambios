#ifndef FUNCIONALIDADES_H
#define FUNCIONALIDADES_H

#include "Estructuras.h"

// Firmas de funciones auxiliares globales reutilizables en los módulos
void limpiarPantalla();
void pausar();
void leerCadena(char *destino, int max_len);
int validarFecha(Fecha f);

#endif