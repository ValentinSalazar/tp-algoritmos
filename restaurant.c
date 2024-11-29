#include "restaurant.h"

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define MIN_COMENSALES 1
#define MAX_MOVIMIENTOS 200

#define COCINA 'C'
#define MESA 'T'
#define PERSONAJE 'L'
#define MOPA 'O'
#define MONEDA 'M'
#define PATINES 'P'
#define CHARCOS 'H'
#define CUCARACHA 'U'
#define MESA_OCUPADA 'X'

#define MOVER_ARRIBA 'W'
#define MOVER_ABAJO 'S'
#define MOVER_DER 'D'
#define MOVER_IZQ 'A'
#define ACCION_MOPA 'O'
#define TOMAR_PEDIDO 'T'
#define ACTIVAR_PATINES 'P'

#define JUEGO_GANADO 1
#define JUEGO_PERDIDO -1
#define SEGUIR_JUGANDO 0

#define POSICION_VACIA ' '

#define PRECIO_MESA_GRUPAL 20000
#define PRECIO_MESA_INDIVIDUAL 5000

const int ID_MESA_NAPOLITANA = 1;
const char MILANESA_NAPOLITANA = 'M';
const int TIEMPO_MILANESA_NAPOLITANA = 30;

const int ID_HAMBURGUESA = 2;
const char HAMBURGUESA = 'H';
const int TIEMPO_HAMBURGUESA = 15;

const int ID_PARRILLA = 3;
const char PARRILLA = 'P';
const int TIEMPO_PARRILLA = 20;

const int ID_RATATOUILLE = 4;
const char RATATOUILLE = 'R';
const int TIEMPO_RATATOUILLE = 25;

const int CANTIDAD_MESAS_INDIVIDUALES = 6;
const int CANTIDAD_MESAS_COMPARTIDAS = 4;

const int CANTIDAD_MONEDAS = 8;
const int CANTIDAD_PATINES = 5;
const int CANTIDAD_CHARCOS = 5;

const int ESCONDER_MOPA = 200;

const int MONEDAS_GANAR_JUEGO = 150000;
const int DINERO_POR_MOENDA = 1000;

const int MESA_VACIA = 0;
const int ES_MESA_VACIA = -1;

const int ERROR_ASIGNAR_MEMORIA = -1;

// Pre:
// Post: Inicializa todas las posiciones del terreno con un espacio vacio -> ' '
void inicializar_terreno(char terreno_resto[MAX_FILAS][MAX_COLUMNAS]){
    for(int i = 0; i < MAX_FILAS; i++){
        for(int j = 0; j < MAX_COLUMNAS; j++){
            terreno_resto[i][j] = POSICION_VACIA;
        }
    }
}

// Pre: El terreno debe estar previamente inicializado. Se representa mediante una matriz de MAX_FILAS x MAX_COLUMNAS.
// Post: Recorre la matriz del terreno e imprime todos sus elementos.
void imprimir_terreno(char terreno[MAX_FILAS][MAX_COLUMNAS]) {
    for(int i = 0; i < MAX_FILAS; i++) {
        printf("\n");
        for(int j = 0; j < MAX_COLUMNAS; j++) {
            printf("| %c |", terreno[i][j]);

        }
    }
}

// Pre: El terreno debe estar previamente inicializado en una matriz de MAX_FILAS y MAX_COLUMNAS.
// Post: Genera una coordenada random dentro de los limites (MAX_FILAS, MAX_COLUMNAS) de la matriz y en una
//    posición vacía representada por un caracter vacio. Luego, esa coordenada la devuelvo.
coordenada_t generar_coordenada_random(char terreno[MAX_FILAS][MAX_COLUMNAS]){
    coordenada_t coordenada;
    bool coordenada_invalida = true;
    while(coordenada_invalida) {
        int fila = rand() % (MAX_FILAS - 1) + 0;
        int columna = rand() % (MAX_COLUMNAS - 1) + 0;
        if((terreno[fila][columna] == POSICION_VACIA) && (fila < (MAX_FILAS - 1) && columna < (MAX_COLUMNAS - 1))) {
            coordenada.fil = fila;
            coordenada.col = columna;
            coordenada_invalida = false;
        }
    }
    return coordenada;
}

// Pre:
// Post: Calcula la distancia entre 2 coordenadas y la devuelve.
int distancia_a_mesa(coordenada_t primer_coordenada, coordenada_t segunda_coordenada){
    double distancia = 0;
    distancia = sqrt(pow(primer_coordenada.fil - segunda_coordenada.fil, 2) + pow(primer_coordenada.col - segunda_coordenada.col, 2));
    return (int)distancia;
}

// Pre: Las coordenadas que llegan por parametro deben estar inicializadas.
// Post: Retorna True si las coordenadas se encuentran en la misma posición. Caso contrario, retorna False.
bool misma_coordenada(coordenada_t primer_coordenada, coordenada_t segunda_coordenada){
    return primer_coordenada.fil == segunda_coordenada.fil && primer_coordenada.col == segunda_coordenada.col;
}

