#ifndef FUNCIONESCAMBIOS_H
#define FUNCIONESCAMBIOS_H

// Definición de macro-colores ANSI para personalizar las salidas en la consola de comandos
#define RESET   "\033[0m"
#define RED     "\033[0;31m"
#define GREEN   "\033[0;32m"
#define YELLOW  "\033[0;33m"
#define BLUE    "\033[0;34m"
#define MAGENTA "\033[0;35m"
#define CYAN    "\033[0;36m"

// Constantes de nombres de archivos físicos para persistencia de datos
#define Archivodirector "director.txt"
#define Archivocambios "cambios.txt"

// Estructura anidada para el manejo formal de fechas de implementación
typedef struct {
    int dia;
    int mes;
    int anio;
} Fecha;

// Estructura principal que almacena los atributos de una Solicitud de Cambio TI
typedef struct {
    char codigo[12];       // Código único identificador (ej: CHG-01)
    char solicitante[50];  // Nombre del empleado que genera la solicitud
    char descripcion[150]; // Detalle del cambio tecnológico propuesto
    int riesgo;            // Escala del riesgo: 1-Bajo, 2-Medio, 3-Alto, 4-Crítico
    int impacto;           // Escala del impacto en producción: 1-Bajo, 2-Medio, 3-Alto, 4-Crítico
    Fecha f_propuesta;     // Fecha planificada (Estructura interna)
    int estado;            // Flujo de estado: 1-Pendiente, 2-Aprobado, 3-Rechazado
} Cambio;

// Estructura de un Nodo para el manejo de una Lista Doblemente Enlazada
typedef struct Nodo {
    Cambio dato;           // Carga útil del nodo (información de la solicitud)
    struct Nodo *sig;      // Puntero de enlace hacia el siguiente elemento
    struct Nodo *ant;      // Puntero de enlace hacia el elemento anterior
} Nodo;

#endif