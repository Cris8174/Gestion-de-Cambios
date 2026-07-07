#ifndef FUNCIONES_CAMBIOS_H
#define FUNCIONES_CAMBIOS_H

#include <stdio.h>
#include "Estructuras.h"

// Definición de contratos funcionales para los flujos operativos e interfaces de usuario
FILE* abrirOCrearDirector(char iddirector[5]);
void registrarSolicitud(Nodo **cabeza);
void buscarCambio(Nodo *cabeza);
void mostrarCambiosPendientes(Nodo *cabeza);
void generarListadoHistorico(Nodo *cabeza);
void evaluarCambio(Nodo *cabeza, int nuevo_estado);

#endif