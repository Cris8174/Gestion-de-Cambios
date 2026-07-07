#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Estructuras.h"
#include "Funcionalidades.h"
#include "Lista.h"
#include "Archivos.h"
#include "Funcionescambios.h"

int main() {
    Nodo *listaCambios = NULL; // Puntero de control raíz de la lista (inicializado en vacío)
    cargarDesdeArchivo(&listaCambios); // Deserializa los datos históricos del disco y los monta en memoria RAM
    
    int rol = 0;
    char id_ingresado[100], id_correcto[5]; // Se expandió 'id_ingresado' a 100 para recibir de forma segura entradas largas sin romper la pila
    
    // Bucle primario de autenticación y selección de privilegios de acceso
    do {
        limpiarPantalla();
        printf(CYAN "==========================================\n" RESET);
        printf(CYAN "     SISTEMA DE CONTROL DE CAMBIOS TI     \n" RESET);
        printf(CYAN "==========================================\n" RESET);
        printf("1. Entrar como Gerente (Director)\n");
        printf("2. Entrar como Empleado\n");
        printf("3. Apagar\n");
        printf("Seleccione rol: ");
        if (scanf("%d", &rol) != 1) { rol = 0; }
        getchar(); // Purga obligatoria del salto de línea que deja 'scanf' en el búfer
        
        if (rol == 3) {
            liberarLista(listaCambios); // Apagado controlado: libera la memoria dinámica para dejar el Heap limpio
            return 0;
        }
        if (rol == 1) {
            FILE *f_gerente = abrirOCrearDirector(id_correcto); // Valida y obtiene la credencial guardada
            if (f_gerente != NULL) {
                fscanf(f_gerente, "%4s", id_correcto); // Extrae los hasta 4 caracteres registrados
                fclose(f_gerente);
            }
            
            printf("Ingrese ID de Gerente: ");
            leerCadena(id_ingresado, sizeof(id_ingresado)); // Captura limpia sin fugas del búfer de consola
            
            // Compara la clave ingresada con la almacenada en los registros del archivo del sistema
            if (strcmp(id_ingresado, id_correcto) == 0) {
                printf(GREEN "\nAcceso concedido Administrador.\n" RESET);
                pausar(); break; // Supera el control de acceso y entra a la interfaz
            } else {
                printf(RED "\nID incorrecto.\n" RESET);
                pausar(); rol = 0; // Reinicia el rol para obligar un reintento
            }
        } else if (rol == 2) {
            printf(GREEN "\nAcceso concedido Operador.\n" RESET);
            pausar(); break; // Los empleados generales saltan el bypass de autenticación por contraseña
        } else {
            printf(RED "\nFuera de rango.\n" RESET);
            pausar();
        }
    } while (rol < 1 || rol > 3);

    int opcionMenu;
    // Bucle operativo secundario del Menú de Funcionalidades del Sistema
    do {
        limpiarPantalla();
        if (rol == 1) { // Despliegue de capacidades completas del perfil Gerencial
            printf(GREEN "==========================================\n" RESET);
            printf(GREEN "             INTERFAZ GERENTE             \n" RESET);
            printf(GREEN "==========================================\n" RESET);
            printf("1. Registrar Solicitud de Cambio\n");
            printf("2. Buscar Cambio por Codigo\n");
            printf("3. Mostrar Cambios Pendientes\n");
            printf("4. Generar Listado Historico\n");
            printf("5. Aprobar Solicitud\n");
            printf("6. Rechazar Solicitud\n");
            printf("7. Salir del Sistema\n");
        } else { // Despliegue con restricciones físicas para la Interfaz General del Operador
            printf(YELLOW "==========================================\n" RESET);
            printf(YELLOW "            INTERFAZ EMPLEADO             \n" RESET);
            printf(YELLOW "==========================================\n" RESET);
            printf("1. Registrar Solicitud de Cambio\n");
            printf("2. Buscar Cambio por Codigo\n");
            printf("3. Mostrar Cambios Pendientes\n");
            printf("4. Generar Listado Historico\n");
            printf(RED "--- [Acciones Gerenciales Bloqueadas] ---\n" RESET);
            printf("7. Salir del Sistema\n");
        }
        
        printf("Opcion: ");
        if(scanf("%d", &opcionMenu) != 1) { opcionMenu = 0; }
        getchar(); // Limpieza del flujo de entrada estándar de consola
        limpiarPantalla(); 
        
        // Enrutamiento de llamadas modulares mediante Máquina de Estados (Switch-Case)
        switch (opcionMenu) {
            case 1: registrarSolicitud(&listaCambios); break;
            case 2: buscarCambio(listaCambios); break;
            case 3: mostrarCambiosPendientes(listaCambios); break;
            case 4: generarListadoHistorico(listaCambios); break;
            case 5:
                if (rol == 1) evaluarCambio(listaCambios, 2); // 2: Estado Aprobado
                else printf(RED "Bloqueado: Funcion exclusiva del Gerente.\n" RESET);
                break;
            case 6:
                if (rol == 1) evaluarCambio(listaCambios, 3); // 3: Estado Rechazado
                else printf(RED "Bloqueado: Funcion exclusiva del Gerente.\n" RESET);
                break;
            case 7: printf("Guardando registros...\n"); break;
            default: printf(RED "Opcion no valida.\n" RESET); break;
        }
        if (opcionMenu != 7) pausar(); // Pausa informativa entre llamadas de acciones del menú
    } while (opcionMenu != 7);

    liberarLista(listaCambios); // Recolección de basura manual: vacía las estructuras de datos de la RAM antes de concluir
    printf(GREEN "Cierre completado sin fugas de memoria.\n" RESET);
    return 0;
}