// Pre:
// Post: En caso de que no haya mesas en el juego, retornará que la distancia de la mesa nueva es valida.
//      Pero si hay, deberá comparar las coordenadas de cada una de las mesas que esten disponibles con cada coordenada de la mesa nueva,
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


// Pre: La coordenada que viene por parametro debe ser una coordenada valida (su fila y columna deben estar entre 0 y 20)
//      y haberse generado de manera random.
// Post: Inicializa las propiedades de la mesa compartida (comensales, paciencia, pedido tomado y la cantidad de lugares),
//      y ademas le asigna la coordenada que viene por parametro. Luego, esta mesa es devuelta.
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
    mesa_creada.cantidad_comensales = 0;
    mesa_creada.paciencia = 0;
    mesa_creada.pedido_tomado = false;
    return mesa_creada;
}


// Pre: La coordenada que viene por parametro debe ser una coordenada valida (su fila y columna deben estar entre 0 y 20)
//      y haberse generado de manera random.
// Post: Inicializa las propiedades de la mesa individual (comensales, paciencia, pedido tomado y la cantidad de lugares),
//      y ademas le asigna la coordenada que viene por parametro. Luego, esta mesa es devuelta.
mesa_t crear_mesa_individual(coordenada_t coordenada){
    mesa_t mesa_nueva;
    mesa_nueva.cantidad_lugares = 0;
    mesa_nueva.cantidad_comensales = MIN_COMENSALES;
    mesa_nueva.posicion[mesa_nueva.cantidad_lugares] = coordenada;
    mesa_nueva.cantidad_lugares += 1;
    mesa_nueva.cantidad_comensales = 0;
    mesa_nueva.paciencia = 0;
    mesa_nueva.pedido_tomado = false;
    return mesa_nueva;
}


// Pre: La mesa y el terreno (matriz de 20x20) deben estar previamente inicializados.
// Post: Obtengo una mesa por parametro (ya sea compartida o individual) y la asigna al terreno. Es decir,
//      en la coordenada/s correspondiente de la mesa, dibuja una 'T' en el terreno en representación de una mesa.
void asignar_mesa(mesa_t mesa, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    for(int i = 0; i < mesa.cantidad_lugares; i++) {
        terreno[mesa.posicion[i].fil][mesa.posicion[i].col] = MESA;
    }
}

// Pre: Es necesario que las mesas, su cantidad (mayor o igual a 0) y el terreno (matriz de 20x20) esten inicializados.
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
// Post: Crea una coordenada random valida y se la asigna al personaje.
void inicializar_linguini(juego_t* juego,char terreno[MAX_FILAS][MAX_COLUMNAS]){
    coordenada_t coordenada_linguini = generar_coordenada_random(terreno);
    juego->mozo.posicion = coordenada_linguini;
    juego->mozo.tiene_mopa = false;
    juego->mozo.patines_puestos = 0;
    juego->mozo.cantidad_patines = 0;
    juego->mozo.cantidad_bandeja = 0;
    juego->mozo.cantidad_pedidos = 0;
    for(int i = 0; i < MAX_PEDIDOS; i++){
        juego->mozo.pedidos[i].cantidad_platos = 0;
    }
    terreno[coordenada_linguini.fil][coordenada_linguini.col] = PERSONAJE;
}

// Pre: El juego y el terreno deben estar previamente inicializados.
// Post: Crea una coordenada random valida y se la asigna a la cocina.
void inicializar_cocina(juego_t* juego, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    coordenada_t coordenada = generar_coordenada_random(terreno);
    cocina_t cocina;
    cocina.posicion = coordenada;
    juego->cocina = cocina;
    juego->cocina.cantidad_listos = 0;
    juego->cocina.cantidad_preparacion = 0;
    juego->cocina.platos_listos = NULL;
    juego->cocina.platos_preparacion = NULL;
    terreno[cocina.posicion.fil][cocina.posicion.col] = COCINA;
}


// Pre: Debe llegar por parametro un tipo de objeto valido (mopa 'O', moneda 'M', patines 'P') y un terreno inicializado.
// Post: Genera una coordenada random y se la asigna al objeto. Luego, ese objeto lo agrega al array de herramientas.
void inicializar_objeto(char tipo_objeto, char terreno[MAX_FILAS][MAX_COLUMNAS], juego_t* juego){
    coordenada_t coordenada_objeto = generar_coordenada_random(terreno);
    objeto_t nuevo_objeto;
    nuevo_objeto.posicion = coordenada_objeto;
    nuevo_objeto.tipo = tipo_objeto;
    terreno[nuevo_objeto.posicion.fil][nuevo_objeto.posicion.col] = nuevo_objeto.tipo;
    juego->herramientas[juego->cantidad_herramientas] = nuevo_objeto;
    juego->cantidad_herramientas += 1;
}

// Pre:
// Post: Verifica que la coordenada este dentro del limite (MAX_FILAS, MAX_COLUMNAS) del terreno.
//      En caso correcto, retorna true. Caso contrario, false.
bool esta_dentro_limite(coordenada_t coordenada){
    return((coordenada.fil < (MAX_FILAS) && coordenada.col < (MAX_COLUMNAS )) && (coordenada.fil >= 0 && coordenada.col >= 0));
}

