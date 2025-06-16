/*
SIMULACION ANIMADA DE UN GESTOR DE PROCESOS IMPLEMENTANDO UN TAD COLA SIN CONSIDERAR PRIORIDADES
Autor original de la implementacion y el codigo objeto: Edgardo Adrian Franco Martinez
Autor(es) de la implementacion: Escompiladores
Versión: 1.100.2 (Mayo 2025) 
DESCRIPCION: Muchos de los procesadores modernos utilizan algoritmos "Round Robin" para la planificacion de procesos. Dichos algoritmos
asignan a cada proceso un tiempo fijo de CPU llamado quantum o cuanto de tiempo. Si el procesador no termina en ese tiempo, entonces 
se interrumpe su atención y se pone al final de la cola de "listos". Luego el proceso siguiente recibe su turno
El programa simula dicho algoritmo, partiendo de una lista de procesos previamente dada por el usuario, se empiezan a atender uno a uno
en un quantum de tiempo de 1 segundo. Si el proceso es atendido se manda a la cola de "Procesos atendidos", de lo contrario se reencola
hasta poder finalizar con su tiempo estimado.
COMPILACION
-gcc presentacionWin.c TADColaDin.c main.c -o main.exe
-gcc main.C presentacionWin.c TADColaDin.c -o main
*/

//Librerias utilizadas 
#include <stdio.h> 
#include <stdlib.h>
#include "TADColaDin.h"
#include <windows.h>
#include"presentacion.h"
#include <time.h>

//Prototipos de funciones
void pedirDatos(elemento e);
elemento generarProceso();
void encolarProceso();
void mostrarProceso();
void atenderProceso();
void mostrarProcesosAtendidos();
void dibujarCaja();
void dibujarTitulo(int xTitulo, int yTitulo);
void dibujarMargen();
void animacionProceso1();
void borrarAnimacionProceso1();
void animacionProceso2();
void borrarAnimacionProceso2();
void medirTiempo();
void dibujarCuadro1();
void dibujarCuadro2();

#define TIEMPO_BASE 3
#define ALTO 12      //Alto de la caja principal
#define ANCHO 80    //Ancho de la caja principal
//Se piensa una consola de 120x30   
#define ANCHOSHELL 132  
#define ALTOSHELL 36
#define ALTOCUADRO 5
#define ANCHOCUADRO 45

//-----------VARIABLES GLOBALES------------------------
cola c; //Cola que contiene los procesos a ser atendidos
cola atendidos; //Cola que contendrá los procesos atendidos
int counter,hayProceso;
clock_t fin; //Variable que permite 

int main()
{
    int xTitulo, yTitulo;   //Dimensiones del titulo (opcionales a mostrar en consola)
    
    BorrarPantalla();   
    dibujarMargen();    
    //dibujarTitulo(70,2);
    dibujarCaja();
    int p = 1; //variable auxiliar inicializada
    Initialize(&c);
    Initialize(&atendidos);
    MoverCursor(2,1);
    printf("Simulacion de procesos del sistema operativo\n");
    MoverCursor(2,2);
    printf("Ingrese la cantidad de procesos: ");
    scanf("%d", &counter);
    dibujarCuadro1();
    dibujarCuadro2();
    while(p <= counter) //Mientras p sea menor al numero de procesos ingresados
    {
        encolarProceso();
        mostrarProceso();
        p++;
    }
    atenderProceso();
    MoverCursor(0,35);
    return 0;
}

