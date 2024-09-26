#include "restaurant.h"

#include <stdlib.h>

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

// Pre: La fila y la columna deben encontrarse entre 0 y 20.
// Post: Se genera un numero random para cada indice.
coordenada_t generar_coordenadas_random_validas(char terreno[MAX_FILAS][MAX_COLUMNAS]){
    coordenada_t coordenada = {-1, -1};
    bool flag = true;

    while(flag) {
        int fila = rand() % MAX_FILAS + 0;
        int columna = rand() % MAX_COLUMNAS + 0;
        if((terreno[fila][columna] == ' ') && (fila < 20 && columna < 20)) {
            coordenada.fil = fila;
            coordenada.col = columna;
            flag = false;
        }
    }

    return coordenada;
}



void crear_mesa_compartida(coordenada_t coordenadas[MAX_POSICION], mesa_t* mesa_creada, mesa_t mesas[MAX_MESAS], int* cantidad_mesas){
    for(int i = 0; i < MAX_POSICION; i++) {
        mesa_creada->posicion[i] = coordenadas[i];
        mesas[*cantidad_mesas] = *mesa_creada;
        *cantidad_mesas += 1;
    }
}

bool es_coordenada_ocupada(int* fila, int* columna, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    return(terreno[*fila][*columna] != ' ');
}

bool esta_dentro_limite(int* fila, int* columna){
    return(*fila < 20 && *columna < 20);
}

// void completar_mesa_compartida(coordenada_t* coordenada_principal) {
//     coordenada_t coordenada_secundaria;
//     while()


// }

void asignar_mesa(coordenada_t* coordenadas_mesa, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    terreno[coordenadas_mesa->fil][coordenadas_mesa->col] = 'T';
}

void inicializar_mesas(mesa_t mesas[MAX_MESAS], int* cantidad_mesas, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    
    coordenada_t coordenadas_mesa_compartidas[MAX_COMENSALES];
    coordenada_t coordenada_mesa = generar_coordenadas_random_validas(terreno);
    asignar_mesa(&coordenada_mesa, terreno);


    // int contador_mesas_creadas = 1;
    // while (contador_mesas_creadas < CANTIDAD_MESAS_COMPARTIDAS){
        
        

    //     int i = coordenada_mesa.fil;
    //     int j = coordenada_mesa.col + 1;
    //     // while(i == ' ' && i < 20) {
    //     //     coordenadas_mesa_compartidas = 
    //     // }

    //     coordenada_t coordenada_mesa_2 = {coordenada_mesa.fil, coordenada_mesa.col + 1};
    //     coordenada_t coordenada_mesa_3 = {i+1, j};
    //     coordenada_t coordenada_mesa_4 = {i+1, j+1};

    //     contador_mesas_creadas += 1;
    // }

    // int contador_mesas_creadas = 0;
    // while(contador_mesas_creadas < CANTIDAD_MESAS_INDIVIDUALES) {
    //     // generar_coordenadas_random(&i, &j);

    //     // if(i < 20 && j < 20){
    //     //     coordenada_t coordenada_mesa = {i, j};
    //     //     mesa_t mesa;
    //     //     crear_mesa_individual(&coordenada_mesa, mesas, &cantidad_mesas, MIN_COMENSALES);
    //     //     contador_mesas_creadas += 1;
    //     // }
    // }
    
}


//! IMPORTANTE
// A la hora de inicializar las demas cosas, antes debo crear una funcion que me verifique las posiciones de las mesas.

void inicializar_juego(juego_t* juego){

    char terreno[MAX_FILAS][MAX_COLUMNAS];
    inicializar_terreno(terreno);

    imprimir_terreno(terreno);

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