// Pre: Debe llegar por parametro un obstaculo valido (charco 'H' unicamente, de momento) y un terreno inicializado.
// Post: Crea un obstaculo dependiendo de su tipo y lo agrega al array de obstaculos.
void inicializar_obstaculo(char tipo_obstaculo, char terreno[MAX_FILAS][MAX_COLUMNAS], juego_t* juego){
    coordenada_t coordenada_obstaculo = generar_coordenada_random(terreno);
    objeto_t nuevo_obstaculo;
    nuevo_obstaculo.tipo = tipo_obstaculo;
    nuevo_obstaculo.posicion = coordenada_obstaculo;
    juego->obstaculos[juego->cantidad_obstaculos] = nuevo_obstaculo;
    juego->cantidad_obstaculos += 1;
}




// Pre: El juego debe estar inicializado, particularmente el vector de herramientas con una longitud de MAX_HERRAMIENTAS
// Post: Retorna el indice de la mopa en caso de encontrarlo, si no, retorna -1.
int buscar_indice_mopa(juego_t* juego){
    int indice_mopa = -1;
    for(int i = 0; i < juego->cantidad_herramientas; i++){
        if(juego->herramientas[i].tipo == MOPA){
            indice_mopa = i;
        }
    }
    return indice_mopa;
}

// Pre: Todos los elementos del juego y el terreno deben estar previamente inicializados.
// Post: Todas las posiciones de los objetos del juego, serán posicionadas en el terreno que viene por parametro.
void posicionar_elementos_terreno(juego_t* juego, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    for(int i = 0; i < juego->cantidad_mesas; i++){
        for(int p = 0; p < juego->mesas[i].cantidad_lugares; p++){
            if(esta_dentro_limite(juego->mesas[i].posicion[p])){
                if(p < juego->mesas[i].cantidad_comensales){
                    terreno[juego->mesas[i].posicion[p].fil][juego->mesas[i].posicion[p].col] = MESA_OCUPADA;
                } else {
                    terreno[juego->mesas[i].posicion[p].fil][juego->mesas[i].posicion[p].col] = MESA;
                }
            }
        }
    }
    if(esta_dentro_limite(juego->cocina.posicion)){
        terreno[juego->cocina.posicion.fil][juego->cocina.posicion.col] = COCINA;
    }
    objeto_t mopa = juego->herramientas[buscar_indice_mopa(juego)];
    bool mozo_tiene_mopa = juego->mozo.tiene_mopa;

    if(!mozo_tiene_mopa && esta_dentro_limite(mopa.posicion)) {
        terreno[mopa.posicion.fil][mopa.posicion.col] = MOPA;
    }

    for(int i = 0; i < juego->cantidad_herramientas; i++){
        if(esta_dentro_limite(juego->herramientas[i].posicion)){
            terreno[juego->herramientas[i].posicion.fil][juego->herramientas[i].posicion.col] = juego->herramientas[i].tipo;
        }
    }

    for(int j = 0; j < juego->cantidad_obstaculos; j++) {
        if(esta_dentro_limite(juego->obstaculos[j].posicion)){
            terreno[juego->obstaculos[j].posicion.fil][juego->obstaculos[j].posicion.col] = juego->obstaculos[j].tipo;
        }
    }

    if(esta_dentro_limite(juego->mozo.posicion)){
        terreno[juego->mozo.posicion.fil][juego->mozo.posicion.col] = PERSONAJE;
    }
}

// falta chequear si esta bien.
void disminuir_paciencia_mesas(juego_t* juego){
    bool hay_comensales, es_cucaracha, distancia_cucaracha_a_mesa, hay_cucaracha_cerca;
    for(int i = 0; i < juego->cantidad_mesas; i++){
        for(int j = 0; j < juego->mesas[i].cantidad_lugares; j++){
            hay_comensales = juego->mesas[i].cantidad_comensales > 0;
            hay_cucaracha_cerca = false;
            for(int c = 0; c < juego->cantidad_obstaculos; c++){
                es_cucaracha = juego->obstaculos[c].tipo == CUCARACHA;
                distancia_cucaracha_a_mesa = distancia_a_mesa(juego->mesas[i].posicion[j], juego->obstaculos[c].posicion) <= 2;
                if(hay_comensales && es_cucaracha && distancia_cucaracha_a_mesa){
                    hay_cucaracha_cerca = true;
                }
            }
        }
        if(hay_comensales) {
            if(hay_cucaracha_cerca){
                juego->mesas[i].paciencia -= 3;
            } else {
                juego->mesas[i].paciencia -= 1;
            }
        }
    }
}

void eliminar_patin(juego_t* juego, int indice){
    juego->herramientas[indice] = juego->herramientas[juego->cantidad_herramientas - 1];
    juego->cantidad_herramientas -= 1;
}

