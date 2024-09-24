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

// Pre: La fila y la columna deben encontrarse entre 0 y 20.
// Post: Se genera un numero random para cada indice.
void generar_coordenadas_random(int* fila, int* columna){
    *fila = rand() % MAX_FILAS + 0;
    *columna = rand() % MAX_COLUMNAS + 0;
}


// Pre: La coordenada debe encontrarse entre la filas 0 y 20, al igual que las columnas. Y por otro lado,
//      los comensales deben ser 1 o 4.
// Post: Se crea una mesa y se le asigna una coordenada, luego se agrega al vector de mesas.
void crear_mesa(coordenada_t* coordenada, mesa_t mesas[MAX_MESAS], int* cantidad_mesas, int comensales){
    mesa_t mesa_creada;
    *mesa_creada.posicion = *coordenada;
    mesa_creada.cantidad_comensales = comensales;
    mesas[*cantidad_mesas] = mesa_creada;
    *cantidad_mesas += 1;
}


void inicializar_mesas(mesa_t mesas[MAX_MESAS], int* cantidad_mesas){
    int contador_mesas_creadas = 0;
    int i = -1;
    int j = -1;
    while (contador_mesas_creadas < CANTIDAD_MESAS_COMPARTIDAS){
        generar_coordenadas_random(&i, &j);

        //! no esta bien, porque las mesas de 4 tienen un vector de coordenadas porque necesita 4 coordenadas. lo arreglo mañana desp de analisis.
        if((i < 20) && (j < 20) && ((i + 1) < 20) && ((j + 1) < 20)){
            coordenada_t coordenada_mesa = {i, j};
            // terminar esto:
            //coordenada_t coordenada_mesa_2 = {i, j+1};
            //coordenada_t coordenadas_mesa_compartidas[MAX_COMENSALES] = {coordenada_mesa, coordenada_mesa_2};
            mesa_t mesa;
            crear_mesa(&coordenada_mesa, mesas, &cantidad_mesas, MAX_COMENSALES);
            contador_mesas_creadas += 1;
        }
    }

    int contador_mesas_creadas = 0;
    while(contador_mesas_creadas < CANTIDAD_MESAS_INDIVIDUALES) {
        generar_coordenadas_random(&i, &j);

        if(i < 20 && j < 20){
            coordenada_t coordenada_mesa = {i, j};
            mesa_t mesa;
            crear_mesa(&coordenada_mesa, mesas, &cantidad_mesas, MIN_COMENSALES);
            contador_mesas_creadas += 1;
        }
    }
    
}


//! IMPORTANTE
// A la hora de inicializar las demas cosas, antes debo crear una funcion que me verifique las posiciones de las mesas.

void inicializar_juego(juego_t* juego){
    // primero las mesas -> recibo un arrays de mesas y el contador de mesas. 
    mesa_t mesas[MAX_MESAS];
    int cantidad_mesas = 0;
    inicializar_mesas(mesas, &cantidad_mesas);
    // segundo cocina

    // tercero linguini

    // cuarto mopa

    // quinto monedas

    // sexto patines

    // septimo charcos
}