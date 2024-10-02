#include "restaurant.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

const int CANTIDAD_MESAS_INDIVIDUALES = 6;
const int CANTIDAD_MESAS_COMPARTIDAS = 4;

const int CANTIDAD_MONEDAS = 8;
const int CANTIDAD_PATINES = 5;
const int CANTIDAD_CHARCOS = 5;

const int ESCONDER_MOPA = 200;

const int MONEDAS_GANAR_JUEGO = 150000;

// Pre: 
// Post: Inicializa todas las posiciones del terreno con un espacio vacio -> ' '
void inicializar_terreno(char terreno_resto[MAX_FILAS][MAX_COLUMNAS]){
    for(int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            terreno_resto[i][j] = ' ';
        }
    }
}

// Pre: Debe estar previamente inicializado.
// Post: Recorre la matriz del terreno e imprime todas sus posiciones.
void imprimir_terreno(char terreno[MAX_FILAS][MAX_COLUMNAS]) {

    for(int i = 0; i < MAX_FILAS; i++) {
        printf("\n");
        for(int j = 0; j < MAX_COLUMNAS; j++) {
            printf(" |%c| ", terreno[i][j]);

        }
    }
}


// Pre:
// Post: Imprime los movimientos validos que puede realizar el jugador.
void imprimir_movimientos_validos(){
    printf("Mover arriba (W)\n");
    printf("Mover abajo (S)\n");
    printf("Mover derecha (D)\n");
    printf("Mover Izquierda (A)\n");
}


// Pre: El terreno debe estar previamente inicializado.
// Post: Genera una coordenada random dentro de los limites de la matriz y en una posición vacía.
//       Luego, esa coordenada la devuelvo.
coordenada_t generar_coordenada_random(char terreno[MAX_FILAS][MAX_COLUMNAS]){
    coordenada_t coordenada = {-1, -1};
    bool flag = true;

    while(flag) {
        int fila = rand() % (MAX_FILAS - 1) + 0;
        int columna = rand() % (MAX_COLUMNAS - 1) + 0;
        if((terreno[fila][columna] == ' ') && (fila < (MAX_FILAS - 1) && columna < (MAX_COLUMNAS - 1))) {
            coordenada.fil = fila;
            coordenada.col = columna;
            flag = false;
        }
    }

    return coordenada;
}


// Pre: Las coordenadas deben estar inicializadas.
// Post: Calcula la distancia entre 2 coordenadas y la devuelve, para posteriormente ser analizada.
int distancia_a_mesa(coordenada_t primer_coordenada, coordenada_t segunda_coordenada){
    double distancia = 0;

    distancia = sqrt(pow(primer_coordenada.fil - segunda_coordenada.fil, 2) + pow(primer_coordenada.col - segunda_coordenada.col, 2));


    return (int)distancia;
}


// Pre: El tope de la cantidad y la mesa nueva deben estar inicializadas.
// Post: En caso de que no haya mesas en el juego, retornará que la distancia de la mesa nueva es valida.
//      Pero, si hay deberá comparar las coordenadas de cada una de las mesas que esten disponibles con cada coordenada de la mesa nueva,
//      ya sea compartida o individual.
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


// Pre: La coordenada ya debe estar inicializada.
// Post: Crea y devuelve una mesa compartida y le asigna la coordenada que llega por parametro. (coordenada random)
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

// Pre: La coordenada ya debe estar inicializada.
// pOST: Crea y devuelve una mesa individual y le asigna la coordenada que llega por parametro. (coordenada randmom)
mesa_t crear_mesa_individual(coordenada_t coordenada){
    mesa_t mesa_nueva;
    mesa_nueva.cantidad_lugares = 0;
    mesa_nueva.cantidad_comensales = MIN_COMENSALES;
    mesa_nueva.posicion[mesa_nueva.cantidad_lugares] = coordenada;
    mesa_nueva.cantidad_lugares += 1;


    return mesa_nueva;
}


// Pre: La mesa y el terreno deben estar previamente inicializados.
// Post: Obtengo una mesa por parametro (ya sea compartida o individual) y la asigna al terreno. Es decir,
//      en la coordenada/s correspondiente de la mesa, dibuja una 'T' en representación de una mesa.
void asignar_mesa(mesa_t mesa, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    for(int i = 0; i < mesa.cantidad_lugares; i++) {
        terreno[mesa.posicion[i].fil][mesa.posicion[i].col] = MESA;
    }
}

// Pre: Es necesario que las mesas, su cantidad y el terreno esten inicializados.
// Post: Crea mesas compartidas e individuales y las va asignando al array de las mesas. 
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

// Pre: El juego y el terreno deben estar previamente inicializados.
// Post: Dada una coordenada random generada, se la asigna a la cocina.
void inicializar_cocina(juego_t* juego, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    coordenada_t coordenada = generar_coordenada_random(terreno);

    cocina_t cocina;
    cocina.posicion = coordenada;

    juego->cocina.posicion = coordenada;
    
}