void recolectar_patines(juego_t* juego){
    coordenada_t* coordenada_linguini = &juego->mozo.posicion;
    bool es_patin, tienen_misma_coordenada;
    for(int i = 0; i < juego->cantidad_herramientas; i++){
        es_patin = juego->herramientas[i].tipo == PATINES;
        tienen_misma_coordenada = misma_coordenada(*coordenada_linguini, juego->herramientas[i].posicion);
        if(es_patin && tienen_misma_coordenada){
            juego->mozo.cantidad_patines += 1;
            eliminar_patin(juego, i);
        }
    }
}

void eliminar_moneda(juego_t* juego, int indice){
    juego->herramientas[indice] = juego->herramientas[juego->cantidad_herramientas - 1];
    juego->cantidad_herramientas -= 1;
}

void recolectar_moneda(juego_t* juego){
    coordenada_t* coordenada_linguini = &juego->mozo.posicion;
    bool misma_fila, misma_columna, es_moneda;
    for(int i = 0; i < juego->cantidad_herramientas; i++){
        es_moneda = juego->herramientas[i].tipo == MONEDA;
        misma_fila = coordenada_linguini->fil == juego->herramientas[i].posicion.fil;
        misma_columna = coordenada_linguini->col == juego->herramientas[i].posicion.col;
        if(es_moneda && misma_fila && misma_columna){
            juego->dinero += DINERO_POR_MOENDA;
            eliminar_moneda(juego, i);
        }
    }
}

void eliminar_cucarachas(juego_t* juego){
    coordenada_t coordenada_linguini = juego->mozo.posicion;
    bool es_cucaracha, tienen_misma_coordenada;
    for(int i = 0; i < juego->cantidad_obstaculos; i++){
        es_cucaracha = juego->obstaculos[i].tipo == CUCARACHA;
        tienen_misma_coordenada = misma_coordenada(coordenada_linguini, juego->obstaculos[i].posicion);
        if(es_cucaracha && tienen_misma_coordenada){
            juego->obstaculos[i] = juego->obstaculos[juego->cantidad_obstaculos - 1];
            juego->cantidad_obstaculos --;
        }
    }
}

void liberar_mesa(juego_t* juego, int indice_mesa){
    juego->mesas[indice_mesa].cantidad_comensales = 0;
    juego->mesas[indice_mesa].paciencia = 0;
    juego->mesas[indice_mesa].pedido_tomado = false;
    juego->mozo.cantidad_bandeja = 0;
}

void eliminar_bandeja(juego_t* juego){
    bool es_charco, tienen_misma_coordenada;
    for(int i = 0; i < juego->cantidad_obstaculos; i++){
        es_charco = juego->obstaculos[i].tipo == CHARCOS;
        tienen_misma_coordenada = misma_coordenada(juego->obstaculos[i].posicion, juego->mozo.posicion);
        if(es_charco && tienen_misma_coordenada){
            for(int j = 0; j < juego->mozo.cantidad_bandeja; j++){
                liberar_mesa(juego, juego->mozo.bandeja[j].id_mesa);
            }
        }
    }
}

// Pre: El juego y el terreno deben estar previamente inicializados, al igual que la nueva coordenada que tomará el personaje.
// Post: Verificará que la posición nueva del personaje sea valida (no puede subirse a una mesa ni estar por fuera de los limites).
//      En caso de que sea valida, entonces le asignará esa nueva coordenada al personaje y aumentará la cantidad de movimientos realizados.
void mover_linguini(char terreno[MAX_FILAS][MAX_COLUMNAS], juego_t* juego, coordenada_t nueva_coordenada){
    bool dentro_limite = esta_dentro_limite(nueva_coordenada);
    if(dentro_limite == true){
        char posicion_nueva = terreno[nueva_coordenada.fil][nueva_coordenada.col];
        bool es_mesa = posicion_nueva == MESA;
        bool es_mesa_ocupada = posicion_nueva == MESA_OCUPADA;
        bool mozo_tiene_mopa = juego->mozo.tiene_mopa;
        if((!es_mesa && !es_mesa_ocupada) && dentro_limite) {
            juego->mozo.posicion = nueva_coordenada;
            if(!juego->mozo.patines_puestos){
                juego->movimientos += 1;
                disminuir_paciencia_mesas(juego);
            }
            if(!mozo_tiene_mopa){
                recolectar_moneda(juego);
                recolectar_patines(juego);
                eliminar_cucarachas(juego);
                eliminar_bandeja(juego);
            }
        }
    }

}

