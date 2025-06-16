#ifndef SUPERMERCADO_H
#define SUPERMERCADO_H

#include "TADColaDin.h" 

#define ALTO 24
//Para el control de las coordendas a emplear sera un atributo que tendremos para poner navegar
typedef struct {
    int x;
    int y;
} coordenada;

//Estructura clave de control de cada caja 

typedef struct {
    int tiempo_atencion;
    cola fila;
    int atendiendo;
    coordenada inicio_fila; 
    coordenada atencion;
    int cont_fila;
    coordenada pos_exceso; 
    int tiempo_restante;     
    int inicio_atencion; 
    int exceso_anterior; 

} caja;

#endif
