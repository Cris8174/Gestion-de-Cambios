#include <stdio.h>
#include <string.h>
#include "Funcionescambios.h"
#include "Lista.h"
#include "Archivos.h"
#include "Funcionalidades.h"

// Abre o inicializa las credenciales de seguridad del director validando el límite estricto de caracteres
FILE* abrirOCrearDirector(char iddirector[5]) {
    FILE *archivo = fopen(Archivodirector, "r"); // Intenta abrir el archivo de seguridad en modo lectura
    if (archivo == NULL) { // Si el archivo no existe, arranca la primera configuración obligatoria
        printf(RED "\n[SISTEMA] No existe credencial de director." RESET);
        
        int valido = 0;
        do {
            printf("\nRegistre la clave numerica del Director (MAXIMO 4 digitos): ");
            char buffer_id[100]; // Búfer amplio temporal para recolectar lo que sea que digite el usuario
            leerCadena(buffer_id, sizeof(buffer_id)); // Captura toda la entrada de forma controlada y segura
            
            int len = strlen(buffer_id);
            // Comprobación de límites: no puede estar vacío, ni superar los 4 caracteres permitidos
            if (len == 0) {
                printf(RED "Error: La clave no puede estar vacia.\n" RESET);
            } else if (len > 4) {
                printf(RED "Error: La clave supera los 4 caracteres permitidos.\n" RESET);
            } else {
                strcpy(iddirector, buffer_id); // Copia el búfer validado a la variable de destino de tamaño exacto [5]
                valido = 1;                     // Rompe el bucle de captura
            }
        } while (!valido);

        archivo = fopen(Archivodirector, "w"); // Crea físicamente el archivo de credenciales
        if (archivo != NULL) {
            fprintf(archivo, "%s", iddirector); // Registra de forma limpia la cadena validada de hasta 4 caracteres
            printf(GREEN "\nClave configurada en el sistema de manera exitosa.\n" RESET);
            fclose(archivo);
        }
        archivo = fopen(Archivodirector, "r"); // Reabre en modo lectura para acoplarse al flujo regular del software
    }
    return archivo; // Retorna el puntero del descriptor de archivo
}

// Despliega el formulario iterativo para dar de alta una nueva solicitud de cambio
void registrarSolicitud(Nodo **cabeza) {
    Cambio c;
    printf(MAGENTA "==========================================\n" RESET);
    printf(MAGENTA "         NUEVA SOLICITUD DE CAMBIO        \n" RESET);
    printf(MAGENTA "==========================================\n" RESET);
    
    // Captura iterativa con verificación lógica del identificador único
    do {
        printf("Codigo Unico del Cambio (ej. CHG-01): ");
        leerCadena(c.codigo, sizeof(c.codigo));
        if (!validarCodigoUnico(*cabeza, c.codigo)) {
            printf(RED "Error: El codigo ya esta registrado.\n" RESET);
        } else if (strlen(c.codigo) == 0) {
            printf(RED "Error: El codigo no puede estar vacio.\n" RESET);
        } else break; // El código es apto y cumple las restricciones del sistema
    } while (1);

    printf("Nombre del Solicitante: ");
    leerCadena(c.solicitante, sizeof(c.solicitante));
    
    printf("Descripcion del cambio: ");
    leerCadena(c.descripcion, sizeof(c.descripcion));
    
    // Menú iterativo para delimitar y capturar el nivel de riesgo del cambio propuesto
    do {
        printf("\nNivel de Riesgo:\n1. Bajo\n2. Medio\n3. Alto\n4. Critico\nSeleccione (1-4): ");
        if (scanf("%d", &c.riesgo) != 1) { c.riesgo = 0; }
        getchar(); // Purga el búfer
    } while (c.riesgo < 1 || c.riesgo > 4);

    // Menú iterativo para delimitar y capturar el nivel de impacto operativo
    do {
        printf("\nImpacto del Cambio:\n1. Bajo\n2. Medio\n3. Alto\n4. Critico\nSeleccione (1-4): ");
        if (scanf("%d", &c.impacto) != 1) { c.impacto = 0; }
        getchar(); // Purga el búfer
    } while (c.impacto < 1 || c.impacto > 4);

    if (c.riesgo == 1 && c.impacto == 1) {
        // [ITIL 4 - Cambio Estándar]: Bajo riesgo e impacto.
        // Se pre-aprueba automáticamente sin pasar por el flujo del Gerente.
        c.estado = 2; // 2 = Aprobado en tu sistema
        
        printf(GREEN "\n========================================================\n" RESET);
        printf("[ITIL 4] DETECTADO: Cambio Estandar (Bajo Riesgo/Impacto).\n");
        printf("-> El cambio ha sido PRE-APROBADO automaticamente.\n");
        printf("========================================================\n");
    } else {
        // [ITIL 4 - Cambio Normal / Emergencia]: Requiere revisión de la Autoridad.
        c.estado = 1; // 1 = Pendiente
        
        printf(YELLOW "\n========================================================\n" RESET);
        printf("[ITIL 4] DETECTADO: Cambio Normal / Critico.\n");
        printf("-> Registrado como PENDIENTE para revision del Gerente.\n");
        printf("========================================================\n");
    }

    // Sub-interfaz repetitiva para garantizar la exactitud cronológica de la propuesta de ejecución
    do {
        printf("\nFecha propuesta de implementacion:\n");
        printf("Dia (DD): "); if(scanf("%d", &c.f_propuesta.dia)!=1) c.f_propuesta.dia=0;
        printf("Mes (MM): "); if(scanf("%d", &c.f_propuesta.mes)!=1) c.f_propuesta.mes=0;
        printf("Anio (AAAA): "); if(scanf("%d", &c.f_propuesta.anio)!=1) c.f_propuesta.anio=0;
        getchar(); // Limpia el salto de línea de la última lectura numérica
        
        if (!validarFecha(c.f_propuesta)) {
            printf(RED "Fecha Invalida (Rangos validos: 2026-2100). Reintente.\n" RESET);
        } else break; // Estructura de fecha lógica confirmada
    } while (1);

    c.estado = 1; // Asignación por omisión del estado inicial: '1' significa 'Pendiente'
    insertarAlFinal(cabeza, c); // Inserta el bloque en la lista doblemente enlazada en ejecución
    guardarEnArchivo(*cabeza);   // Guarda en frío de forma inmediata los cambios en el disco duro
    printf(GREEN "\n[EXITO] Solicitud creada exitosamente.\n" RESET);
}

