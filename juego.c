#include <time.h>
#include "restaurant.h"

#include <stdlib.h>
#include <stdio.h>


#define MAX_MOVIMIENTOS 200

#define MOVER_ARRIBA 'W'
#define MOVER_ABAJO 'S'
#define MOVER_DER 'D'
#define MOVER_IZQ 'A'
#define ACCION_MOPA 'O'


// Pre:
// Post: Imprime los movimientos validos que puede realizar el jugador.
void imprimir_movimientos_validos(){
    printf("Mover arriba (W)\n");
    printf("Mover abajo (S)\n");
    printf("Mover derecha (D)\n");
    printf("Mover Izquierda (A)\n");
}
// Pre: Llega una jugada por parametro.
// Post: Verifica que esa jugada es valida y retorna true en caso correcto, en caso contrario false.
bool es_jugada_valida(char jugada){
    return jugada == MOVER_ARRIBA || jugada == MOVER_ABAJO || jugada == MOVER_DER || jugada == MOVER_IZQ || jugada == ACCION_MOPA;
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