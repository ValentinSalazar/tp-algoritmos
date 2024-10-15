#include "restaurant.h"

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define MIN_COMENSALES 1

#define COCINA 'C'
#define MESA 'T'
#define PERSONAJE 'L'
#define MOPA 'O'
#define MONEDA 'M'
#define PATINES 'P'
#define CHARCOS 'H'


#define MOVER_ARRIBA 'W'
#define MOVER_ABAJO 'S'
#define MOVER_DER 'D'
#define MOVER_IZQ 'A'
#define ACCION_MOPA 'O'

#define JUEGO_GANADO 1
#define JUEGO_PERDIDO 1
#define SEGUIR_JUGANDO 0

#define POSICION_VACIA ' '


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
    coordenada_t coordenada = {-1, -1};
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


// Pre:
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
    juego->mozo.pedidos->cantidad_platos = 0;

    terreno[coordenada_linguini.fil][coordenada_linguini.col] = PERSONAJE;
}

// Pre: El juego y el terreno deben estar previamente inicializados.
// Post: Crea una coordenada random valida y se la asigna a la cocina.
void inicializar_cocina(juego_t* juego, char terreno[MAX_FILAS][MAX_COLUMNAS]){
    coordenada_t coordenada = generar_coordenada_random(terreno);

    cocina_t cocina;
    cocina.posicion = coordenada;
    juego->cocina = cocina;

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

// Pre: Debe llegar por parametro un obstaculo valido (charco 'H' unicamente, de momento) y un terreno inicializado.
// Post: Crea un obstaculo dependiendo de su tipo y lo agrega al array de obstaculos.
void inicializar_obstaculo(char tipo_obstaculo, char terreno[MAX_FILAS][MAX_COLUMNAS], juego_t* juego){
    coordenada_t coordenada_obstaculo = generar_coordenada_random(terreno);

    objeto_t nuevo_obstaculo;
    nuevo_obstaculo.tipo = tipo_obstaculo;
    nuevo_obstaculo.posicion = coordenada_obstaculo;

    terreno[nuevo_obstaculo.posicion.fil][nuevo_obstaculo.posicion.col] = nuevo_obstaculo.tipo;

    juego->obstaculos[juego->cantidad_obstaculos] = nuevo_obstaculo;
    juego->cantidad_obstaculos += 1;
}


// Pre: 
// Post: Verifica que la coordenada este dentro del limite (MAX_FILAS, MAX_COLUMNAS) del terreno. 
//      En caso correcto, retorna true. Caso contrario, false.
bool esta_dentro_limite(coordenada_t coordenada){
    return((coordenada.fil < (MAX_FILAS) && coordenada.col < (MAX_COLUMNAS )) && (coordenada.fil >= 0 && coordenada.col >= 0));
}

// Pre: Todos los elementos del juego y el terreno deben estar previamente inicializados.
// Post: Todas las posiciones de los objetos del juego, serán posicionadas en el terreno que viene por parametro.
void posicionar_elementos_terreno(juego_t* juego, char terreno[MAX_FILAS][MAX_COLUMNAS]){

    for(int i = 0; i < juego->cantidad_mesas; i++){
        for(int p = 0; p < juego->mesas[i].cantidad_lugares; p++){
            terreno[juego->mesas[i].posicion[p].fil][juego->mesas[i].posicion[p].col] = MESA;
        }
    }
    terreno[juego->cocina.posicion.fil][juego->cocina.posicion.col] = COCINA;

    for(int i = 0; i < juego->cantidad_herramientas; i++){
        terreno[juego->herramientas[i].posicion.fil][juego->herramientas[i].posicion.col] = juego->herramientas[i].tipo;
    }

    for(int j = 0; j < juego->cantidad_obstaculos; j++) {
        terreno[juego->obstaculos[j].posicion.fil][juego->obstaculos[j].posicion.col] = juego->obstaculos[j].tipo;
    }
    terreno[juego->mozo.posicion.fil][juego->mozo.posicion.col] = PERSONAJE;
}


// Pre: El juego y el terreno deben estar previamente inicializados, al igual que la nueva coordenada que tomará el personaje.
// Post: Verificará que la posición nueva del personaje sea valida (no puede subirse a una mesa ni estar por fuera de los limites).
//      En caso de que sea valida, entonces le asignará esa nueva coordenada al personaje y aumentará la cantidad de movimientos realizados.
void mover_linguini(char terreno[MAX_FILAS][MAX_COLUMNAS], juego_t* juego, coordenada_t nueva_coordenada){
    char posicion_nueva = terreno[nueva_coordenada.fil][nueva_coordenada.col];
    if((posicion_nueva != MESA) && esta_dentro_limite(nueva_coordenada)) {
        juego->mozo.posicion = nueva_coordenada;
        juego->movimientos += 1;
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
    } else {
        coordenada_nueva.fil = coordenada_actual.fil;
        coordenada_nueva.col = coordenada_actual.col - 1;
    }
    return coordenada_nueva;
}



// Pre: El terreno y el juego deben estar previamente inicializados, y con todos los objetos correspondientes posicionados en el terreno.
// Post: En caso de que el personaje este en la misma posición que la mopa, entonces tomará la mopa y se actualizará  
//      la posición de la mopa (fuera de rango), simulando que la tiene Linguini. 
//      En caso de que ya tenga la mopa, entonces dejará la mopa en la posición en la que se encuentre (con sus reestricciones), 
//      y se actualizará la posición de la mopa en donde la dejo el personaje.
void accion_de_mopa(char terreno[MAX_FILAS][MAX_COLUMNAS], juego_t* juego){

    coordenada_t* coordenada_mopa = &(juego->herramientas[0].posicion);

    coordenada_t* coordenada_personaje = &(juego->mozo.posicion);

    coordenada_t* coordenada_cocina = &(juego->cocina.posicion);

    bool misma_coordenada_mopa = coordenada_personaje->fil == coordenada_mopa->fil && coordenada_personaje->col == coordenada_mopa->col;
    bool tiene_mopa = juego->mozo.tiene_mopa == true;
    bool misma_coordenada_cocina = coordenada_personaje-> fil == coordenada_cocina->fil && coordenada_personaje->col == coordenada_cocina ->col;
    bool espacio_vacio = terreno[coordenada_personaje->fil][coordenada_personaje->col] == PERSONAJE;
    
    if(misma_coordenada_mopa && !tiene_mopa) {
        coordenada_mopa->fil = ESCONDER_MOPA;
        coordenada_mopa->col = ESCONDER_MOPA;
        terreno[coordenada_personaje->fil][coordenada_personaje->col] = 'L';
        juego->mozo.tiene_mopa = true;
    } else if (tiene_mopa && espacio_vacio && !misma_coordenada_cocina){
        terreno[juego->mozo.posicion.fil][juego->mozo.posicion.col] = MOPA;
        coordenada_mopa->fil = coordenada_personaje->fil;
        coordenada_mopa->col = coordenada_personaje->col;
        juego->mozo.tiene_mopa = false;
    }
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

// Pre: El juego que viene por parametro debe estar previamente inicializado y la acción a realizar, 
//      debe ser valida (W,A,S,D o accion de mopa).
// Post: Verifica que tipo de acción es y en cada caso particular, llamará a una funcion encargada de ejecutar la jugada.
void realizar_jugada(juego_t* juego, char accion){

    char terreno[MAX_FILAS][MAX_COLUMNAS];
    inicializar_terreno(terreno);
    posicionar_elementos_terreno(juego, terreno);

    if(accion == ACCION_MOPA) {
        accion_de_mopa(terreno, juego);
    } else {
        mover_linguini(terreno, juego, nueva_cordenada_linguini(accion, juego));
    }
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

    printf("Cantidad de movimientos: %i\n", juego.movimientos);
    printf("Cantidad Pedidos: %i\n", juego.mozo.cantidad_pedidos);
    printf("Dinero: %i\n", juego.dinero);
}


// Pre: El juego que viene por parametro debe estar previamente inicializado.
// Post: Dependiendo del dinero que junto el personaje, retornará JUEGO_GANADO si alcanzo el dinero suficiente. 
//      Retornará JUEGO_PERDIDO si no lo hizo. Retornará SEGUIR_JUGANDO, si debe seguir jugando.      
int estado_juego(juego_t juego){
    int dinero_acumulado = juego.dinero;
    int estado = SEGUIR_JUGANDO;
    if(dinero_acumulado >= MONEDAS_GANAR_JUEGO) {
        estado = JUEGO_GANADO;
    } else {
        estado = JUEGO_PERDIDO;
    }
    
    return estado;
}

