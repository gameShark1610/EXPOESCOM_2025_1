/*
Supermercado.c
V 1.0 Mayo 2025
Autor: Escompiladores

Programa de simulacion de un supermercado
EL programa debe de simular la atencion de personas en un supermercado con diferentes velocidades de 
atencion para cada caja bajo un numero constante de llegada de clientes, el programa cierra si no hay
clientes por atender y se han atendido 100 clientes por lo menos

Compilación
    Windows:
    gcc presentacionWin.c TADColaDin.c Market.c Titulos.c -o Market.exe

    Ejemplo de entrada: 
    ESCOM
    5
    10000
    10000
    10000
    10000
    10000
    1000
    Market.exe
Parametros de entrada: Ninguno
Esto ejecuta el programa y pedira los requisitos para comenzar
*/

//Librerias estandar
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include <time.h>
 
//Librerias propias
#include "TADColaDin.h"
#include "Titulos.h"
#include "supermercado.h"
#include "presentacion.h"

//Constantes de control
#define META 100
#define TIEMPO_BASE 3

//Declarar prototipos de funciones

        //Funciones de control
        void GuardarDatos(caja *info_caja, int num_caja);
        void ControlarMercado(caja *info_caja, int num_caja, int *atendidos, int llegada_clientes);
        int HayClientesEnEspera(caja *info_caja, int num_caja);
        int AsignarFila(caja *info_caja, int num_caja);
        int HayCajasOcupadas(caja *info_caja, int num_caja);

        //Parte "dinamica"
        void RecorrerFilaVisual(caja *info_caja, int caja);  
        void MostrarExcesoClientes(int fila_actual, int columna, int exceso_anterior, int exceso_actual);

        //Funciones de dibujo
        void DibujarClienteLLegada(int num_cliente);
        void DibujarClienteFila(int num_cliente, caja *info_caja, int caja);
        void DibujarClienteEnAtencion(int num_cliente, caja *info_caja, int caja);

        //Funciones de borrado
        void BorrarClienteLLegada(int num_cliente);
        void BorrarClienteEnAtencion(caja *info_caja, int caja);
        void BorrarClienteFila(caja *info_caja, int i);

/*
    void main()
    Recibe: Nada directamente, aunque solicita datos al usuario durante la ejecución.
    Devuelve: Nada.
    Observaciones: Inicia el programa solicitando parámetros al usuario, configura el entorno gráfico y lanza la simulación del supermercado.
*/

void main (){
    char nombre_tienda[15];
    int num_cajas;
    int *atendidos;
    caja *info_cajas;
    int llegada_clientes;
    atendidos=(int *)malloc(1*sizeof(int));
    *atendidos = 0;


    //Solicitud de datos
            //printf("\nIngrese el nombre del supermercado: ");
            gets(nombre_tienda);
            scanf("%d",&num_cajas);
            info_cajas = (caja *)malloc(num_cajas  * sizeof(caja));
            if (info_cajas == NULL) {
                printf("Error: no se pudo asignar memoria para las cajas.\n");
                exit(1);
}
            GuardarDatos(info_cajas,num_cajas);
            scanf("%d",&llegada_clientes);
            BorrarPantalla();
            //Funciones de diseno en la libreria de titulos.h
            DibujarContorno();
            DibujarTituloCentrado(nombre_tienda);
            DibujarCajas(num_cajas,info_cajas);
           
            //Funcionamiento principal del programa
            ControlarMercado(info_cajas,num_cajas,atendidos,llegada_clientes);
            MostrarAnuncioCierre();
            
}

/*
    void GuardarDatos(caja *info_caja, int num_caja)
    Recibe: Un arreglo de estructuras de tipo caja y la cantidad total de cajas.
    Devuelve: Nada.
    Observaciones: Solicita al usuario el tiempo de atención para cada caja, valida que sea múltiplo de 10 y establece valores iniciales.
*/

void GuardarDatos(caja *info_caja, int num_caja){
  
    for (int i = 0; i < num_caja; i++) {
        //printf("Caja %d:\n", i + 1);
        //printf("  Tiempo de atención (ms): ");
        scanf("%d", &info_caja[i].tiempo_atencion);
        if (info_caja[i].tiempo_atencion % 10 != 0) {
            printf("\nError: El tiempo de atención de la caja %d debe ser múltiplo de 10.\n", i + 1);
            exit(1);
        }
        info_caja[i].atendiendo = 0;
        info_caja[i].tiempo_restante = 0;
        info_caja[i].cont_fila=0;
        Initialize(&info_caja[i].fila);
    }
}


