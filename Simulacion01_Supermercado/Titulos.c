
//Librerias necesarias para el funcionamiento de estas animaciones
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "presentacion.h"
#include "supermercado.h"
#include "Titulos.h"
#define TIEMPO_BASE 3
#define ANCHO 4
#define FILAS_CLIENTES 4 // número de espacios de fila por caja
extern const char *LETRAS[][5];

/*Busca que se imprima un titulo centrado el cual sebera de centrarse en un rango de 79 columnas pero 
 la primera esta ocupada al igual que la ultima entonces va de 2 a 78
*/
/*
| Número | Color                    |
| ------ | ------------------------ |
| 0      | Negro                    |
| 1      | Azul oscuro              |
| 2      | Verde oscuro             |
| 3      | Aqua / Cian oscuro       |
| 4      | Rojo oscuro              |
| 5      | Púrpura oscuro           |
| 6      | Amarillo oscuro          |
| 7      | Gris claro (por defecto) |
| 8      | Gris oscuro              |
| 9      | Azul claro               |
| 10     | Verde claro              |
| 11     | Aqua claro / Cian        |
| 12     | Rojo claro               |
| 13     | Rosa / Magenta claro     |
| 14     | Amarillo claro           |
| 15     | Blanco brillante         |
Colores para el titulo 
*/

/*
    void DibujarTitulo(const char *titulo, int x, int y)
    Recibe: Un texto (titulo) y las coordenadas (x, y) donde se debe iniciar su impresión en pantalla.
    Devuelve: Nada.
    Observaciones: Dibuja el título con letras grandes utilizando una tabla de caracteres definidos, alternando colores para cada letra.
*/

void DibujarTitulo(const char *titulo, int x, int y ){
    int posX = x;  // Posición horizontal
    int cont = 0;

    for (int i = 0; titulo[i] != '\0'; i++) {
        char c = toupper(titulo[i]);

        if (c >= 'A' && c <= 'Z') {
            int color;
            if (cont % 2 == 0) {
                color = 14;  // Azul claro
            } else {
                color = 3; // Verde claro
            }
            cambiarColor(color);
            for (int fila = 0; fila < 5; fila++) {
                MoverCursor(posX, y + fila);
                printf("%s", LETRAS[c - 'A'][fila]);
            }
            posX += 6;  // Ancho de letra + espacio
            cont++;
        } else {
            // Espacio para carácter no alfabético
            for (int fila = 0; fila < 5; fila++) {
                MoverCursor(posX, y + fila);
                printf("     "); // 5 espacios
            }
            posX += 6;
        }
    }

    cambiarColor(15); // Restaurar color
}

/*
    void DibujarTituloCentrado(const char *titulo)
    Recibe: Un texto (titulo).
    Devuelve: Nada.
    Observaciones: Centra horizontalmente el título en la pantalla y lo imprime con letras grandes usando DibujarTitulo.
*/
void DibujarTituloCentrado(const char *titulo){
    int longitud = 0;
    for (int i = 0; titulo[i] != '\0'; i++) {
        if (isalpha(titulo[i]) || titulo[i] == ' ') {
            longitud++; // contamos letras y espacios como letras gráficas de ancho 6
        }
    }

    int anchoTitulo = longitud * 6; // cada letra ocupa 6 columnas (5 + espacio)
    int x = 1 + (78 - anchoTitulo) / 2; // centrado entre columna 1 y 78
    int y = 1; // una fila debajo del marco superior
    MostrarNumeroClientesAtendidos(0);

    DibujarTitulo(titulo, x, y);
}

/*
    void MostrarNumeroClientesAtendidos(int cantidad)
    Recibe: Un entero que representa la cantidad de clientes atendidos.
    Devuelve: "Nada ".
    Observaciones: Muestra o actualiza el número de clientes atendidos en una posición fija de la pantalla.
*/

void MostrarNumeroClientesAtendidos(int cantidad) {
    static int xNumero = 0; // posición x donde empieza el número (se guarda una sola vez)
    int y = 7; // fila donde se imprime el mensaje
    
    if (cantidad == 0) {
        char mensaje[50];
        sprintf(mensaje, "Numero de clientes atendidos: %d", cantidad);

        int ancho = strlen(mensaje);
        int x = (80 - ancho) / 2; 

        // Guardamos la posición exacta donde inicia el número
        // para reutilizarla cuando se actualice
        xNumero = x + strlen("Numero de clientes atendidos: ");

        MoverCursor(x, y);
        cambiarColor(15);
        printf("%s", mensaje);
        cambiarColor(2);
    } else if (xNumero != -1) {
        // Solo actualizar el número
        MoverCursor(xNumero, y);
        cambiarColor(15);
        printf("%d   ", cantidad);  // agregamos espacio para borrar restos (ej: de 99 a 100)
        cambiarColor(7);
    }
}