/*
elemento generarProceso()
Recibe: void (no recibe valor explícito)
Devuelve: elemento proceso (proceso generado listo para ser usado en la simulacion)
Observaciones: El usuario ha ingresado los n procesos determinados por counter (declarada como variable global)
-Se hace uso de la funcion fgets() para los campos "nombre" y "actividad" del struct elemento, esto para permitir al usuario
ingresar cadenas con espacios
*/
elemento generarProceso()
{
    elemento proceso;
    int c; //Variable que nos permite controlar la lectura de la cadena mediante la entrada estandar (stdin)
    while ((c = getchar()) != '\n' && c != EOF); //Limpia el buffer, lee mientras el caracter ingresado sea diferente a un salto de linea y "/0"

    //-------ENTRADA DE LOS DATOS------------
    MoverCursor(50,1);
    printf("Nombre del proceso: ");
    MoverCursor(70,1);
    fgets(proceso.nombre, sizeof(proceso.nombre), stdin); //Lee la cadena (entrada stdin) ingresada por el usuario de longitud proceso.nombre - 1 
    proceso.nombre[strcspn(proceso.nombre, "\n")] = '\0'; //Elimina el salto de linea que fgets() dejó, para evitar problemas

    MoverCursor(50,2);
    printf("Actividad que realiza: ");
    MoverCursor(74,2);
    fgets(proceso.actividad, sizeof(proceso.actividad), stdin);
    proceso.actividad[strcspn(proceso.actividad, "\n")] = '\0';

    //Para los campos "ID" y "Tiempo" se utiliza la funcion scanf() ya que no se permiten entradas con espacios vacios
    MoverCursor(50,3);
    printf("ID del proceso: ");
    MoverCursor(66,3);
    scanf("%s",&proceso.id);

    MoverCursor(50,4);
    printf("Tiempo para su atencion: ");
    MoverCursor(75,4);
    scanf("%d", &proceso.tiempo);

    return proceso;
}

/*
void encolarProceso()
Recibe: void (no recibe valor explícito)
Devuelve: void (no retorna valor explícito)
Observaciones: El usuario ha creado los n procesos determinados por counter (declarada como variable global) y los encola en &c
Al crear cada proceso el usuario da inicio a la variable clock() que determinará el tiempo de vida del proceso hasta que este haya sido
atendido
*/
void encolarProceso()
{
    elemento nuevoProceso;
    nuevoProceso = generarProceso();
    nuevoProceso.tiempoInicio = clock(); //Se comienza a medir el tiempo de vida del proceso
    Queue(&c, nuevoProceso);
    return;
}

/*
void mostrarProceso()
Recibe: void (no recibe valor explícito)
Devuelve: void (no retorna valor explícito)
Observaciones: Se muestran los procesos a atender en los apartados "Proximo proceso a atender" y "Ultimo proceso atendido"
*/
void mostrarProceso()
{
    int y = 22; 
    int i, j; //variables para loops

    //Limpia la region donde previamente se solicitaron los datos de entrada de cada proceso
    for(i = 1; i <5 ; i++)
    {
        MoverCursor(50,i);
        printf("                                                       ");
    }
    //Limpia los recuadros cada vez que se encola un nuevo proceso, para evitar mala estetica
    for(i=20;i<=23;i++)
    {
        MoverCursor(6,i);
        printf("                                            ");
    } 
    for(i=28;i<=31;i++)
    {
        MoverCursor(6,i);
        printf("                                            ");
    }
    if(!Empty(&c)) //Mientras la cola no este vacia (de lo contrario se habrán atendido a todos los procesos)
    {
        elemento proximoProceso=Element(&c,1); //Primer elemento de la cola
        elemento ultimoProceso=Element(&c,Size(&c)); //Ultimo elemento de la cola
        MoverCursor(5,18);
        printf("Proximo proceso a atender:");
        MoverCursor(6,20);
        printf("%s",proximoProceso.nombre);
        MoverCursor(6,21);
        printf("%s",proximoProceso.actividad);
        MoverCursor(6,22);
        printf("%s",proximoProceso.id);
        MoverCursor(6,23);
        printf("%d",proximoProceso.tiempo);
        /*hayProceso indica que al menos ya se ha atendido un proceso en un quantum de 1 seg, lo que indica que irá a formar parte de 
        del apartado "Ultimo proceso atendido"*/
        if(hayProceso>0)
        {
            MoverCursor(5,26);
            printf("Ultimo proceso atendido:");
            MoverCursor(6,28);
            printf("%s",ultimoProceso.nombre);
            MoverCursor(6,29);
            printf("%s",ultimoProceso.actividad);
            MoverCursor(6,30);
            printf("%s",ultimoProceso.id);
            MoverCursor(6,31);
            printf("%d",ultimoProceso.tiempo);
        }
    }
    //Si la cola quedó vacía, entonces se limpia completamente los recuadros
    else
    {
        for(i=20;i<=23;i++)
        {
            MoverCursor(6,i);
            printf("                                            ");
        }
        for(i=28;i<=31;i++)
        {
            MoverCursor(6,i);
            printf("                                            ");
        }
    }
    return;
}

