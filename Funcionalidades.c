#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Funcionalidades.h"

// Limpia la consola detectando dinámicamente el sistema operativo de ejecución
void limpiarPantalla() {
    #ifdef _WIN32
        system("cls");   // Comando nativo para entornos Windows
    #else
        system("clear"); // Comando nativo para entornos UNIX / Linux / macOS
    #endif
}

// Genera una pausa controlada en el flujo pidiendo la interacción del usuario
void pausar() {
    printf(CYAN "\nPresione Enter para continuar..." RESET);
    getchar(); // Captura el salto de línea pendiente para pausar la terminal
}

// Lee cadenas de caracteres de forma segura evitando desbordamientos de búfer (Buffer Overflow)
void leerCadena(char *destino, int max_len) {
    fgets(destino, max_len, stdin);               // Lee de la entrada estándar incluyendo espacios
    destino[strcspn(destino, "\n")] = '\0';       // Busca el carácter de nueva línea y lo reemplaza por el nulo final
}

// Realiza una validación lógica rigurosa sobre fechas comerciales e industriales
int validarFecha(Fecha f) {
    if (f.anio < 2026 || f.anio > 2100) return 0; // Delimita el rango de acción al marco del proyecto (año 2026+)
    if (f.mes < 1 || f.mes > 12) return 0;        // Verifica meses válidos del calendario gregoriano
    if (f.dia < 1 || f.dia > 31) return 0;        // Valida cota máxima genérica de días
    
    // Filtra los meses específicos que tienen un límite estricto de 30 días
    if (f.mes == 4 || f.mes == 6 || f.mes == 9 || f.mes == 11) {
        if (f.dia > 30) return 0;
    }
    // Lógica especializada para el tratamiento del mes de febrero (incluyendo años bisiestos)
    if (f.mes == 2) {
        int bisiesto = (f.anio % 4 == 0 && f.anio % 100 != 0) || (f.anio % 400 == 0);
        if (bisiesto && f.dia > 29) return 0;  // Año bisiesto: permite hasta el 29 de febrero
        if (!bisiesto && f.dia > 28) return 0; // Año común: permite hasta el 28 de febrero
    }
    return 1; // Retorna verdadero si superó con éxito todos los filtros anteriores
}