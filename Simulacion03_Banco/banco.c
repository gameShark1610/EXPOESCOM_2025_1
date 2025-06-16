/*
banco.c
V 1.0 Mayo 2025
Autor: Escompiladores

Programa de simulacion de banco
EL programa debe de simular la atencion de personas en un banco, cuidando que
sean respetadas las politicas de atencion del mismo y evitando que las personas
dejen de ser atentidas

Compilación
    Windows:
    gcc presentacionWin.c TADColaDin.c banco.c  -o ejecutar.exe
    ejecutar.exe
Parametros de entrada: Ninguno
Esto ejecuta el programa y pedira los requisitos para comenzar
*/

// Librerias//
#include <stdio.h>
#include <stdlib.h>
#include "TADColaDin.h"
#include <windows.h> //Funciona unicamente en Windows para usar la función Sleep()
#include "presentacion.h"

// CONSTANTES
#define ALTO 5 // Se piensa en un pantalla de 24 filas x 79 columnas
#define ANCHO 4
#define TIEMPO_BASE 3 // Tiempo base en milisegundos

// Variables globales
int centrar;

// Metodos
void DibujarTitulo();
void DibujarClienteEnFilas(int filaBanco, char *cliente, cola *colaDeBanco);
void DibujarClienteEnCaja(int caja, char *cliente);
void DibujarFilas();
void DibujarCajeros(int cantidadCajeros);
void inicializarColaDePrioridad(cola *colaDePrioridad);
void controlarBanco(cola *colaDePrioridad, int *tiempos, int *cajeros, int cantidadCajeros);

/*
int main (void)
Recibe: Nada solo ejecuta el programa
Devuelve: int (1) si se ejecuto correctamente
Observaciones: Se utiliza un 'menu' para poder recibir las variables del numero de cajero y los tiempos a manejar.
*/
int main()
{
    // Declaracion de variables
    cola colaDePrioridad[3];
    int tiempos[4];

    // Datos de entrada
    int numeroCajeros;
    printf("Numero de cajeros en el banco:\n ");
    scanf("%d", &numeroCajeros); // Lee el numero de cajeros
    printf("Tiempo de atencion en milisegundos de cada cajero:\n ");
    scanf("%d", &tiempos[0]); // Lee el tiempo de atencion por caja
    printf("Tiempo de llegada de los clientes del banco:\n ");
    scanf("%d", &tiempos[2]); // Lee el tiempo de llegada de los clientes
    printf("Tiempo de llegada de los usuarios del banco:\n ");
    scanf("%d", &tiempos[3]); // Lee el tiempo de llegada de los usuarios
    printf("Tiempo de llegada de los preferentes del banco:\n ");
    scanf("%d", &tiempos[1]); // Lee el tiempo de llegada de los preferentes

    // Revisa que el numero de cajas sea 0<numero de cajas<11, de lo contrario cierra el programa
    if (numeroCajeros <= 0 || numeroCajeros >= 11)
    {
        printf("El numero de cajas debe ser mayor a 0 y menor a 11\n");
        exit(1);
    }

    // Revisa que los tiempos manejados sean multiplos de 10, de lo contrario cierra el programa
    for (int i = 0; i < 4; i++)
    {
        if (tiempos[i] % 10 != 0)
        {
            printf("Los tiempos registrados deben ser multiplos de 10\n");
            exit(1);
        }
    }

    //(Arreglo binario) Crea la cantidad de cajeros y los incializa con valor de 0
    //"0" representa que el cajero esta disponible para atender, "1" representa que esta ocupado
    int cajeros[numeroCajeros];
    for (int i = 0; i < numeroCajeros; i++)
        cajeros[i] = 0;

    DibujarCajeros(numeroCajeros);
    DibujarFilas();
    DibujarTitulo();
    inicializarColaDePrioridad(colaDePrioridad);
    controlarBanco(colaDePrioridad, tiempos, cajeros, numeroCajeros);

    return 0;
}

