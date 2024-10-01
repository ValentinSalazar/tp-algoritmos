#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "restaurant.c"

void imprimir_movimientos_validos(){
    printf("Mover arriba (W)\n");
    printf("Mover abajo (S)\n");
    printf("Mover derecha (D)\n");
    printf("Mover Izquierda (A)\n");
}

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
        printf("cant mov:%i\n", juego.movimientos);
        
        scanf(" %c", &jugada);
        if(!es_jugada_valida(jugada)){
            printf("Realice un movimiento valido (mover o agarrar/soltar mopa): ");
            imprimir_movimientos_validos();
            scanf(" %c", &jugada);
        }
        printf("cant mov22:%i\n", juego.movimientos);
        realizar_jugada(&juego, jugada);
        mostrar_juego(juego);
    }

    return 0;
}