// Pre: La jugada que ingresa por parametro debe ser valida (W,A,S,D) y el juego debe estar inicializado, especialmente la posición del mozo.
// Post: A partir de la jugada que viene por parametro y de la posición actual del mozo, se genererá la nueva posición del mozo y se retornará.
//          Sea 'i' la fila y 'j' la columna en la que se encuentra el mozo:
//          W -> Se mueve hacia arriba (se mantiene en la misma columna, sube una fila) -> j se mantiene, i - 1
//          S -> Se mueve hacia abajo  (se mantiene en la misma columna, baja una fila) -> j se mantiene, i + 1
//          D -> Se mueve hacia la derecha   (se mantiene en la misma fila, avanza una columna)    -> i se mantiene, j + 1
//          A -> Se mueve hacia la izquierda (se mantiene en la misma fila, retrocede una columna) -> i se mantiene, j - 1
coordenada_t nueva_cordenada_linguini(char jugada, juego_t* juego){
    coordenada_t coordenada_actual = juego->mozo.posicion;
    coordenada_t coordenada_nueva = {-1, -1};
    if(jugada == MOVER_ARRIBA){
        coordenada_nueva.fil = coordenada_actual.fil - 1;
        coordenada_nueva.col = coordenada_actual.col;
    } else if(jugada == MOVER_DER) {
        coordenada_nueva.fil = coordenada_actual.fil;
        coordenada_nueva.col = coordenada_actual.col + 1;
    } else if (jugada == MOVER_ABAJO) {
        coordenada_nueva.fil = coordenada_actual.fil + 1;
        coordenada_nueva.col = coordenada_actual.col;
    } else if(jugada == MOVER_IZQ) {
        coordenada_nueva.fil = coordenada_actual.fil;
        coordenada_nueva.col = coordenada_actual.col - 1;
    }
    return coordenada_nueva;
}


// Pre: El juego debe estar inicializado ya que se trabajaran con los topes las herramientas y obstaculos.
// Post: Verifica que la coordenada (del personaje) que viene por parametro, tenga la misma posición que
//      las herramientas (mopa, moneda o patines) o los obstaculos (charcos).
//      En caso de que sea la misma, entonces retornará true. En caso contrario, retornará falso.
bool es_posicion_ocupada(juego_t* juego, coordenada_t coordenada){
    bool es_ocupada = false;
    int i = 0;

    while(i < juego->cantidad_herramientas){
        if(misma_coordenada(juego->herramientas[i].posicion, coordenada)){
            es_ocupada = true;
        }
        i++;
    }

    int j = 0;
    while(j < juego->cantidad_obstaculos){
        if(misma_coordenada(juego->obstaculos[j].posicion, coordenada)) {
            es_ocupada = true;
        }
        j++;
    }
    return es_ocupada;
}

bool linguini_tiene_mopa(juego_t* juego){
    return juego->mozo.tiene_mopa;
}

// -> Falta agregar las pre y post.
void eliminar_mopa(juego_t* juego){
    int indice_mopa = buscar_indice_mopa(juego);
    if(indice_mopa != -1){
        juego->herramientas[indice_mopa] = juego->herramientas[juego->cantidad_herramientas - 1];
        juego->cantidad_herramientas--;
    }
}

// -> Falta agregar las pre y post.
void agregar_mopa(juego_t* juego){
    objeto_t mopa;
    mopa.tipo = MOPA;
    mopa.posicion = juego->mozo.posicion;
    juego->herramientas[juego->cantidad_herramientas] = mopa;
    juego->cantidad_herramientas++;
}

// Pre: El terreno y el juego deben estar previamente inicializados, y con todos los objetos correspondientes posicionados en el terreno.
// Post: En caso de que el personaje este en la misma posición que la mopa, entonces tomará la mopa y se actualizará
//      la posición de la mopa (fuera de rango), simulando que la tiene Linguini.
//      En caso de que ya tenga la mopa, entonces dejará la mopa en la posición en la que se encuentre (con sus reestricciones),
//      y se actualizará la posición de la mopa en donde la dejo el personaje.
void accion_de_mopa(char terreno[MAX_FILAS][MAX_COLUMNAS], juego_t* juego){
    bool misma_coordenada_mopa, tiene_mopa, misma_coordenada_cocina, espacio_vacio, misma_coordenada_herramientas;
    int indice_mopa = buscar_indice_mopa(juego);
    coordenada_t* coordenada_mopa = &(juego->herramientas[indice_mopa].posicion);
    coordenada_t* coordenada_personaje = &(juego->mozo.posicion);
    coordenada_t* coordenada_cocina = &(juego->cocina.posicion);
    misma_coordenada_mopa = misma_coordenada(*coordenada_personaje, *coordenada_mopa);
    misma_coordenada_cocina = misma_coordenada(*coordenada_personaje, *coordenada_cocina);
    tiene_mopa = juego->mozo.tiene_mopa == true;
    espacio_vacio = terreno[coordenada_personaje->fil][coordenada_personaje->col] == PERSONAJE;
    misma_coordenada_herramientas = es_posicion_ocupada(juego, *coordenada_personaje);
    if(misma_coordenada_mopa && !tiene_mopa) {
        eliminar_mopa(juego);
        juego->mozo.tiene_mopa = true;
    } else if (tiene_mopa && espacio_vacio && !misma_coordenada_cocina && !misma_coordenada_herramientas){
        agregar_mopa(juego);
        juego->mozo.tiene_mopa = false;
    }
}

// -> Falta agregar los pre y post.
int encontrar_mesa_vacia(juego_t* juego, int lugares){
    int indice_mesa_vacia = -1;
    int i = 0;
    bool mesa_encontrada = false;
    while( i < juego->cantidad_mesas && !mesa_encontrada){
        if(juego->mesas[i].cantidad_comensales == MESA_VACIA && juego->mesas[i].cantidad_lugares == lugares){
            indice_mesa_vacia = i;
            mesa_encontrada = true;
        }
        i++;
    }
    return indice_mesa_vacia;
}