/*
    void ControlarMercado(caja *info_caja, int num_caja, int *atendidos, int llegada_clientes)
    Recibe:
        - info_caja: Arreglo con la información de todas las cajas.
        - num_caja: Número total de cajas en el sistema.
        - atendidos: Puntero al contador de clientes atendidos.
        - llegada_clientes: Intervalo de tiempo (en milisegundos) entre la llegada de nuevos clientes.
    Acción:
        - Controla el ciclo de simulación del mercado: llegadas, atención y actualización visual.
        - Los clientes llegan periódicamente y son asignados a una caja disponible o encolados.
        - Las cajas atienden a un cliente durante un tiempo fijo, y luego pasan al siguiente en fila si existe.
    Observaciones:
        - La simulación termina cuando se cumple la meta de atención y no hay clientes en espera ni cajas ocupadas.
        - El cliente es atendido directamente si la caja está libre y su fila está vacía.
        - Se actualiza visualmente cada acción: llegada, movimiento, atención, y finalización.
        - Usa `EsperarMiliSeg(10)` para controlar el tiempo entre ciclos de ejecución.
*/
void ControlarMercado(caja *info_caja, int num_caja, int *atendidos, int llegada_clientes) {
    int tiempo = 0;
    int num_cliente = 0;
    elemento cliente;

    // El ciclo continúa hasta que se alcance la meta de clientes atendidos, no haya clientes esperando en fila y todas las cajas estén libres.
    while (!(*atendidos >= META && !HayClientesEnEspera(info_caja, num_caja) && !HayCajasOcupadas(info_caja, num_caja))) {

        // 1. Llegada de un nuevo cliente
        if (tiempo % llegada_clientes == 0 && tiempo !=0) {
            num_cliente++;
            DibujarClienteLLegada(num_cliente);

            int caja_asignada = AsignarFila(info_caja, num_caja);
            BorrarClienteLLegada(num_cliente);
            cliente.id = num_cliente;

            if (!info_caja[caja_asignada].atendiendo && Empty(&info_caja[caja_asignada].fila)) {
                // La caja está libre y la fila vacía: atender directamente
                info_caja[caja_asignada].atendiendo = 1;
                info_caja[caja_asignada].tiempo_restante = info_caja[caja_asignada].tiempo_atencion;
                DibujarClienteEnAtencion(cliente.id, info_caja, caja_asignada);
            } else {
                // Hay alguien en atención o fila: poner en cola
                Queue(&info_caja[caja_asignada].fila, cliente);
                DibujarClienteFila(cliente.id, info_caja, caja_asignada);
            }
        }

        // 2. Revisión de atención en cada caja
        for (int i = 0; i < num_caja; i++) {
            if (info_caja[i].atendiendo) {
                info_caja[i].tiempo_restante-=10;

                if (info_caja[i].tiempo_restante <= 0) {
                    // Cliente terminó su atención
                    BorrarClienteEnAtencion(info_caja, i);
                    (*atendidos)++;
                    MostrarNumeroClientesAtendidos(*atendidos);

                    if (!Empty(&info_caja[i].fila)) {
                        cliente = Dequeue(&info_caja[i].fila);
                       // BorrarClienteFila(info_caja, i);  // Borra el primero de la fila visible
                        RecorrerFilaVisual(info_caja, i);
                        DibujarClienteEnAtencion(cliente.id, info_caja, i);
                        info_caja[i].tiempo_restante = info_caja[i].tiempo_atencion;
                    } else {
                        info_caja[i].atendiendo = 0;
                    }
                }
            }
        }

        EsperarMiliSeg(10);
        tiempo=tiempo+10;
    }
}

