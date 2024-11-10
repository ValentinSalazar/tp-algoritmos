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
#define TOMAR_PEDIDO 'T'

#define JUEGO_GANADO 1
#define JUEGO_PERDIDO -1

// Pre:
// Post: Imprime los movimientos validos que puede realizar el jugador.
void imprimir_movimientos_validos(){
    printf("Mover arriba (%c)\n", MOVER_ARRIBA);
    printf("Mover abajo (%c)\n", MOVER_ABAJO);
    printf("Mover derecha (%c)\n", MOVER_DER);
    printf("Mover Izquierda (%c)\n", MOVER_IZQ);
}
// Pre: Llegara un caracter por parametro representando la jugada que ha hecho el usuario.
// Post: Verifica que esa jugada es valida (W,A,S,D o accion de mopa) y retorna true en caso que sea valida, en caso contrario false.
bool es_jugada_valida(char jugada){
    return jugada == MOVER_ARRIBA || jugada == MOVER_ABAJO || jugada == MOVER_DER || jugada == MOVER_IZQ || jugada == ACCION_MOPA || jugada == TOMAR_PEDIDO;
}

// Pre: El puntero de la jugada debe estar inicializado.
// Post: Solicita al usuario un caracter representando una jugada y se la asigna al puntero de la jugada.
void solicitar_jugada(char* jugada){
    printf("Realice un movimiento: (mover o agarrar/soltar mopa):\n");
    imprimir_movimientos_validos();
    scanf(" %c", jugada);
}

int main(){
    system("clear");
    srand((unsigned)time(NULL));

    juego_t juego;

    inicializar_juego(&juego);
    mostrar_juego(juego);


    while(juego.movimientos < MAX_MOVIMIENTOS) {
        char jugada;
        do{
            solicitar_jugada(&jugada);
        } while(!es_jugada_valida(jugada));

        realizar_jugada(&juego, jugada);
        mostrar_juego(juego);
    }


    int estado = estado_juego(juego);

    if(estado == JUEGO_GANADO){
        printf("Ganaste el juego.");
    } else if (estado == JUEGO_PERDIDO){
        printf("Perdiste el juego.");
    } else {
        printf("Sigue jugando.");
    }

    return 0;
}