// -> Falta agregar los pre y post.
void asignar_comensales(juego_t* juego){
    int cantidad_comensales = rand() % 4 + 1;
    int indice_mesa_vacia;
    if(cantidad_comensales == MIN_COMENSALES){
        indice_mesa_vacia = encontrar_mesa_vacia(juego, MIN_COMENSALES);
        if(indice_mesa_vacia != ES_MESA_VACIA){
            mesa_t* mesa = &juego->mesas[indice_mesa_vacia];
            mesa->cantidad_comensales = MIN_COMENSALES;
            mesa->paciencia = rand() % 100 + 100;
        } else {
            indice_mesa_vacia = encontrar_mesa_vacia(juego, MAX_COMENSALES);
            if(indice_mesa_vacia != ES_MESA_VACIA){
                mesa_t* mesa = &juego->mesas[indice_mesa_vacia];
                mesa->cantidad_comensales = MIN_COMENSALES;
                mesa->paciencia = rand() % 100 + 100;
            }
        }
    } else {
        indice_mesa_vacia = encontrar_mesa_vacia(juego, MAX_COMENSALES);
        if(indice_mesa_vacia != ES_MESA_VACIA){
            mesa_t* mesa = &juego->mesas[indice_mesa_vacia];
            mesa->cantidad_comensales = cantidad_comensales;
            mesa->paciencia = rand() % 100 + 100;
        }
    }
}

void liberar_pedido_de_preparacion(juego_t* juego, mesa_t mesa){
    for(int i = 0; i < juego->cocina.cantidad_preparacion; i++){
        for(int j = 0; j < juego->cantidad_mesas; j++){
            if(juego->mesas[j].pedido_tomado && juego->cocina.platos_preparacion[i].id_mesa == j){
                juego->cocina.platos_preparacion[i] = juego->cocina.platos_preparacion[juego->cocina.cantidad_preparacion - 1];
                juego->cocina.cantidad_preparacion -= 1;
            }
        }
    }
}

void desocupar_mesa(juego_t* juego){
    int i = 0;
    while(i < juego->cantidad_mesas) {
        if(juego->mesas[i].paciencia == 0 && juego->mesas[i].cantidad_comensales > 0){
            juego->mesas[i].cantidad_comensales = MESA_VACIA;
            juego->mesas[i].pedido_tomado = false;
            liberar_pedido_de_preparacion(juego, juego->mesas[i]);
            if(juego->mozo.cantidad_pedidos > 0){
               juego->mozo.cantidad_pedidos --;
            }
        }
        i++;
    }
}

void calcular_tiempo_preparacion(int* tiempo_actual, int* tiempo_mayor){
    if(*tiempo_actual > *tiempo_mayor){
        *tiempo_mayor = *tiempo_actual;
    }
}

void asignar_pedido(juego_t* juego, mesa_t* mesa, int indice_mesa){
    pedido_t pedido;
    pedido.cantidad_platos = 0;
    pedido.id_mesa = indice_mesa;
    int tiempo_preparacion_actual = 0;
    int tiempo_preparacion_mayor = 0;
    for(int i = 0; i < mesa->cantidad_comensales; i++){
        int generar_pedido = rand() % 4 + 1;
        if(generar_pedido == ID_MESA_NAPOLITANA){
            pedido.platos[pedido.cantidad_platos] = MILANESA_NAPOLITANA;
            tiempo_preparacion_actual = TIEMPO_MILANESA_NAPOLITANA;
            calcular_tiempo_preparacion(&tiempo_preparacion_actual, &tiempo_preparacion_mayor);
        } else if(generar_pedido == ID_HAMBURGUESA){
            pedido.platos[pedido.cantidad_platos] = HAMBURGUESA;
            tiempo_preparacion_actual = TIEMPO_HAMBURGUESA;
            calcular_tiempo_preparacion(&tiempo_preparacion_actual, &tiempo_preparacion_mayor);
        } else if(generar_pedido == ID_PARRILLA){
            pedido.platos[pedido.cantidad_platos] = PARRILLA;
            tiempo_preparacion_actual = TIEMPO_PARRILLA;
            calcular_tiempo_preparacion(&tiempo_preparacion_actual, &tiempo_preparacion_mayor);
        } else {
            pedido.platos[pedido.cantidad_platos] = RATATOUILLE;
            tiempo_preparacion_actual = TIEMPO_RATATOUILLE;
            calcular_tiempo_preparacion(&tiempo_preparacion_actual, &tiempo_preparacion_mayor);
        }
        pedido.cantidad_platos += 1;
    }
    pedido.tiempo_preparacion = tiempo_preparacion_mayor;
    juego->mozo.pedidos[juego->mozo.cantidad_pedidos] = pedido;
    juego->mozo.cantidad_pedidos += 1;
}