/*
void DibujarTitulo()
Descripción: Dibuja el titulo
Recibe: int numero del cajero, char* cliente que esta pasando al cajero
Devuelve:
Observaciones:
*/
void DibujarTitulo()
{
    /*    {
        "  *  ",
        " * * ",
        "*****",
        "*   *",
        "*   *"
    },
    // B
    {
        "**** ",
        "*   *",
        "**** ",
        "*   *",
        "**** "*/

    int xTitulo, yTitulo;
    xTitulo = 12;
    yTitulo = 1;
    MoverCursor(xTitulo, yTitulo);
    printf("\033[38;5;32m******   \033[38;5;228m  **    \033[38;5;32m*     *  \033[38;5;228m******  \033[38;5;32m******  \033[38;5;228m*****  *****\n");
    MoverCursor(xTitulo, yTitulo + 1);
    printf("\033[38;5;32m*     *  \033[38;5;228m *  *   \033[38;5;32m*     *  \033[38;5;228m*       \033[38;5;32m*    *  \033[38;5;228m*   *  *   *\n");
    MoverCursor(xTitulo, yTitulo + 2);
    printf("\033[38;5;32m******   \033[38;5;228m******  \033[38;5;32m  *   *  \033[38;5;228m*       \033[38;5;32m*    *  \033[38;5;228m*   ****   *\n");
    MoverCursor(xTitulo, yTitulo + 3);
    printf("\033[38;5;32m*     *  \033[38;5;228m*    *  \033[38;5;32m*   * *  \033[38;5;228m*       \033[38;5;32m*    *  \033[38;5;228m*          *\n");
    MoverCursor(xTitulo, yTitulo + 4);
    printf("\033[38;5;32m******   \033[38;5;228m*    *  \033[38;5;32m*     *  \033[38;5;228m******  \033[38;5;32m******  \033[38;5;228m*          *\033[37m\n");
}

/*
void DibujarClienteEnFilas(int caja, char* cliente)
Descripción: Dibuja el cliente actual en la fila del banco
Recibe: int numero del cajero, char* cliente que esta pasando al cajero
Devuelve:
Observaciones:
*/
void DibujarClienteEnFilas(int filaBanco, char *cliente, cola *colaDeBanco)
{
    elemento aux;
    int filas,i;
    filas=17;
    int cant;
    
    switch (filaBanco)
    {
    case 0:
        //Limpia los resiudos
        for ( i = 0; i < 5 ; i++)
        {
            MoverCursor(36, filas+i);
            printf("    ");
        }
        //Muestras las personas en la fila
        for ( i = 0; i < 5 && i<Size(colaDeBanco); i++)
        {
            //EsperarMiliSeg(50);
            aux=Element(colaDeBanco,i+1);
            MoverCursor(36, filas+i);
            printf(aux.palabras);
        }
        MoverCursor(36, 22);
        printf("+");
        cant=Size(colaDeBanco);
        MoverCursor(37, 22);
        if (cant>5)
        {
            printf("%d", cant-5);
        }else printf("%d", 0);
        
        break;
    case 1:
        //Limpia los resiudos
        for ( i = 0; i < 5 ; i++)
        {
            MoverCursor(46, filas+i);
            printf("    ");
        }
        //Muestras las personas en la fila
        for ( i = 0; i < 5 && i<Size(colaDeBanco); i++)
        {
            //EsperarMiliSeg(50);
            aux=Element(colaDeBanco,i+1);
            MoverCursor(46, filas+i);
            printf(aux.palabras);
        }
        MoverCursor(46, 22);
        printf("+");
        cant=Size(colaDeBanco);
        MoverCursor(47, 22);
        if (cant>5)
        {
            printf("%d", cant-5);
        }else printf("%d", 0);
        
        break;
    case 2:
        //Limpia los resiudos
        for ( i = 0; i < 5 ; i++)
        {
            MoverCursor(56, filas+i);
            printf("    ");
        }
        //Muestras las personas en la fila
        for ( i = 0; i < 5 && i<Size(colaDeBanco); i++)
        {
            //EsperarMiliSeg(50);
            aux=Element(colaDeBanco,i+1);
            MoverCursor(56, filas+i);
            printf(aux.palabras);
        }
        MoverCursor(56, 22);
        printf("+");
        cant=Size(colaDeBanco);
        MoverCursor(57, 22);
        if (cant>5)
        {
            printf("%d", cant-5);
        }else printf("%d", 0);
        break;

    default:
        break;
    }
    /*
    MoverCursor(47, 16);
    printf("C");

    MoverCursor(57, 16);
    printf("U");

    int columna, fila;
    fila = 5;
    columna = centrar + 6 * (caja) + caja;
    MoverCursor(columna + 2, fila);
    printf(cliente);
    */
}

