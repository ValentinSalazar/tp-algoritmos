#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "restaurant.c"

int main(){

    srand((unsigned)time(NULL));

    juego_t juego;

    inicializar_juego(&juego);

    return 0;
}