/*
    void DibujarContorno()
    Recibe: Nada.
    Devuelve: Nada.
    Observaciones: Dibuja un marco animado alrededor del área principal del programa con un efecto visual.
*/
void DibujarContorno(){
    int i;
    // Dibuja el contorno del programa
    cambiarColor(3);
    for (i = 0; i < 79; i++)
    {
        MoverCursor(i, 0);
        printf("*");

        MoverCursor(i, 23);
        printf("*");
        EsperarMiliSeg(TIEMPO_BASE);
    }
    for (i = 0; i < 24; i++)
    {
        MoverCursor(0, i);
        printf("*");

        MoverCursor(79, i);
        printf("*");
        EsperarMiliSeg(TIEMPO_BASE);
    }
    cambiarColor(15); // Restaurar color
}


/*
    void DibujarCajas(int cantidadCajeros, caja *info_cajas)
    Recibe: El número de cajas y un arreglo de estructuras que contienen la información de cada caja.
    Devuelve: Nada.
    Observaciones: Dibuja gráficamente las cajas de atención y sus respectivas filas. También guarda posiciones importantes para futuras animaciones.
*/

void DibujarCajas(int cantidadCajeros, caja *info_cajas)
{
    int centrar = (79 - (cantidadCajeros * 7 + 1)) / 2;
    int filaCaja = 7;
    int cont = 0;

    for (int i = 0; i < cantidadCajeros; i++)
    {
        cont++;
        int columna = centrar + 6 * i + cont;

        // Etiqueta de caja
        MoverCursor(columna + 2, filaCaja+2);
        printf("A");
        MoverCursor(columna + 3, filaCaja+2);
        printf("%d", i + 1);
         // Guardar coordenada dinámica de atencion de clientes
        info_cajas[i].atencion.x= columna + 1;         // centro de la fila
        info_cajas[i].atencion.y = filaCaja + 4;   

        cambiarColor(11);
        // Parte gráfica de la caja
        for (int j = 0; j < 4; j++)
        {
            MoverCursor(columna + j + 1, filaCaja + 1);
            printf("_");

            MoverCursor(columna + j + 1, filaCaja + 3);
            printf("-");

           // MoverCursor(columna + j + 1, filaCaja + 3);
            //printf("o");

            MoverCursor(columna + j + 1, filaCaja + 6);
            printf("_");
            EsperarMiliSeg(TIEMPO_BASE);

            if (j == 0)
            {
                for (int k = 0; k < 5; k++)
                {
                    MoverCursor(columna + j, filaCaja + k + 2);
                    printf("|");
                }
            }
            if (j == 3)
            {
                for (int k = 0; k < 5; k++)
                {
                    MoverCursor(columna + j + 2, filaCaja + k + 2);
                    printf("|");
                }
            }
        }

        //Para las filas 
        // Parte superior de la fila (*----*)
        cambiarColor(7);
        MoverCursor(columna, filaCaja + 9);
        printf("*====*");

        // Cuerpo de la fila con :    :
        for (int f = 0; f < FILAS_CLIENTES; f++)
        {
            int filaActual = filaCaja + 10 + f;
            MoverCursor(columna, filaActual);
            printf(":");

            MoverCursor(columna + 5, filaActual);
            printf(":");
            EsperarMiliSeg(TIEMPO_BASE);
        }
        
        // Guardar coordenada dinámica del inicio de la fila
        /*
        Indisquensable que sea en esta parte ya que en este momento estamos obteniendo las coordenadas de
        inicio de las filas visuales para imprimirlas, entonces se aprovecha para guardarlas para funciones en el main
        */
        info_cajas[i].inicio_fila.x = columna + 1;         // centro de la fila
        info_cajas[i].inicio_fila.y = filaCaja + 10;   
        info_cajas[i].pos_exceso.x= columna +1;
        info_cajas[i].exceso_anterior = 0;
        info_cajas[i].pos_exceso.y=20;
    }
    cambiarColor(14); 
    //Dibujamos la "entrada" de los clientes aqui apareceran
        MoverCursor(1, 22);
     printf("------------------------------------    -------------------------------------");
     cambiarColor(15); // Restaurar color
}

