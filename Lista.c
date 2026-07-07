#include <stdlib.h>
#include <string.h>
#include "Lista.h"

// Instancia un nuevo nodo reservando memoria dinámica en el Heap
Nodo* crearNodo(Cambio c) {
    Nodo *nuevo = (Nodo*)malloc(sizeof(Nodo)); // Reserva el bloque exacto que ocupa la estructura del Nodo
    if (nuevo != NULL) {
        nuevo->dato = c;    // Asigna la información estructurada de la solicitud
        nuevo->sig = NULL;  // Inicializa el puntero de avance en nulo
        nuevo->ant = NULL;  // Inicializa el puntero de retroceso en nulo
    }
    return nuevo; // Retorna la dirección de memoria asignada o NULL en caso de fallo
}

// Inserta de forma ordenada un cambio al extremo final de la lista doblemente enlazada
void insertarAlFinal(Nodo **cabeza, Cambio c) {
    Nodo *nuevo = crearNodo(c); // Llama al constructor dinámico del nodo
    if (nuevo == NULL) return;  // Si no hay memoria disponible, interrumpe el flujo de forma segura
    
    if (*cabeza == NULL) {
        *cabeza = nuevo;        // Si la lista está vacía, el nuevo nodo se transforma en el origen de la lista
    } else {
        Nodo *temp = *cabeza;   // Puntero auxiliar para recorrer la estructura sin alterar la cabeza original
        while (temp->sig != NULL) {
            temp = temp->sig;   // Desplazamiento iterativo secuencial hasta alcanzar el último nodo actual
        }
        temp->sig = nuevo;      // Conecta el puntero siguiente del antiguo último nodo hacia el nuevo nodo
        nuevo->ant = temp;      // Enlace hacia atrás: conecta el puntero anterior del nuevo nodo al viejo último
    }
}

// Recorre secuencialmente la lista liberando los bloques de memoria asignados para prevenir Memory Leaks
void liberarLista(Nodo *cabeza) {
    Nodo *temp;
    while (cabeza != NULL) {
        temp = cabeza;          // Resguarda la dirección del nodo actual
        cabeza = cabeza->sig;   // Avanza la cabeza al siguiente elemento antes de destruir el actual
        free(temp);             // Destruye y libera el espacio del bloque en el Heap de memoria
    }
}

// Busca colisiones de identificadores dentro de la lista para garantizar claves primarias únicas
int validarCodigoUnico(Nodo *cabeza, const char *codigo) {
    Nodo *temp = cabeza;
    while (temp != NULL) {
        if (strcmp(temp->dato.codigo, codigo) == 0) return 0; // Colisión encontrada: código duplicado (Falso)
        temp = temp->sig;                                      // Transición al siguiente elemento
    }
    return 1; // Código limpio y disponible para uso del sistema (Verdadero)
}