/*
void atenderProceso()
Recibe: void (no recibe valor explícito)
Devuelve: void (no retorna valor explícito)
Observaciones: Logica principal del programa. Se comienzan a atender a los procesos encolados con un quatum de tiempo de 1 segundo. Si 
el tiempo total de dicho proceso es mayor que 0, entonces se reencola, de los contrario se marca como atendido y se manda a la cola
de procesos atendidos. La funcion termina cuando todos los procesos fueron atendidos, es decir, cuando ya no hay elementos disponibles en
la cola &c
*/
void atenderProceso()
{
    elemento procesoAtendido;
    int y=12;
    int yImpresion=23;
    int i;
    int barraTotal=40;
    int progreso;

    while(!Empty(&c)) //Mientras la cola no esté vacía
    {
        procesoAtendido = Dequeue(&c); //Se desencola el proceso que en ese momento esté al frente
        animacionProceso1(); //Simula una animacion de entrada a la pantalla principal
        //mostrarProceso();

        //Limpiar pantalla principal, para simular que el siguiente proceso ha sido atendido
        for (i = 10; i <= 14; i++) 
        {
            MoverCursor(28, i);
            printf("                                                                             ");
        }

        fin = clock(); //Se para el tiempo de vida del proceso, para mostrar en tiempo real el tiempo del mismo desde que fue encolado por primera vez
        procesoAtendido.tiempoTotal = (double)(fin - procesoAtendido.tiempoInicio) / CLOCKS_PER_SEC; //se imprime el tiempo
        progreso = (procesoAtendido.tiempoTotal / procesoAtendido.tiempo) * barraTotal;
        MoverCursor(28 ,10);
        /*printf("╔══════════════════════════════════════════════════════════════════════╗\n");
        printf("║                             MONITOR DE CPU                           ║\n");
        printf("╠══════════════════════════════════════════════════════════════════════╣\n");
        printf("║                             [ EN EJECUCIÓN ]                         ║\n");
        printf("║----------------------------------------------------------------------║\n");
        printf("║ Proceso: %-15s ID: %-5s                                  ║\n", procesoAtendido.nombre, procesoAtendido.id);
        printf("║ Actividad: %-50s║\n", procesoAtendido.actividad);
        printf("║ Tiempo transcurrido: %5.2f s                                         ║\n", procesoAtendido.tiempoTotal);
        printf("║                                                                      ║\n");

        printf("║ ");
        for (int i = 0; i < barraTotal; i++) {
            if (i < progreso) printf("▓");
            else printf("░");
        }
        printf(" %3.0f%%                  ║\n", (procesoAtendido.tiempoTotal / procesoAtendido.tiempo) * 100);
        printf("║----------------------------------------------------------------------║\n");
        printf("╚══════════════════════════════════════════════════════════════════════╝\n");*/
        printf("ATENDIENDO EL PROCESO..   | %d seg restantes(s)",procesoAtendido.tiempo);
        MoverCursor(28,11);
        printf("-------------------------------------------------------------------");
        MoverCursor(28,12);
        
        printf("%-10s | %-10s | %-5s | %.2f s", procesoAtendido.nombre, procesoAtendido.actividad, procesoAtendido.id, procesoAtendido.tiempoTotal);
        Sleep(1000); //Quantum de 1 segundo, se atenderá el proceso durante este tiempo
        borrarAnimacionProceso1();
        procesoAtendido.tiempo--; //Se disminuye el quantum de 1 segundo al tiempo total de ejecucion del proceso

        //Si el tiempo total de ejecucion es mayor a cero, entonces el proceso no ha sido atendido completamente
        if(procesoAtendido.tiempo>0)
        {
            Queue(&c, procesoAtendido); //Se reencola el proceso
            hayProceso++; //Al menos un proceso ya pasó por la pantalla principal
        }
        else
        {
            fin = clock(); //Se pone fin al tiempo de vida del proceso
            procesoAtendido.tiempoTotal = (double)(fin - procesoAtendido.tiempoInicio) / CLOCKS_PER_SEC;
            Queue(&atendidos, procesoAtendido); //Se encola a la cola de procesos atendidos
            animacionProceso2(); //Simula la salida de la pantalla principal
            borrarAnimacionProceso2();
        }
        //Actualizacion grafica de las tablas y los procesos
        mostrarProceso();
        mostrarProcesosAtendidos();
    }

    //Se vacian graficamente las tablas de "Ultimo proceso atendido" y "proximo proceso a atender"
    if(Empty(&c))
    {
        for(i=10;i<=13;i++)
        {
            MoverCursor(28,i);
            printf("                                                                   ");    
        }
    }
}