// Pre: El juego y el terreno deben estar previamente inicializados.
// Post: Dada una coordenada random generada, se la asigna al personaje.
void inicializar_linguini(juego_t* juego,char terreno[MAX_FILAS][MAX_COLUMNAS]){
    coordenada_t coordenada_linguini = generar_coordenada_random(terreno);

    juego->mozo.posicion = coordenada_linguini;
    juego->mozo.tiene_mopa = false;

    terreno[coordenada_linguini.fil][coordenada_linguini.col] = PERSONAJE;
}

void inicializar_objeto(char tipo_objeto, char terreno[MAX_FILAS][MAX_COLUMNAS], juego_t* juego){
    coordenada_t coordenada_objeto = generar_coordenada_random(terreno);

    objeto_t nuevo_objeto;

    nuevo_objeto.posicion = coordenada_objeto;
    nuevo_objeto.tipo = tipo_objeto;

    juego->herramientas[juego->cantidad_herramientas] = nuevo_objeto;
    juego->cantidad_herramientas += 1;
    
}

void inicializar_obstaculo(char tipo_obstaculo, char terreno[MAX_FILAS][MAX_COLUMNAS], juego_t* juego){
    coordenada_t coordenada_obstaculo = generar_coordenada_random(terreno);

    objeto_t nuevo_obstaculo;
    nuevo_obstaculo.tipo = tipo_obstaculo;
    nuevo_obstaculo.posicion = coordenada_obstaculo;

    juego->obstaculos[juego->cantidad_obstaculos] = nuevo_obstaculo;
    juego->cantidad_obstaculos += 1;

}

void inicializar_juego(juego_t* juego){

    char terreno[MAX_FILAS][MAX_COLUMNAS];
    inicializar_terreno(terreno);

    
    juego->cantidad_mesas = 0;
    juego->cantidad_herramientas = 0;
    juego->dinero = 0;
    juego->movimientos = 0;
    juego->mozo.cantidad_pedidos = 0;
    juego->cantidad_obstaculos = 0;
    
    inicializar_mesas(juego->mesas, &juego->cantidad_mesas, terreno);

    inicializar_cocina(juego, terreno);
    
    inicializar_linguini(juego, terreno);
    
    inicializar_objeto(MOPA, terreno, juego);
    
    for(int i = 0; i < CANTIDAD_MONEDAS; i++) {
        inicializar_objeto(MONEDA, terreno, juego);
    }


    for(int i = 0; i < CANTIDAD_PATINES; i++) {
        inicializar_objeto(PATINES, terreno, juego);
    }

    for(int i = 0; i < CANTIDAD_CHARCOS; i++) {
        inicializar_obstaculo(CHARCOS, terreno, juego);
    }
   
}

bool es_jugada_valida(char jugada){
    return jugada == MOVER_ARRIBA || jugada == MOVER_ABAJO || jugada == MOVER_DER || jugada == MOVER_IZQ || jugada == ACCION_MOPA;
}

bool esta_dentro_limite(coordenada_t coordenada){
    return((coordenada.fil < (MAX_FILAS) && coordenada.col < (MAX_COLUMNAS )) && (coordenada.fil >= 0 && coordenada.col >= 0));
}


void posicionar_elementos_terreno(juego_t* juego, char terreno[MAX_FILAS][MAX_COLUMNAS]){

    for(int i = 0; i < juego->cantidad_mesas; i++){
        for(int p = 0; p < juego->mesas[i].cantidad_lugares; p++){
            terreno[juego->mesas[i].posicion[p].fil][juego->mesas[i].posicion[p].col] = MESA;
        }
    }
    terreno[juego->cocina.posicion.fil][juego->cocina.posicion.col] = COCINA;
    terreno[juego->mozo.posicion.fil][juego->mozo.posicion.col] = PERSONAJE;

    for(int i = 0; i < juego->cantidad_herramientas; i++){
        terreno[juego->herramientas[i].posicion.fil][juego->herramientas[i].posicion.col] = juego->herramientas[i].tipo;
    }


    for(int j = 0; j < juego->cantidad_obstaculos; j++) {
        terreno[juego->obstaculos[j].posicion.fil][juego->obstaculos[j].posicion.col] = juego->obstaculos[j].tipo;
    }
}

void mover_arriba(char terreno[MAX_FILAS][MAX_COLUMNAS], juego_t* juego){
    coordenada_t coordenada_accion = {juego->mozo.posicion.fil - 1, juego->mozo.posicion.col};
    char posicion_nueva = terreno[coordenada_accion.fil][coordenada_accion.col];
    if((posicion_nueva != MESA) && esta_dentro_limite(coordenada_accion)) {
        juego->mozo.posicion.fil = coordenada_accion.fil;
        juego->movimientos += 1;
    }
}