// Ejecuta una búsqueda secuencial filtrando por la clave primaria de cadena 'codigo'
void buscarCambio(Nodo *cabeza) {
    char b_codigo[12];
    printf("Ingrese el codigo de cambio a buscar: ");
    leerCadena(b_codigo, sizeof(b_codigo));
    
    Nodo *temp = cabeza;
    int encontrado = 0;
    while (temp != NULL) {
        if (strcmp(temp->dato.codigo, b_codigo) == 0) { // Comprobación binaria exacta de cadenas
            encontrado = 1;
            printf(GREEN "\n--- CAMBIO ENCONTRADO ---\n" RESET);
            printf("Codigo:      %s\n", temp->dato.codigo);
            printf("Solicitante: %s\n", temp->dato.solicitante);
            printf("Descripcion: %s\n", temp->dato.descripcion);
            // Mapeo dinámico en texto de los valores enteros de Riesgo, Impacto y Estado
            printf("Riesgo:      %s\n", (temp->dato.riesgo == 1) ? "Bajo" : (temp->dato.riesgo == 2) ? "Medio" : (temp->dato.riesgo == 3) ? "Alto" : "Critico");
            printf("Impacto:     %s\n", (temp->dato.impacto == 1) ? "Bajo" : (temp->dato.impacto == 2) ? "Medio" : (temp->dato.impacto == 3) ? "Alto" : "Critico");
            printf("Fecha:       %02d/%02d/%d\n", temp->dato.f_propuesta.dia, temp->dato.f_propuesta.mes, temp->dato.f_propuesta.anio);
            printf("Estado:      %s\n", (temp->dato.estado == 1) ? YELLOW "Pendiente" RESET : (temp->dato.estado == 2) ? GREEN "Aprobado" RESET : RED "Rechazado" RESET);
            break; // Rompe la iteración al hallar la coincidencia primaria única
        }
        temp = temp->sig; // Avanza el nodo
    }
    if (!encontrado) printf(RED "\n[ERROR] No se encontro ninguna coincidencia.\n" RESET);
}

