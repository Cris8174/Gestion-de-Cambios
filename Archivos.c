#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Archivos.h"
#include "Lista.h"

// Escribe secuencialmente cada nodo de la lista en un archivo de texto plano
void guardarEnArchivo(Nodo *cabeza) {
    FILE *f = fopen(Archivocambios, "w"); // Abre el archivo en modo "w" para sobrescribir y refrescar toda la base de datos
    if (f == NULL) return; // Si ocurre un fallo de E/S, detiene el proceso de escritura de forma segura
    
    Nodo *temp = cabeza;
    // Recorre la lista nodo por nodo imprimiendo cada propiedad en una línea diferente
    while (temp != NULL) {
        fprintf(f, "%s\n", temp->dato.codigo);
        fprintf(f, "%s\n", temp->dato.solicitante);
        fprintf(f, "%s\n", temp->dato.descripcion);
        fprintf(f, "%d\n", temp->dato.riesgo);
        fprintf(f, "%d\n", temp->dato.impacto);
        fprintf(f, "%d\n", temp->dato.f_propuesta.dia);
        fprintf(f, "%d\n", temp->dato.f_propuesta.mes);
        fprintf(f, "%d\n", temp->dato.f_propuesta.anio);
        fprintf(f, "%d\n", temp->dato.estado);
        temp = temp->sig; // Pasa al nodo siguiente
    }
    fclose(f); // Cierra el descriptor de archivo para asegurar la escritura fidedigna en disco
}

// Lee el archivo de texto y reconstruye la lista en memoria dinámica al iniciar el sistema
void cargarDesdeArchivo(Nodo **cabeza) {
    FILE *f = fopen(Archivocambios, "r"); // Abre en modo "r" (lectura)
    if (f == NULL) return; // Si el archivo no existe (primera ejecución), termina sin hacer nada
    
    Cambio c;
    char buffer[150]; // Búfer temporal para convertir texto en enteros usando la función 'atoi'
    
    // Ciclo de lectura que extrae registros línea por línea hasta llegar al final del archivo (EOF)
    while (fgets(c.codigo, sizeof(c.codigo), f)) {
        c.codigo[strcspn(c.codigo, "\n")] = '\0'; // Limpia el salto de línea residual
        
        fgets(c.solicitante, sizeof(c.solicitante), f);
        c.solicitante[strcspn(c.solicitante, "\n")] = '\0';
        
        fgets(c.descripcion, sizeof(c.descripcion), f);
        c.descripcion[strcspn(c.descripcion, "\n")] = '\0';
        
        // Conversión obligatoria: Convierte las strings numéricas leídas del archivo a enteros nativos de C
        fgets(buffer, sizeof(buffer), f);   c.riesgo = atoi(buffer);
        fgets(buffer, sizeof(buffer), f);   c.impacto = atoi(buffer);
        fgets(buffer, sizeof(buffer), f);   c.f_propuesta.dia = atoi(buffer);
        fgets(buffer, sizeof(buffer), f);   c.f_propuesta.mes = atoi(buffer);
        fgets(buffer, sizeof(buffer), f);   c.f_propuesta.anio = atoi(buffer);
        fgets(buffer, sizeof(buffer), f);   c.estado = atoi(buffer);
        
        // Inserta la estructura reconstruida de forma ordenada al final de la lista en memoria RAM
        insertarAlFinal(cabeza, c);
    }
    fclose(f); // Cierra el archivo liberando los recursos del sistema operativo
}