/*
void mostrarProcesosAtendidos()
Recibe: void (no recibe valor explícito)
Devuelve: void (no retorna valor explícito)
Observaciones: Se muestran los procesos a atendidos con todos sus datos
*/
void mostrarProcesosAtendidos()
{
    int j;
    elemento p;
    int y = 20;
    MoverCursor(60,18);
    printf("Procesos atendidos:");
    MoverCursor(60,19);
    printf("Nombre             | Actividad                  | ID   | Tiempo total");
    MoverCursor(60,20);
    printf("-------------------+----------------------------+------+-------------");
    //Impresion de la cola atendidos
    y = 21;
    for(j = 1; j <= Size(&atendidos); j++)
    {
        p = Element(&atendidos, j);
        MoverCursor(60,y++);
        printf("%-18s | %-26s | %-4s | %.2f s", p.nombre, p.actividad, p.id, p.tiempoTotal);
    }
}

/*
void dibujarCaja()
Recibe: void (no recibe valor explícito)
Devuelve: void (no retorna valor explícito)
Observaciones: Se dibuja la pantalla principal al centro de la pantalla. Las coordenadas no son relativas
*/
void dibujarCaja()
{
    int x = (ANCHOSHELL-ANCHO)/2;    
    int y = 5;   
    int i;

    //Parte superior
    
    for (i = 0; i < ANCHO; i++) {
        MoverCursor(x + i, y);
        EsperarMiliSeg(TIEMPO_BASE);
        printf("\033[38;5;228m═");  //Cambio de color a verde
        printf("\033[0m");      //Se regresa el color al blanco predeterminado para evitar alterar por completo la apariencia del cmd
        MoverCursor(x + i, y+2);
        printf("\033[38;5;228m═");  //Cambio de color a verde
        printf("\033[0m");   
        MoverCursor(x + i, y+4);
        printf("\033[38;5;228m-");  //Cambio de color a verde
        printf("\033[0m"); 
    }
    MoverCursor(x, y);
    printf("\033[38;5;228m╔");
    MoverCursor(x+ANCHO-1, y);
    printf("\033[38;5;228m╗");
    

    //Laterales
    for (i = 1; i < ALTO - 1; i++) {
        MoverCursor(x, y + i);
        EsperarMiliSeg(TIEMPO_BASE);
        printf("\033[38;5;32m║");
        printf("\033[0m");
        MoverCursor(x + ANCHO-1, y + i);
        EsperarMiliSeg(TIEMPO_BASE);
        printf("\033[38;5;32m║");
        printf("\033[0m");
    }
    MoverCursor(x, y+4);
    printf("\033[38;5;228m╠");
    MoverCursor(x+ANCHO-1, y+4);
    printf("\033[38;5;228m╣");

    //Parte inferior
    for (i = 0; i < ANCHO; i++) {
        MoverCursor(x + i, y + ALTO - 1);
        EsperarMiliSeg(TIEMPO_BASE);
        printf("\033[38;5;228m═");
        printf("\033[0m");
    }
    MoverCursor(x, y+ALTO-1);
    printf("\033[38;5;228m╚");
    MoverCursor(x+ANCHO-1, y+ALTO-1);
    printf("\033[38;5;228m╝");

    MoverCursor(x+ANCHO/2-5, y+1);
    printf("\033[0m Monitor de CPU");
    MoverCursor(x+ANCHO/2-6, y+3);
    printf("\033[0m [ EN EJECUCION ]");

}