/*
void DibujarClienteEnCaja(int caja, char* cliente)
Descripción: Dibuja el cliente actual en la caja;
Recibe: int numero del cajero, char* cliente que esta pasando al cajero
Devuelve:
Observaciones:
*/
void DibujarClienteEnCaja(int caja, char *cliente)
{
    Sleep(200);
    int columna, fila;
    fila = 11;
    columna = centrar + 6 * (caja) + caja;
    MoverCursor(columna + 2, fila);
    printf(cliente);
}

/*
void DibujarCajas(int cantidadCajeros)
Descripción: Dibuja la cantidad de cajeros
Recibe: int cantidad de cajeros
Devuelve:
Observaciones:
*/
void DibujarFilas()
{
    // Indice

    MoverCursor(5, 16);
    printf("\033[38;5;251m[P] Clientes Preferentes");
    EsperarMiliSeg(TIEMPO_BASE);
    MoverCursor(5, 18);
    printf("[C] Clientes del banco");
    EsperarMiliSeg(TIEMPO_BASE);
    MoverCursor(5, 20);
    printf("[U] Usuarios del banco");
    EsperarMiliSeg(TIEMPO_BASE);
    // MoverCursor( 79, i);
    // printf("*");

    // Dibuja las filas
    for (int i = 0; i < 7; i++)
    {
        MoverCursor(35, 16 + i);
        printf("\033[38;5;250m:");
        MoverCursor(40, 16 + i);
        printf(":");

        MoverCursor(45, 16 + i);
        printf(":");
        MoverCursor(50, 16 + i);
        printf(":");

        MoverCursor(55, 16 + i);
        printf(":");
        MoverCursor(60, 16 + i);
        printf(":\033[38m");

        // MoverCursor( 79, i);
        // printf("*");
        EsperarMiliSeg(TIEMPO_BASE);
    }

    MoverCursor(37, 15);
    printf("P");

    MoverCursor(47, 15);
    printf("C");

    MoverCursor(57, 15);
    printf("U");

    for (int i = 0; i < 4; i++)
    {
        MoverCursor(35, 16);
        printf("*");
        MoverCursor(36 + i, 16);
        printf("=");
        MoverCursor(40, 16);
        printf("*");

        MoverCursor(45, 16);
        printf("*");
        MoverCursor(46 + i, 16);
        printf("=");
        MoverCursor(50, 16);
        printf("*");

        MoverCursor(55, 16);
        printf("*");
        MoverCursor(56 + i, 16);
        printf("=");
        MoverCursor(60, 16);
        printf("*");

        // MoverCursor( 79, i);
        // printf("*");
        EsperarMiliSeg(TIEMPO_BASE);
    }

    // MoverCursor(cantidadCajeros + (num * 8) + 1, 2);
    // printf("A:%d", cliente);
    MoverCursor(0, 25);
}