// Filtra e imprime en pantalla un reporte tabular de todas las tareas en cola (estado == 1)
void mostrarCambiosPendientes(Nodo *cabeza) {
    Nodo *temp = cabeza;
    int cont = 0;
    printf(YELLOW "=========================================================================\n" RESET);
    printf(YELLOW "                     SOLICITUDES EN ESTADO PENDIENTE                     \n" RESET);
    printf(YELLOW "=========================================================================\n" RESET);
    printf("%-10s | %-15s | %-10s | %-10s | %-12s\n", "CODIGO", "SOLICITANTE", "RIESGO", "IMPACTO", "FECHA");
    printf("-------------------------------------------------------------------------\n");
    
    while (temp != NULL) {
        if (temp->dato.estado == 1) { // Verifica la propiedad condicional de estado pendiente
            char r[15], imp[15];
            strcpy(r, (temp->dato.riesgo == 1) ? "Bajo" : (temp->dato.riesgo == 2) ? "Medio" : (temp->dato.riesgo == 3) ? "Alto" : "Critico");
            strcpy(imp, (temp->dato.impacto == 1) ? "Bajo" : (temp->dato.impacto == 2) ? "Medio" : (temp->dato.impacto == 3) ? "Alto" : "Critico");
            
            // Imprime formateado con límites espaciales estrictos para mantener la alineación de la tabla
            printf("%-10s | %-15.15s | %-10s | %-10s | %02d/%02d/%d\n", 
                   temp->dato.codigo, temp->dato.solicitante, r, imp, 
                   temp->dato.f_propuesta.dia, temp->dato.f_propuesta.mes, temp->dato.f_propuesta.anio);
            cont++;
        }
        temp = temp->sig; // Avance del puntero de iteración
    }
    if (cont == 0) printf(GREEN "\nNo hay tareas pendientes en el tablero.\n" RESET);
}

// Genera una auditoría completa del histórico sin discriminación de estados de aprobación
void generarListadoHistorico(Nodo *cabeza) {
    Nodo *temp = cabeza;
    printf(BLUE "=========================================================================\n" RESET);
    printf(BLUE "                       REPORTE HISTORICO GENERAL                         \n" RESET);
    printf(BLUE "=========================================================================\n" RESET);
    printf("%-10s | %-15s | %-10s | %-20s | %-12s\n", "CODIGO", "SOLICITANTE", "RIESGO", "ESTADO EVALUACION", "FECHA");
    printf("-------------------------------------------------------------------------\n");
    
    while (temp != NULL) {
        char r[15], est[30];
        strcpy(r, (temp->dato.riesgo == 1) ? "Bajo" : (temp->dato.riesgo == 2) ? "Medio" : (temp->dato.riesgo == 3) ? "Alto" : "Critico");
        
        // Formatea el estado agregando códigos ANSI de color para fácil interpretación visual
        if (temp->dato.estado == 1) sprintf(est, YELLOW "Pendiente" RESET);
        else if (temp->dato.estado == 2) sprintf(est, GREEN "Aprobado" RESET);
        else sprintf(est, RED "Rechazado" RESET);

        printf("%-10s | %-15.15s | %-10s | %-29s | %02d/%02d/%d\n", 
               temp->dato.codigo, temp->dato.solicitante, r, est, 
               temp->dato.f_propuesta.dia, temp->dato.f_propuesta.mes, temp->dato.f_propuesta.anio);
        temp = temp->sig; // Avance del puntero
    }
}

// Permite dictaminar resoluciones operativas exclusivas del rol de Gerente
void evaluarCambio(Nodo *cabeza, int nuevo_estado) {
    char b_codigo[12];
    mostrarCambiosPendientes(cabeza); // Despliega el abanico actual de pendientes para guiar la selección
    printf("\nSeleccione el codigo de solicitud a dictaminar: ");
    leerCadena(b_codigo, sizeof(b_codigo));
    
    Nodo *temp = cabeza;
    int encontrado = 0;
    while (temp != NULL) {
        if (strcmp(temp->dato.codigo, b_codigo) == 0) {
            encontrado = 1;
            if (temp->dato.estado != 1) { // Regla de negocio: No se puede re-evaluar un ticket cerrado
                printf(RED "\n[ERROR] Esta solicitud ya fue resuelta previamente.\n" RESET);
            } else {
                temp->dato.estado = nuevo_estado; // Sobrescribe el estado con la decisión gerencial
                guardarEnArchivo(cabeza);           // Commitea y sincroniza los cambios en el archivo físico plano
                if (nuevo_estado == 2) printf(GREEN "\n[CAMBIO APLICADO] Solicitud aprobada.\n" RESET);
                else printf(RED "\n[CAMBIO APLICADO] Solicitud rechazada.\n" RESET);
            }
            break; // Detiene la exploración
        }
        temp = temp->sig; // Tránsito secuencial
    }
    if (!encontrado) printf(RED "\n[ERROR] Codigo no registrado.\n" RESET);
}