/*
void dibujarTitulo()
Recibe: void (no recibe valor explícito)
Devuelve: void (no retorna valor explícito)
Observaciones: Se dibuja una serie de astericos que imprimen en conjunto la palabra "ESCOM"
*/
void dibujarTitulo(int xTitulo, int yTitulo)
{
    MoverCursor(xTitulo,yTitulo);
    printf("******  ******  ******  ******  ****  ****\n");
    MoverCursor(xTitulo,yTitulo+1);
    printf("*       *       *       *    *  *  *  *  *\n");
    MoverCursor(xTitulo,yTitulo+2);
    printf("******  ******  *       *    *  *  ****  *\n");
    MoverCursor(xTitulo,yTitulo+3);    
    printf("*            *  *       *    *  *        *\n");
    MoverCursor(xTitulo,yTitulo+4);
    printf("******  ******  ******  ******  *        *\n");
}

/*
void dibujarMargen()
Recibe: void (no recibe valor explícito)
Devuelve: void (no retorna valor explícito)
Observaciones: Dibuja un margen en pantalla considerando las dimnensiones previamente definidas del SHELL
*/
void dibujarMargen()
{
    int i;
    int x = 0; 
    int y = 0;
    //Para la parte superior e inferior del margen, considerando el alto del shell
    for(i=0;i<ANCHOSHELL;i++)
    {
        MoverCursor(x+i,y);
        printf("\033[38;5;32m*");
        printf("\033[0m");
        EsperarMiliSeg(TIEMPO_BASE);
        MoverCursor(x+i,y+ALTOSHELL-1);
        printf("\033[38;5;32m*");
        printf("\033[0m");
        EsperarMiliSeg(TIEMPO_BASE);
    }
    //Laterales del margen, considerando el ancho del shell
    for(i=1;i<=ALTOSHELL-1;i++)
    {
        MoverCursor(x,y+i);
        printf("\033[38;5;228m*");
        printf("\033[0m");
        EsperarMiliSeg(TIEMPO_BASE);
        MoverCursor(x+ANCHOSHELL-1,y+i);
        printf("\033[38;5;228m*");
        printf("\033[0m");
        EsperarMiliSeg(TIEMPO_BASE);
    }
}

/*
void animacionProceso1()
Recibe: void (no recibe valor explícito)
Devuelve: void (no retorna valor explícito)
Observaciones: Se dibujan una serie de asteriscos que simulan la entrada de un proceso a la pantalla principal
*/
void animacionProceso1()
{   
    int i;
    int x=18, y=12;
    int pos=(ANCHOSHELL-ANCHO)/2;
    for(i=17;i>11;i--)
    {
        MoverCursor(x,i);
        printf("\033[38;5;104m*");
        printf("\033[0m");
        EsperarMiliSeg(TIEMPO_BASE);
    }
    for(i=18;i<=pos;i++)
    {
        MoverCursor(i,y);
        printf("\033[38;5;104m*");
        printf("\033[0m");
        EsperarMiliSeg(TIEMPO_BASE);
    }
    return;
}