// completar bien los pre y post.
void tomar_pedidos_mesas(juego_t* juego){
    int distancia;
    int i = 0;
    int j = 0;
    while(i < juego->cantidad_mesas){
        while(j < juego->mesas[i].cantidad_lugares){
            distancia = distancia_a_mesa(juego->mozo.posicion, juego->mesas[i].posicion[j]);
            bool hay_comensales_en_mesa = juego->mesas[i].cantidad_comensales > 0;
            bool tiene_espacio_en_bandeja = juego->mozo.cantidad_pedidos < MAX_PEDIDOS;
            bool mesa_tiene_pedido = juego->mesas[i].pedido_tomado;
            if(distancia == 1 && tiene_espacio_en_bandeja && hay_comensales_en_mesa && !mesa_tiene_pedido){
                asignar_pedido(juego, &juego->mesas[i], i);
                juego->mesas[i].pedido_tomado = true;
            }
            j++;
        }
        j = 0;
        i++;
    }
}

void eliminar_charco(juego_t* juego, int indice){
    juego->obstaculos[indice] = juego->obstaculos[juego->cantidad_obstaculos - 1];
    juego->cantidad_obstaculos --;
}

void limpiar_charco(juego_t* juego){
    int i = 0;
    bool es_charco, tienen_misma_coordenada, tiene_mopa;

    while(i < juego->cantidad_obstaculos){
        es_charco = juego->obstaculos[i].tipo == CHARCOS;
        tienen_misma_coordenada = misma_coordenada(juego->cocina.posicion, juego->mozo.posicion);
        tiene_mopa = juego->mozo.tiene_mopa;
        if(es_charco && tienen_misma_coordenada && tiene_mopa){
            eliminar_charco(juego, i);
        }
        i++;
    }
}

bool puede_moverse_con_patines(char terreno[MAX_FILAS][MAX_COLUMNAS], coordenada_t nueva_coordenada){
    return terreno[nueva_coordenada.fil][nueva_coordenada.col] != MESA && terreno[nueva_coordenada.fil][nueva_coordenada.col] != MESA_OCUPADA;
}

void utilizar_patines(juego_t* juego, char jugada, char terreno[MAX_FILAS][MAX_COLUMNAS]) {
    coordenada_t nueva_coordenada = juego->mozo.posicion;
    bool flag = true;
    while(flag){
        if(jugada == MOVER_ARRIBA){
            nueva_coordenada.fil -= 1;
        } else if(jugada == MOVER_DER){
            nueva_coordenada.col += 1;
        } else if(jugada == MOVER_ABAJO){
            nueva_coordenada.fil += 1;
        } else {
            nueva_coordenada.col -= 1;
        }
        if(!esta_dentro_limite(nueva_coordenada) || !puede_moverse_con_patines(terreno, nueva_coordenada)){
            flag = false;
        } else {
            mover_linguini(terreno, juego, nueva_coordenada);
        }
    }
    juego->mozo.patines_puestos = false;
    juego->mozo.cantidad_patines -= 1;
    juego->movimientos += 1;
}



void asignar_pedidos_a_cocina(juego_t* juego){
    coordenada_t coordenada_personaje = juego->mozo.posicion;
    coordenada_t coordenada_cocina = juego->cocina.posicion;
    bool misma_coordenada_cocina = misma_coordenada(coordenada_personaje, coordenada_cocina);
    bool cantidad_pedidos = juego->mozo.cantidad_pedidos;
    bool linguini_tiene_mopa = juego->mozo.tiene_mopa == true;
    if(misma_coordenada_cocina && cantidad_pedidos > 0 && !linguini_tiene_mopa){
        juego->cocina.platos_preparacion = realloc(juego->cocina.platos_preparacion, sizeof(pedido_t) * (size_t)(juego->cocina.cantidad_preparacion + juego->mozo.cantidad_pedidos));
        if(juego->cocina.platos_preparacion == NULL){
            free(juego->cocina.platos_preparacion);
            return;
        }
        for(int i = 0; i < juego->mozo.cantidad_pedidos; i++){
            juego->cocina.platos_preparacion[juego->cocina.cantidad_preparacion] = juego->mozo.pedidos[i];
            juego->cocina.cantidad_preparacion += 1;
        }
        juego->mozo.cantidad_pedidos = 0;
    }
}

void asignar_pedidos_a_listo(juego_t* juego){
    coordenada_t coordenada_cocina = juego->cocina.posicion;
    coordenada_t coordenada_linguini = juego->mozo.posicion;
    bool misma_coordenada_cocina = misma_coordenada(coordenada_cocina, coordenada_linguini);
    if(juego->cocina.cantidad_preparacion > 0 && misma_coordenada_cocina){
        juego->cocina.platos_listos = realloc(juego->cocina.platos_listos, sizeof(juego->cocina.cantidad_listos + juego->cocina.cantidad_preparacion));
        if(juego->cocina.platos_listos == NULL){
            free(juego->cocina.platos_preparacion);
            free(juego->cocina.platos_listos);
            return;
        }
        for(int i = 0; i < juego->cocina.cantidad_preparacion; i++){
            for(int j = 0; j < juego->cocina.platos_preparacion[i].cantidad_platos; j++){
                juego->cocina.platos_listos[juego->cocina.cantidad_listos].platos[juego->cocina.platos_listos[juego->cocina.cantidad_listos].cantidad_platos] = juego->cocina.platos_preparacion[i].platos[j];
                juego->cocina.platos_listos += 1;
            }
        }
        juego->cocina.platos_preparacion = 0;
    }
}