/*
void DibujarCajas(int cantidadCajeros)
Descripción: Dibuja la cantidad de cajeros
Recibe: int cantidad de cajeros
Devuelve:
Observaciones:
*/
void DibujarCajeros(int cantidadCajeros)
{
    centrar = (79 - (cantidadCajeros * 7 + 1)) / 2;
    BorrarPantalla();
    int columna = centrar, fila = 7, i = 0;
    int cont = 0;
    // Recorrer cada columna
    for (int i = 0; i < cantidadCajeros; i++)
    {
        cont++;
        columna = centrar + 6 * i + cont;
        MoverCursor(columna + 2, fila);
        printf("\033[37mC");
        MoverCursor(columna + 3, fila);
        printf("%d", i + 1);

        for (int j = 0; j < ANCHO; j++)
        {
            // Mover el cursor, dibujar un * y esperar TIEMPO_BASE milisegundos
            MoverCursor(columna + j + 1, fila + 1);
            printf("\033[38;5;111m_");

            MoverCursor(columna + j + 1, fila + 2);
            printf("_");

            MoverCursor(columna + j + 1, fila + 3);
            printf("o");

            MoverCursor(columna + j + 1, fila + 6);
            printf("_");
            EsperarMiliSeg(TIEMPO_BASE);

            if (j == 0)
            {
                for (int k = 0; k < ALTO; k++)
                {
                    MoverCursor(columna + j, fila + k + 2);
                    printf("|");
                    // EsperarMiliSeg(TIEMPO_BASE);
                }
            }
            if (j == 3)
            {
                for (int k = 0; k < ALTO; k++)
                {
                    MoverCursor(columna + j + 2, fila + k + 2);
                    printf("|");
                    // EsperarMiliSeg(TIEMPO_BASE);
                }
            }
        }
        MoverCursor(columna + 1, fila + 4);
        printf("0");
        MoverCursor(columna + 2, fila + 4);
        printf("0");
        MoverCursor(columna + 3, fila + 4);
        printf("0");
        MoverCursor(columna + 4, fila + 4);
        printf("0");
    }

    // Dibuja el contorno del programa
    for (i = 0; i < 79; i++)
    {
        MoverCursor(i, 0);
        printf("\033[38;5;32m*");

        MoverCursor(i, 23);
        printf("\033[38;5;32m*");
        EsperarMiliSeg(TIEMPO_BASE);
    }
    for (i = 0; i < 24; i++)
    {
        MoverCursor(0, i);
        printf("*");

        MoverCursor(79, i);
        printf("*\033[38;5;32m");
        EsperarMiliSeg(TIEMPO_BASE);
    }

    // MoverCursor(cantidadCajeros + (num * 8) + 1, 2);
    // printf("A:%d", cliente);
    MoverCursor(0, 25);
}

/*
void inicializarColaDePrioridad(cola *colaDePrioridad)
Descripción: Recibe un  arreglo de colas y e inicializa cada una
Recibe: cola *c (Referencia al arreglo de colas "colaDePrioridad")
Devuelve:
Observaciones: Solo funciona para arreglos de 3 colas
*/
void inicializarColaDePrioridad(cola *colaDePrioridad)
{
    Initialize(&colaDePrioridad[0]);
    Initialize(&colaDePrioridad[1]);
    Initialize(&colaDePrioridad[2]);
}