/*
    int AsignarFila(caja *info_caja, int num_caja)
    Recibe: El arreglo con la información de las cajas y el número total de cajas.
    Devuelve: El índice de la caja a la que se asignará el siguiente cliente.
    Acción:
        - Identifica las cajas que no están atendiendo (disponibles).
        - Si hay al menos una caja libre, selecciona aleatoriamente entre ellas.
        - Si todas las cajas están ocupadas, asigna aleatoriamente entre todas.
    Observaciones:
        - Usa memoria dinámica temporalmente para guardar los índices disponibles.
        - Inicializa el generador aleatorio (`srand`) solo una vez usando una variable estática.
        - Termina el programa si no se puede asignar memoria.
*/
int AsignarFila(caja *info_caja, int num_caja) {
    int total_disponibles = 0;
    int indice_asignado;
    int *disponibles = (int *)malloc(num_caja * sizeof(int));
    if (disponibles == NULL) {
    printf("Error al asignar memoria.\n");
    exit(1);
}

    // Verificar cajas libres
    for (int i = 0; i < num_caja; i++) {
        if (info_caja[i].atendiendo == 0) {
            disponibles[total_disponibles++] = i;  // Guardamos el índice de la caja libre
        }
    }

    static int inicializado = 0;
    if (!inicializado) {
        srand(time(NULL));
        inicializado = 1;
    }


    if (total_disponibles > 0) {
        indice_asignado = disponibles[rand() % total_disponibles];
    } else {
        indice_asignado = rand() % num_caja;
    }

    return indice_asignado; // Devuelve el índice de la caja asignada
}

/*
    int HayClientesEnEspera(caja *info_caja, int num_caja)
    Recibe: El arreglo con la información de las cajas y el número total de cajas.
    Devuelve: 1 si al menos una caja tiene clientes en espera, 0 en caso contrario.
    Observaciones: Revisa todas las colas de las cajas para detectar si hay elementos.
*/

int HayClientesEnEspera(caja *info_caja, int num_caja) {
    for (int i = 0; i < num_caja; i++) {
        if (!Empty(&info_caja[i].fila)) return 1;
    }
    return 0;
}

/*
    int HayCajasOcupadas(caja *info_caja, int num_caja)
    Recibe: El arreglo con la información de las cajas y el número total de cajas.
    Devuelve: 1 si al menos una caja está atendiendo a un cliente, 0 en caso contrario.
    Observaciones: Evalúa el estado de atención de cada caja.
*/

int HayCajasOcupadas(caja *info_caja, int num_caja) {
    for (int i = 0; i < num_caja; i++) {
        if (info_caja[i].atendiendo) return 1;
    }
    return 0;
}

/*
    void RecorrerFilaVisual(caja *info_caja, int caja_idx)
    Recibe: El arreglo con la información de las cajas y el índice de la caja correspondiente.
    Acción: Recorre visualmente la fila de clientes de la caja en la consola, mostrando los primeros 3 clientes.
    Observaciones:
        - Si hay más de 3 clientes en la fila, muestra un contador de exceso en una posición definida.
        - Los clientes se imprimen como "C" seguido de su ID.
        - Borra las posiciones vacías si hay menos de 3 clientes visibles.
        - Utiliza fflush(stdout) para asegurar que los cambios se reflejen inmediatamente en pantalla.
*/
void RecorrerFilaVisual(caja *info_caja, int caja_idx) {
    int x = info_caja[caja_idx].inicio_fila.x;
    int y = info_caja[caja_idx].inicio_fila.y;
    int tamano_cola = Size(&info_caja[caja_idx].fila);

    for (int i = 0; i < 3; i++) {
        MoverCursor(x, y + i);
        if (i < tamano_cola) {
            elemento e = Element(&info_caja[caja_idx].fila, i + 1);
            printf("C%d", e.id);
        } else {
            printf("   ");
        }
    }

    int exceso_actual = 0;
    if (tamano_cola > 3) {
        exceso_actual = tamano_cola - 3;
    }

    MostrarExcesoClientes(info_caja[caja_idx].pos_exceso.y,
                          info_caja[caja_idx].pos_exceso.x,
                          info_caja[caja_idx].exceso_anterior,
                          exceso_actual);

    info_caja[caja_idx].exceso_anterior = exceso_actual;
    info_caja[caja_idx].cont_fila = tamano_cola;

    fflush(stdout);
}


/*
    void MostrarExcesoClientes(int fila_actual, int columna, int exceso_anterior, int exceso_actual)
    Recibe: La fila y columna donde se mostrará el contador de exceso, el número anterior y el actual de clientes en exceso.
    Observaciones: Si hay exceso actual, lo muestra como "+n". Si ya no hay exceso pero antes sí, borra el contador.
*/
void MostrarExcesoClientes(int fila_actual, int columna, int exceso_anterior, int exceso_actual) {
    MoverCursor(columna, fila_actual);
    if (exceso_actual > 0) {
       
             printf("+%d", exceso_actual);
        
    } else if (exceso_anterior > 0) {
        printf("    :");
    }
    fflush(stdout); // Fuerza la impresión inmediata en consola, asegurando que el texto se muestre sin esperar al búfer.
}


