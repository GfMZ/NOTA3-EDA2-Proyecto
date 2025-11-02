#include "../include/TablaHash.h"
#include <iostream> 

TablaHash::TablaHash(int tam) {
    tamano = tam;
    ocupados = 0;
    tabla = new HashEntry[tamano];
    for (int i = 0; i < tamano; i++) {
        tabla[i] = HashEntry(); 
    }
}

TablaHash::~TablaHash() {
    delete[] tabla;
}

void TablaHash::insertar(long long key, int value) {
    if (ocupados >= tamano) return;

    int i = 0;
    int indice = funcionHash(key);

    while (tabla[indice].ocupado && tabla[indice].key != key) {
        i++;
        indice = (funcionHash(key) + i * i) % tamano;
    }

    if (!tabla[indice].ocupado) {
        tabla[indice].key = key;
        tabla[indice].value = value;
        tabla[indice].ocupado = true;
        ocupados++;
    }
}

int TablaHash::buscar(long long key) {
    int i = 0;
    int indice = funcionHash(key);

    while (tabla[indice].ocupado) {
        if (tabla[indice].key == key) {
            return tabla[indice].value; 
        }
        i++;
        indice = (funcionHash(key) + i * i) % tamano;

        if (i > tamano) break; 
    }
    
    return -1; 
}