/*
void controlarBanco(cola *colaDePrioridad, int *tiempos, int *cajeros, int cantidadCajeros)
Descripción: Realiza la funcion principal del baco, administras cajas y filas de clientes
Recibe: cola *cola de prioridad (Referencia al arreglo de colas "colaDePrioridad"), int *tiempos (Tiempo de los cientes en llegar), int* cajeros (Estado actual del cajero, vacio o ocupado), int cantidadCajeros (la cantidad de cajeros a manejar)
Devuelve:
Observaciones: Este funcion siempre se mantiene ejecutando, las colas de la cola de prioridad deben estar previamente inicializadas
*/
void controlarBanco(cola *colaDePrioridad, int *tiempos, int *cajeros, int cantidadCajeros)
{
    // Variables
    int contadoresClientes[3] = {0};
    int contador = 0, filasUsuarios = 0, filasClientes = 0;
    int *tiemposActuales;
    int cajaAbierta = 0;
    elemento nombre;
    char numeroToString[45];

    tiemposActuales = malloc(sizeof(int) * cantidadCajeros);

    while (1)
    {
        Sleep(1); // Espera 1ms
        contador += 10;
        // printf("%d\n",contador);

        // Para los clientes
        for (int i = 0; i < 3; i++)
        {
            if (contador % tiempos[i + 1] == 0)
            {
                contadoresClientes[i]++;
                sprintf(numeroToString, "%d", contadoresClientes[i]); // Convierte el numero a string

                // Asinga el tipo de usuario y agrega a la variable elemento la palabra
                switch (i + 1)
                {
                case 1:
                    strcpy(nombre.palabras, "P");
                    break;
                case 2:
                    strcpy(nombre.palabras, "C");
                    break;
                case 3:
                    strcpy(nombre.palabras, "U");
                    break;
                default:
                    break;
                }
                strcat(nombre.palabras, numeroToString); // Agrega a la variable elemento el numero de cliente
                // printf("%s",nombre.palabras);
                Queue(&colaDePrioridad[i], nombre); // Agrega el cliente a la cola
                strcpy(nombre.palabras, "");

                if (!Empty(&colaDePrioridad[i]))
                {
                    DibujarClienteEnFilas(i, Front(&colaDePrioridad[i]).palabras, &colaDePrioridad[i]);
                }
            }
        }

        // Para las cajas
        for (int i = 0; i < cantidadCajeros; i++)
        {
            // Verifica que se termine de desocupar la caja
            if ((contador - cajeros[i]) == tiempos[0])
            {
                cajeros[i] = 0;
                DibujarClienteEnCaja(i, "    ");
                Sleep(1);
            }
            // MoverCursor(0, 24);
            // printf("Contador:%d\n", contador);
            // MoverCursor(0, 25 + i);
            // printf("Cajero: %d, Tiempo:%d\n", i, cajeros[i]);
            //  Verifica que le caja este vacia
            if (cajeros[i] == 0)
            {

                for (int j = 0; j < 3; j++)
                {
                    if (!Empty(&colaDePrioridad[j]))
                    {
                        switch (j)
                        {
                        case 0:
                            if (filasUsuarios < 5 && filasClientes < 2)
                            {
                                DibujarClienteEnCaja(i, Dequeue(&colaDePrioridad[j]).palabras);
                                if (!Empty(&colaDePrioridad[j]))
                                {
                                    DibujarClienteEnFilas(j, Front(&colaDePrioridad[j]).palabras, &colaDePrioridad[j]);
                                }
                                else
                                {
                                    MoverCursor(36, 17);
                                    printf("    ");
                                }
                                // printf("%s\n", Dequeue(&colaDePrioridad[j]).palabras);
                                // printf("Caja %d:",i);
                                cajeros[i] = contador;
                                filasUsuarios++;
                                filasClientes++;
                            }
                            else
                                continue;
                            break;
                        case 1:
                            if (filasUsuarios < 5)
                            {
                                DibujarClienteEnCaja(i, Dequeue(&colaDePrioridad[j]).palabras);
                                if (!Empty(&colaDePrioridad[j]))
                                {
                                    DibujarClienteEnFilas(j, Front(&colaDePrioridad[j]).palabras, &colaDePrioridad[j]);
                                }
                                else
                                {
                                    MoverCursor(46, 17);
                                    printf("    ");
                                }
                                // printf("%s\n", Dequeue(&colaDePrioridad[j]).palabras);
                                cajeros[i] = contador;
                                filasUsuarios++;
                                filasClientes = 0;
                            }
                            else
                                continue;
                            ;
                            break;
                        case 2:
                            DibujarClienteEnCaja(i, Dequeue(&colaDePrioridad[j]).palabras);
                            if (!Empty(&colaDePrioridad[j]))
                            {
                                DibujarClienteEnFilas(j, Front(&colaDePrioridad[j]).palabras, &colaDePrioridad[j]);
                            }
                            else
                            {
                                MoverCursor(56, 17);
                                printf("    ");
                            }
                            // printf("%s\n", Dequeue(&colaDePrioridad[j]).palabras);
                            cajeros[i] = contador;
                            filasClientes++;
                            filasUsuarios = 0;
                            break;
                        default:
                            break;
                        }

                        break;
                    } // else printf("No hay personas en el banco\n");
                }
            }
        }
    }
}