void destruir_juego(juego_t* juego) {
    if(juego->cocina.platos_listos == NULL){
        return;
    }
    free(juego->cocina.platos_listos);

    if(juego->cocina.platos_preparacion == NULL){
        return;
    }
    free(juego->cocina.platos_preparacion);
}


// Pre:
// Post: Simulando un terreno representado por la matriz de MAX_FILAS x MAX_COLUMNAS, se inicializaran todos los objetos del juego.
void inicializar_juego(juego_t* juego){
    char terreno[MAX_FILAS][MAX_COLUMNAS];
    inicializar_terreno(terreno);
    juego->cantidad_mesas = 0;
    juego->cantidad_herramientas = 0;
    juego->dinero = 0;
    juego->movimientos = 0;
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

// Pre: El juego que viene por parametro debe estar previamente inicializado y la acción a realizar,
//      debe ser valida (W,A,S,D o accion de mopa).
// Post: Verifica que tipo de acción es y en cada caso particular, llamará a una funcion encargada de ejecutar la jugada.
void realizar_jugada(juego_t* juego, char accion){
    char terreno[MAX_FILAS][MAX_COLUMNAS];
    inicializar_terreno(terreno);
    posicionar_elementos_terreno(juego, terreno);

    if(accion == ACCION_MOPA) {
        accion_de_mopa(terreno, juego);
    } else if(accion == TOMAR_PEDIDO && !linguini_tiene_mopa(juego)) {
        tomar_pedidos_mesas(juego);
    } else if(accion == ACTIVAR_PATINES && juego->mozo.cantidad_patines > 0 && !juego->mozo.tiene_mopa){
        juego->mozo.patines_puestos = true;
    } else if(juego->mozo.patines_puestos) {
        utilizar_patines(juego, accion, terreno);
    } else {
        mover_linguini(terreno, juego, nueva_cordenada_linguini(accion, juego));
        if(linguini_tiene_mopa(juego)){
            limpiar_charco(juego);
        }
    }

    if(juego->movimientos % 15 == 0) {
        asignar_comensales(juego);
    }

    if(juego->movimientos >= 25 && juego->movimientos % 25 == 0){
        inicializar_obstaculo(CUCARACHA, terreno, juego);
    }

    desocupar_mesa(juego);
    asignar_pedidos_a_cocina(juego);
    // asignar_pedidos_a_preparar(juego);
}

// Pre: El juego que viene por parametro debe estar previamente inicializado.
// Post: Se creará un terreno y se le posicionaran todos los objetos del juego. Luego, se imprime por pantalla el terreno,
//         los movimientos realizados, cantidad de pedidos y el dinero total.
void mostrar_juego(juego_t juego) {
    char terreno[MAX_FILAS][MAX_COLUMNAS];
    inicializar_terreno(terreno);
    posicionar_elementos_terreno(&juego, terreno);
    system("clear");

    imprimir_terreno(terreno);

    printf("\n");
    printf("Cantidad obstaculos: %i\n", juego.cantidad_obstaculos);
    printf("Cantidad de movimientos: %i de %i\n", juego.movimientos, MAX_MOVIMIENTOS);
    printf("Cantidad Pedidos: %i\n", juego.mozo.cantidad_pedidos);
    printf("Cantidad Patines: %i\n", juego.mozo.cantidad_patines);
    printf("Cantidad en Bandeja: %i\n", juego.mozo.cantidad_bandeja);
    printf("Cantidad de Platos en Preparación: %i\n", juego.cocina.cantidad_preparacion);
    printf("Cantidad de Platos Listos: %i\n", juego.cocina.cantidad_listos);
    if(juego.mozo.patines_puestos){
        printf("El mozo tiene los patines puestos.\n");
    } else {
        printf("El mozo NO tiene los patines puestos.\n");
    }
    printf("Dinero: %i\n", juego.dinero);
}

// Pre: El juego que viene por parametro debe estar previamente inicializado.
// Post: Dependiendo del dinero que junto el personaje, retornará JUEGO_GANADO si alcanzo el dinero suficiente.
//      Retornará JUEGO_PERDIDO si no lo hizo. Retornará SEGUIR_JUGANDO, si debe seguir jugando.
int estado_juego(juego_t juego){
    int estado = SEGUIR_JUGANDO;
    if(juego.movimientos == 200 && juego.dinero < MONEDAS_GANAR_JUEGO){
        estado = JUEGO_PERDIDO;
    } else if(juego.movimientos == 200 && juego.dinero >= MONEDAS_GANAR_JUEGO){
        estado = JUEGO_GANADO;
    }
    return estado;
}
