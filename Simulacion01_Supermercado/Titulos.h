/*
Autor: Escompiladores
Version 1.0 (25 de Septiembre 2012)
Descripcion: Cabecera de la libreria para recrear presentaciones mas agradables al usuario 
en el modo consola en cuanto a titulo de presentacion 

Observaciones: Ninguna
*/
#include "supermercado.h"

//DECLARACION DE FUNCIONES
void DibujarTitulo(const char *titulo, int x, int y );
void DibujarTituloCentrado(const char *titulo);
void DibujarContorno();
void DibujarCajas(int cantidadCajeros, caja *info_cajas);
void MostrarNumeroClientesAtendidos(int cantidad);
void MostrarAnuncioCierre();
void DibujarTituloCentradoEnY(const char *titulo, int y);