void mover_abajo(char terreno[MAX_FILAS][MAX_COLUMNAS], juego_t* juego){
    coordenada_t coordenada_accion = {juego->mozo.posicion.fil + 1, juego->mozo.posicion.col};
    char posicion_nueva = terreno[coordenada_accion.fil][coordenada_accion.col];
    if((posicion_nueva != MESA) && esta_dentro_limite(coordenada_accion)) {
        juego->mozo.posicion.fil = coordenada_accion.fil;
        juego->movimientos += 1;
    }
}

void mover_derecha(char terreno[MAX_FILAS][MAX_COLUMNAS], juego_t* juego){
    coordenada_t coordenada_accion = {juego->mozo.posicion.fil, juego->mozo.posicion.col + 1};
    char posicion_nueva = terreno[coordenada_accion.fil][coordenada_accion.col];
    if((posicion_nueva != MESA) && esta_dentro_limite(coordenada_accion)) {
        juego->mozo.posicion.col = coordenada_accion.col;
        juego->movimientos += 1;
    }
}

void mover_izquierda(char terreno[MAX_FILAS][MAX_COLUMNAS], juego_t* juego){
    coordenada_t coordenada_accion = {juego->mozo.posicion.fil, juego->mozo.posicion.col - 1};
    char posicion_nueva = terreno[coordenada_accion.fil][coordenada_accion.col];
    if((posicion_nueva != MESA) && esta_dentro_limite(coordenada_accion)) {
        juego->mozo.posicion.col = coordenada_accion.col;
        juego->movimientos += 1;
    }
}

void accion_de_mopa(char terreno[MAX_FILAS][MAX_COLUMNAS], juego_t* juego){
    for(int i = 0; i < juego->cantidad_herramientas; i++){
        bool misma_coordenada_mopa = juego->mozo.posicion.fil == juego->herramientas[i].posicion.fil && juego->mozo.posicion.col == juego->herramientas[i].posicion.col;
        bool es_mopa = juego->herramientas[i].tipo == MOPA;
        bool tiene_mopa = juego->mozo.tiene_mopa == true;

        if(es_mopa && misma_coordenada_mopa && !tiene_mopa) {
            terreno[juego->mozo.posicion.fil][juego->mozo.posicion.col] = 'K';
            juego->herramientas[i].posicion.fil = ESCONDER_MOPA;
            juego->herramientas[i].posicion.col = ESCONDER_MOPA;
            juego->mozo.tiene_mopa = true;
        } else if (es_mopa && tiene_mopa){
            terreno[juego->mozo.posicion.fil][juego->mozo.posicion.col] = MOPA;
            juego->herramientas[i].posicion.fil = juego->mozo.posicion.fil;
            juego->herramientas[i].posicion.col = juego->mozo.posicion.col;
            juego->mozo.tiene_mopa = false;
        }
    }
}

void realizar_jugada(juego_t* juego, char accion){

    char terreno[MAX_FILAS][MAX_COLUMNAS];
    inicializar_terreno(terreno);
    posicionar_elementos_terreno(juego, terreno);

    switch (accion){
    case ACCION_MOPA:
        accion_de_mopa(terreno, juego); 
        break;
    case MOVER_ARRIBA:
        mover_arriba(terreno, juego);
        break;
    case MOVER_ABAJO:
        mover_abajo(terreno, juego);
        break;
    case MOVER_IZQ:
        mover_izquierda(terreno, juego);
        break;
    case MOVER_DER:
        mover_derecha(terreno, juego);
        break;
    default:
        break;
    }
}

void mostrar_juego(juego_t juego) {
    char terreno[MAX_FILAS][MAX_COLUMNAS];
    inicializar_terreno(terreno);
    posicionar_elementos_terreno(&juego, terreno);

    system("clear");
    imprimir_terreno(terreno);
    printf("\n");

    printf("Cantidad de movimientos: %i\n", juego.movimientos);
    printf("Cantidad Pedidos: %i\n", juego.mozo.cantidad_pedidos);
    printf("Dinero: %i\n", juego.dinero);
}


int estado_juego(juego_t juego){
    int dinero_acumulado = juego.dinero;
    int estado = 0;
    if(dinero_acumulado >= MONEDAS_GANAR_JUEGO) {
        estado = 1;
    } else {
        estado = -1;
    }
    
    return estado;
}




// bool es_coordenada_ocupada(int* fila, int* columna, char terreno[MAX_FILAS][MAX_COLUMNAS]){
//     return(terreno[*fila][*columna] != ' ');
// }


// bool es_coordenada_valida(coordenada_t coordenada, char terreno[MAX_FILAS][MAX_COLUMNAS]){
//     return((coordenada.fil < 20 && coordenada.col < 20) && (terreno[coordenada.fil][coordenada.col] == ' '));
// }