#include "restaurant.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

const int CANTIDAD_MESAS_INDIVIDUALES = 6;
const int CANTIDAD_MESAS_COMPARTIDAS = 4;

const int CANTIDAD_MONEDAS = 8;
const int CANTIDAD_PATINES = 5;
const int CANTIDAD_CHARCOS = 5;


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




// bool es_coordenada_ocupada(int* fila, int* columna, char terreno[MAX_FILAS][MAX_COLUMNAS]){
//     return(terreno[*fila][*columna] != ' ');
// }

// bool esta_dentro_limite(int* fila, int* columna){
//     return(*fila < 20 && *columna < 20);
// }

// bool es_coordenada_valida(coordenada_t coordenada, char terreno[MAX_FILAS][MAX_COLUMNAS]){
//     return((coordenada.fil < 20 && coordenada.col < 20) && (terreno[coordenada.fil][coordenada.col] == ' '));
// }


// Pre: La fila y la columna deben encontrarse entre 0 y 20
// Post: Se genera un numero random para cada indice [i, j]
coordenada_t generar_coordenada_random(char terreno[MAX_FILAS][MAX_COLUMNAS]){
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


int distancia_a_mesa(coordenada_t primer_coordenada, coordenada_t segunda_coordenada){
    double distancia = 0;

    distancia = sqrt(pow(primer_coordenada.fil - segunda_coordenada.fil, 2) + pow(primer_coordenada.col - segunda_coordenada.col, 2));


    return (int)distancia;
}

bool es_distancia_valida(mesa_t mesas[MAX_MESAS], int* cantidad_mesas, mesa_t mesa_nueva){
    bool distancia_valida = true;
    if(*cantidad_mesas == 0){
        distancia_valida = true;
    } else {
        bool hay_posicion_invalida = false;
        int i = 0;
        int j = 0;
        int k = 0;
        while(i < *cantidad_mesas && !hay_posicion_invalida){
            while(j < mesas[i].cantidad_lugares && !hay_posicion_invalida){
                while(k < mesa_nueva.cantidad_lugares && !hay_posicion_invalida){
                    if(distancia_a_mesa(mesas[i].posicion[j], mesa_nueva.posicion[k]) <= 1) { 
                        distancia_valida = false;
                        hay_posicion_invalida = true;
                    }
                    k++;
                }
                k = 0;
                j++;
            }
            j = 0;
            i++;
        }

    }

    return distancia_valida;
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

mesa_t crear_mesa_individual(coordenada_t coordenada){
    mesa_t mesa_nueva;
    mesa_nueva.cantidad_lugares = 0;
    mesa_nueva.cantidad_comensales = MIN_COMENSALES;
    mesa_nueva.posicion[mesa_nueva.cantidad_lugares] = coordenada;
    mesa_nueva.cantidad_lugares += 1;


    return mesa_nueva;
}

void asignar_mesa(mesa_t mesa, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    for(int i = 0; i < mesa.cantidad_lugares; i++) {
        terreno[mesa.posicion[i].fil][mesa.posicion[i].col] = MESA;
    }
}




// Pre: Es necesario que las mesas, su cantidad y el terreno esten inicializados.
// Post: Inicializa las mesas compartidas siempre y cuando se encuentren en una posicion valida.
void inicializar_mesas(mesa_t mesas[MAX_MESAS], int* cantidad_mesas, char terreno[MAX_FILAS][MAX_COLUMNAS]){  

    int contador_mesas_creadas = 0;
    while(contador_mesas_creadas < CANTIDAD_MESAS_COMPARTIDAS) {
        coordenada_t coordenada_nueva_mesa = generar_coordenada_random(terreno);
        mesa_t mesa_generada = crear_mesa_compartida(coordenada_nueva_mesa);

        if(es_distancia_valida(mesas, cantidad_mesas, mesa_generada)) {
            asignar_mesa(mesa_generada, terreno);
            mesas[*cantidad_mesas] = mesa_generada;
            *cantidad_mesas += 1;
            contador_mesas_creadas += 1;
        }
    }
    contador_mesas_creadas = 0;
    while(contador_mesas_creadas < CANTIDAD_MESAS_INDIVIDUALES) {
        coordenada_t coordenada_nueva_mesa = generar_coordenada_random(terreno);
        mesa_t mesa_generada = crear_mesa_individual(coordenada_nueva_mesa);
        if(es_distancia_valida(mesas, cantidad_mesas, mesa_generada)) {
            asignar_mesa(mesa_generada, terreno);
            mesas[*cantidad_mesas] = mesa_generada;
            *cantidad_mesas += 1;
            contador_mesas_creadas += 1;
        }
    }
    
}



void inicializar_cocina(char terreno[MAX_FILAS][MAX_COLUMNAS]){
    coordenada_t coordenada = generar_coordenada_random(terreno);

    cocina_t cocina;
    cocina.posicion = coordenada;

    terreno[cocina.posicion.fil][cocina.posicion.col] = COCINA;
}

void inicializar_linguini(char terreno[MAX_FILAS][MAX_COLUMNAS]){
    coordenada_t coordenada_linguini = generar_coordenada_random(terreno);

    terreno[coordenada_linguini.fil][coordenada_linguini.col] = PERSONAJE;
}

void inicializar_objeto(objeto_t objeto, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    coordenada_t coordenada_objeto = generar_coordenada_random(terreno);

    objeto.posicion = coordenada_objeto;

    terreno[objeto.posicion.fil][objeto.posicion.col] = objeto.tipo;
    

}

void inicializar_juego(juego_t* juego){

    char terreno[MAX_FILAS][MAX_COLUMNAS];
    inicializar_terreno(terreno);
    
    // primero las mesas -> recibo un arrays de mesas y el contador de mesas. 
    
    mesa_t mesas[MAX_MESAS];
    int cantidad_mesas = 0;
    
    inicializar_mesas(mesas, &cantidad_mesas, terreno);

    // segundo cocina
    inicializar_cocina(terreno);
    

    // tercero linguini
    inicializar_linguini(terreno);
    

    // cuarto mopa
    objeto_t mopa;
    mopa.tipo = MOPA;
    inicializar_objeto(mopa, terreno);
    
    


    // quinto monedas
    for(int i = 0; i < CANTIDAD_MONEDAS; i++) {
        objeto_t moneda;
        moneda.tipo = MONEDA;
        inicializar_objeto(moneda, terreno);
    }


    // sexto patines
    for(int i = 0; i < CANTIDAD_PATINES; i++) {
        objeto_t patines;
        patines.tipo = PATINES;
        inicializar_objeto(patines, terreno);
    }


    // septimo charcos
    for(int i = 0; i < CANTIDAD_CHARCOS; i++) {
        objeto_t charco;
        charco.tipo = CHARCOS;
        inicializar_objeto(charco, terreno);
    }
    imprimir_terreno(terreno);
}