/*
    void MostrarAnuncioCierre()
    Recibe: Nada.
    Devuelve: Nada.
    Observaciones: Borra la pantalla y muestra un anuncio de cierre animado utilizando dos títulos centrados.
    Esto corresponde a que se han atendido 100 clientes y no falta nadie por ser atendido en filas.
*/
void MostrarAnuncioCierre(){
 
    // Limpiar pantalla
   BorrarPantalla(); // o "clear" en Linux/Mac
    DibujarContorno();

    DibujarTituloCentradoEnY("META", 4);        
    DibujarTituloCentradoEnY("ALCANZADA", 10); 
    EsperarMiliSeg(10000);
}
  
/*
    void DibujarTituloCentradoEnY(const char *titulo, int y)
    Recibe: Un texto (titulo) y una posición vertical y.
    Devuelve: Nada.
    Observaciones: Dibuja el título centrado horizontalmente en la posición y indicada, empleado especialmente 
    para la funcion de mostrar anuncio de cierre.
*/
void DibujarTituloCentradoEnY(const char *titulo, int y){
    int longitud = 0;
    for (int i = 0; titulo[i] != '\0'; i++) {
        if (isalpha(titulo[i]) || titulo[i] == ' ') {
            longitud++;
        }
    }

    int anchoTitulo = longitud * 6; 
    int x = 1 + (78 - anchoTitulo) / 2;

    DibujarTitulo(titulo, x, y);
}


//Arreglo de todas las letras
const char *LETRAS[][5] = {
    // A
    {
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
        "**** "
    },
    // C
    {
        " ****",
        "*    ",
        "*    ",
        "*    ",
        " ****"
    },
    // D
    {
        "**** ",
        "*   *",
        "*   *",
        "*   *",
        "**** "
    },
    // E
    {
        "*****",
        "*    ",
        "*****",
        "*    ",
        "*****"
    },
    // F
    {
        "*****",
        "*    ",
        "**** ",
        "*    ",
        "*    "
    },
    // G
    {
        " ****",
        "*    ",
        "*  **",
        "*   *",
        " ****"
    },
    // H
    {
        "*   *",
        "*   *",
        "*****",
        "*   *",
        "*   *"
    },
    // I
    {
        "*****",
        "  *  ",
        "  *  ",
        "  *  ",
        "*****"
    },
    // J
    {
        "  ***",
        "   * ",
        "   * ",
        "*  * ",
        " **  "
    },
    // K
    {
        "*   *",
        "*  * ",
        "***  ",
        "*  * ",
        "*   *"
    },
    // L
    {
        "*    ",
        "*    ",
        "*    ",
        "*    ",
        "*****"
    },
    // M
    {
        "*   *",
        "** **",
        "* * *",
        "*   *",
        "*   *"
    },
    // N
    {
        "*   *",
        "**  *",
        "* * *",
        "*  **",
        "*   *"
    },
    
    // O
    {
        " *** ",
        "*   *",
        "*   *",
        "*   *",
        " *** "
    },
    // P
    {
        "**** ",
        "*   *",
        "**** ",
        "*    ",
        "*    "
    },
    // Q
    {
        " *** ",
        "*  * ",
        "*  * ",
        "*  * ",
        " ****"
    },
    // R
    {
        "**** ",
        "*   *",
        "**** ",
        "*  * ",
        "*   *"
    },
    // S
    {
        " ****",
        "*    ",
        " *** ",
        "    *",
        "**** "
    },
    // T
    {
        "*****",
        "  *  ",
        "  *  ",
        "  *  ",
        "  *  "
    },
    // U
    {
        "*   *",
        "*   *",
        "*   *",
        "*   *",
        " *** "
    },
    // V
    {
        "*   *",
        "*   *",
        "*   *",
        " * * ",
        "  *  "
    },
    // W
    {
        "*   *",
        "*   *",
        "* * *",
        "** **",
        "*   *"
    },
    // X
    {
        "*   *",
        " * * ",
        "  *  ",
        " * * ",
        "*   *"
    },
    // Y
    {
        "*   *",
        " * * ",
        "  *  ",
        "  *  ",
        "  *  "
    },
    // Z
    {
        "*****",
        "   * ",
        "  *  ",
        " *   ",
        "*****"
    }
};
