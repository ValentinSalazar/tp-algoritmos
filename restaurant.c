#include "restaurant.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

const int CANTIDAD_MESAS_INDIVIDUALES = 6;
const int CANTIDAD_MESAS_COMPARTIDAS = 4;


void inicializar_terreno(char terreno_resto[MAX_FILAS][MAX_COLUMNAS]){
    for(int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            terreno_resto[i][j] = ' ';
        }
    }
}


void imprimir_terreno(char terreno[MAX_FILAS][MAX_COLUMNAS]) {
    for(int i = 0; i < MAX_FILAS; i++) {
        printf("\n");
        for(int j = 0; j < MAX_COLUMNAS; j++) {
            printf(" |%c| ", terreno[i][j]);

        }
    }
}

// Pre: La fila y la columna deben encontrarse entre 0 y 20
// Post: Se genera un numero random para cada indice [i, j]
coordenada_t generar_coordenada_mesa_compartida(char terreno[MAX_FILAS][MAX_COLUMNAS]){
    coordenada_t coordenada = {-1, -1};
    bool flag = true;

    while(flag) {
        int fila = rand() % (MAX_FILAS - 1) + 0;
        int columna = rand() % (MAX_COLUMNAS - 1) + 0;
        if((terreno[fila][columna] == ' ') && (fila < 19 && columna < 19)) {
            coordenada.fil = fila;
            coordenada.col = columna;
            flag = false;
        }
    }

    return coordenada;
}


// bool es_coordenada_ocupada(int* fila, int* columna, char terreno[MAX_FILAS][MAX_COLUMNAS]){
//     return(terreno[*fila][*columna] != ' ');
// }

// bool esta_dentro_limite(int* fila, int* columna){
//     return(*fila < 20 && *columna < 20);
// }

// bool es_coordenada_valida(coordenada_t coordenada, char terreno[MAX_FILAS][MAX_COLUMNAS]){
//     return((coordenada.fil < 20 && coordenada.col < 20) && (terreno[coordenada.fil][coordenada.col] == ' '));
// }




int distancia_a_mesa(coordenada_t primer_coordenada, coordenada_t segunda_coordenada){
    double distancia = 0;

    distancia = sqrt(pow(primer_coordenada.fil - segunda_coordenada.fil, 2) + pow(primer_coordenada.col - segunda_coordenada.col, 2));
    printf("distancia:%f\n", distancia);


    return (int)distancia;
}

mesa_t crear_mesa_compartida(coordenada_t coordenada){
    
    mesa_t mesa_creada;
    mesa_creada.cantidad_comensales = MAX_COMENSALES;

    mesa_creada.cantidad_lugares = 0;
    
    
    mesa_creada.posicion[mesa_creada.cantidad_lugares] = coordenada;
    mesa_creada.cantidad_lugares += 1;

    
    coordenada_t coordenada_secundaria = {coordenada.fil, coordenada.col + 1};
    mesa_creada.posicion[mesa_creada.cantidad_lugares] = coordenada_secundaria;
    mesa_creada.cantidad_lugares += 1;

    
    coordenada_t coordenada_terciaria = {coordenada.fil + 1, coordenada.col};
    mesa_creada.posicion[mesa_creada.cantidad_lugares] = coordenada_terciaria;
    mesa_creada.cantidad_lugares += 1;

    
    coordenada_t coordenada_cuarta = {coordenada.fil + 1, coordenada.col + 1};
    mesa_creada.posicion[mesa_creada.cantidad_lugares] = coordenada_cuarta;
    mesa_creada.cantidad_lugares += 1;



    return mesa_creada;
}


void asignar_mesa(mesa_t mesa, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    for(int i = 0; i < mesa.cantidad_lugares; i++) {
        terreno[mesa.posicion[i].fil][mesa.posicion[i].col] = 'T';
    }
}



bool es_distancia_valida(mesa_t mesas[MAX_MESAS], int* cantidad_mesas, mesa_t mesa_nueva){
    bool distancia_valida = true;
    if(*cantidad_mesas == 0){
        distancia_valida = true;
    } else {
        bool hay_posicion_invalida = false;
        int i = 1;
        int j = 0;
        while(i < *cantidad_mesas && !hay_posicion_invalida){
            while(j < mesas[i].cantidad_lugares && !hay_posicion_invalida){
                if(distancia_a_mesa(mesas[i].posicion[j], mesa_nueva.posicion[j]) <= 1) {
                    distancia_valida = false;
                    hay_posicion_invalida = true;
                    
                }
                j++;
            }
            i++;
            j = 0;
        }

    }

    return distancia_valida;
}


// Pre: Es necesario que las mesas, su cantidad y el terreno esten inicializados.
// Post: Inicializa las mesas compartidas siempre y cuando se encuentren en una posicion valida.
void inicializar_mesas(mesa_t mesas[MAX_MESAS], int* cantidad_mesas, char terreno[MAX_FILAS][MAX_COLUMNAS]){  

    int contador_mesas_creadas = 0;
    while(contador_mesas_creadas < CANTIDAD_MESAS_COMPARTIDAS) {
        coordenada_t coordenada_nueva_mesa = generar_coordenada_mesa_compartida(terreno);
        mesa_t mesa_generada = crear_mesa_compartida(coordenada_nueva_mesa);

        if(es_distancia_valida(mesas, cantidad_mesas, mesa_generada)) {
            asignar_mesa(mesa_generada, terreno);
            mesas[*cantidad_mesas] = mesa_generada;
            *cantidad_mesas += 1;
            contador_mesas_creadas += 1;
        }
        
    }  
    
}

//! IMPORTANTE
// A la hora de inicializar las demas cosas, antes debo crear una funcion que me verifique las posiciones de las mesas.

void inicializar_juego(juego_t* juego){

    char terreno[MAX_FILAS][MAX_COLUMNAS];
    inicializar_terreno(terreno);
    
    // primero las mesas -> recibo un arrays de mesas y el contador de mesas. 
    
    mesa_t mesas[MAX_MESAS];
    int cantidad_mesas = 0;
    
    inicializar_mesas(mesas, &cantidad_mesas, terreno);
    imprimir_terreno(terreno);

    // segundo cocina

    // tercero linguini

    // cuarto mopa

    // quinto monedas

    // sexto patines

    // septimo charcos
}