/*
void animacionProceso1()
Recibe: void (no recibe valor explícito)
Devuelve: void (no retorna valor explícito)
Observaciones: Se borra la animacion 1
*/
void borrarAnimacionProceso1()
{
    int i;
    int x=18, y=12;
    int pos=(ANCHOSHELL-ANCHO)/2;
    for(i=17;i>11;i--)
    {
        MoverCursor(x,i);
        printf(" ");
        EsperarMiliSeg(TIEMPO_BASE);
    }
    for(i=18;i<pos;i++)
    {
        MoverCursor(i,y);
        printf(" ");
        EsperarMiliSeg(TIEMPO_BASE);
    }
    return;
}

/*
void animacionProceso2()
Recibe: void (no recibe valor explícito)
Devuelve: void (no retorna valor explícito)
Observaciones: Se dibujan una serie de asteriscos que simulan la salida de un proceso de la pantalla principal
*/
void animacionProceso2()
{
    int i;
    int y=12;
    int pos=(ANCHOSHELL-ANCHO)/2 + ANCHO;
    
    for(i=0;i<=17;i++)
    {
        MoverCursor(pos+i,y);
        printf("\033[38;5;104m*");
        printf("\033[0m");
        EsperarMiliSeg(TIEMPO_BASE);
    }
    for(i=y;i<=15;i++)
    {
        MoverCursor(pos+17,i);
        printf("\033[38;5;104m*");
        printf("\033[0m");
        EsperarMiliSeg(TIEMPO_BASE);
    }
}

/*
void animacionProceso1()
Recibe: void (no recibe valor explícito)
Devuelve: void (no retorna valor explícito)
Observaciones: Se borra la animacion 2
*/
void borrarAnimacionProceso2()
{
    int i;
    int y=12;
    int pos=(ANCHOSHELL-ANCHO)/2 + ANCHO;
    
    for(i=0;i<=17;i++)
    {
        MoverCursor(pos+i,y);
        printf(" ");
        EsperarMiliSeg(TIEMPO_BASE);
    }
    for(i=y;i<=19;i++)
    {
        MoverCursor(pos+17,i);
        printf(" ");
        EsperarMiliSeg(TIEMPO_BASE);
    }
}

/*
void dibujarCuadro1()
Recibe: void (no recibe valor explícito)
Devuelve: void (no retorna valor explícito)
Observaciones: dibuja el recuadro para "proximo proceso a atender"
*/
void dibujarCuadro1()
{
    int i;
    int x=5;
    int y=19;
    for(i=0;i<=ANCHOCUADRO;i++)
    {
        MoverCursor(x+i,y);
        printf("-");
        EsperarMiliSeg(TIEMPO_BASE);
        MoverCursor(x+i,y+5);
        printf("-");
        EsperarMiliSeg(TIEMPO_BASE);
    }
    for(i=0;i<=ALTOCUADRO;i++)
    {
        MoverCursor(x+ANCHOCUADRO,y+i);
        printf("-");
        EsperarMiliSeg(TIEMPO_BASE);
    }
    for(i=0;i<=ALTOCUADRO;i++)
    {
        MoverCursor(x,y+i);
        printf("-");
        EsperarMiliSeg(TIEMPO_BASE);
    }
}

/*
void dibujarCuadro2()
Recibe: void (no recibe valor explícito)
Devuelve: void (no retorna valor explícito)
Observaciones: dibuja el recuadro para "Ultimo proceso atendido"
*/
void dibujarCuadro2()
{
    int i;
    int x=5;
    int y=27;
    for(i=0;i<=ANCHOCUADRO;i++)
    {
        MoverCursor(x+i,y);
        printf("-");
        EsperarMiliSeg(TIEMPO_BASE);
        MoverCursor(x+i,y+5);
        printf("-");
        EsperarMiliSeg(TIEMPO_BASE);
    }
    for(i=0;i<=ALTOCUADRO;i++)
    {
        MoverCursor(x+ANCHOCUADRO,y+i);
        printf("-");
        EsperarMiliSeg(TIEMPO_BASE);
    }
    for(i=0;i<=ALTOCUADRO;i++)
    {
        MoverCursor(x,y+i);
        printf("-");
        EsperarMiliSeg(TIEMPO_BASE);
    }
}