/*
    void DibujarClienteLLegada(int num_cliente)
    Recibe: El número identificador del cliente que está llegando.
    Acción: Muestra temporalmente al cliente en la posición de entrada de la tienda.
    Observaciones: Se posiciona en la consola y escribe "C#" simulando la llegada del cliente.
*/
void DibujarClienteLLegada(int num_cliente) {
    MoverCursor(38, 22);
    printf("C%d", num_cliente);
    EsperarMiliSeg(1000);
}
/*
    void BorrarClienteLLegada(int num_cliente)
    Recibe: El número identificador del cliente que acaba de ser asignado a una fila.
    Acción: Borra visualmente al cliente de la posición de entrada.
    Observaciones: Limpia la zona donde se mostró al cliente antes de enviarlo a una caja.
*/
void BorrarClienteLLegada(int num_cliente) {
    MoverCursor(38, 22);
    printf("     ");  // Borra el texto
}

/*
    void DibujarClienteFila(int num_cliente, caja *info_caja, int caja_idx)
    Recibe: El número identificador del cliente, el arreglo de información de cajas y el índice de la caja asignada.
    Acción: Dibuja al cliente en la fila correspondiente si hay espacio visible; si no, actualiza el contador de exceso.
    Observaciones: Muestra hasta 3 clientes visualmente. Si hay más, se muestra un contador de exceso fuera de la fila.
*/
void DibujarClienteFila(int num_cliente, caja *info_caja, int caja_idx) {
    int y = info_caja[caja_idx].inicio_fila.y + info_caja[caja_idx].cont_fila;

    int exceso_actual = 0;
    if (info_caja[caja_idx].cont_fila >= 3) {
        exceso_actual = info_caja[caja_idx].cont_fila - 2;
        MostrarExcesoClientes(info_caja[caja_idx].pos_exceso.y, 
                              info_caja[caja_idx].pos_exceso.x, 
                              info_caja[caja_idx].exceso_anterior, 
                              exceso_actual);
    }

    if (info_caja[caja_idx].cont_fila < 3) {
        MoverCursor(info_caja[caja_idx].inicio_fila.x, y);
        printf("C%d", num_cliente);
    }

    info_caja[caja_idx].cont_fila++;
    info_caja[caja_idx].exceso_anterior = exceso_actual;
}

/*
    void BorrarClienteFila(caja *info_caja, int i)
    Recibe: El arreglo de información de cajas y el índice de la caja desde la que se atiende un cliente.
    Acción: Borra visualmente al primer cliente visible de la fila.
*/
void BorrarClienteFila(caja *info_caja, int i) {
    int x_base = info_caja[i].atencion.x + 2;  // Supongamos que la atención está en x, fila inicia en x + 2
    int y = info_caja[i].atencion.y;

    MoverCursor(x_base, y);
    printf(" "); // Borra el primer caracter de la fila visible
    fflush(stdout);
}

/*
    void DibujarClienteEnAtencion(int num_cliente, caja *info_caja, int caja)
    Recibe: El número de cliente, el arreglo con la información de las cajas y el índice de la caja correspondiente.
    Observaciones: Imprime el identificador del cliente en la posición específica de atención de la caja.
*/
void DibujarClienteEnAtencion(int num_cliente, caja *info_caja, int caja) {
    MoverCursor(info_caja[caja].atencion.x, info_caja[caja].atencion.y);
    printf("C%d", num_cliente);
}

/*
    void BorrarClienteEnAtencion(caja *info_caja, int caja)
    Recibe: El arreglo con la información de las cajas y el índice de la caja correspondiente.
    Acción: Elimina de la consola al cliente que terminó su atención en la caja especificada.
    Observaciones: Borra solo el área ocupada por el cliente atendido para actualizar la animación.
*/

void BorrarClienteEnAtencion(caja *info_caja, int caja) {
    MoverCursor(info_caja[caja].atencion.x, info_caja[caja].atencion.y);
    printf("    "); // Borra el cliente atendido
}







