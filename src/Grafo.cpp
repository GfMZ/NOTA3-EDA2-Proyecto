#include "../include/Grafo.h"

Grafo::Grafo(int numNodos) {
    capacidad = numNodos;
    nodos = new NodoInfo[capacidad];
}

Grafo::~Grafo() {
    delete[] nodos;
}

void Grafo::agregarNodo(int id, double lat, double lon) {
    if (id >= 0 && id < capacidad) {
        nodos[id].idInterno = id;
        nodos[id].lat = lat;
        nodos[id].lon = lon;
    }
}