#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "restaurant.c"



int main(){
    system("clear");
    srand((unsigned)time(NULL));

    juego_t juego;

    inicializar_juego(&juego);
    mostrar_juego(juego);   

    while(juego.movimientos < MAX_MOVIMIENTOS) {
        char jugada;
        printf("Realice un movimiento: (mover o agarrar/soltar mopa):\n");
        imprimir_movimientos_validos();
        
        scanf(" %c", &jugada);
        if(!es_jugada_valida(jugada)){
            printf("Realice un movimiento valido (mover o agarrar/soltar mopa): ");
            imprimir_movimientos_validos();
            scanf(" %c", &jugada);
        }
        realizar_jugada(&juego, jugada);
        mostrar_juego(juego);
    }


    int resultado = estado_juego(juego);
    if(resultado == 1){
        printf("Ganaste el juego.");
    } else if (resultado == -1){
        printf("Perdiste el juego.");
    } else {
        printf("Sigue jugando.");
    }

    return 0;
}