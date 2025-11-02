#include "../include/Grafo.h"

Grafo::Grafo(int numNodos) {
    capacidad = numNodos;
    nodos = new NodoInfo[capacidad];
    adj = new Lista[capacidad];
}

Grafo::~Grafo() {
    delete[] nodos;
    delete[] adj;
}

void Grafo::agregarNodo(int id, double lat, double lon) {
    if (id >= 0 && id < capacidad) {
        nodos[id].idInterno = id;
        nodos[id].lat = lat;
        nodos[id].lon = lon;
    }
}

void Grafo::agregarArista(int origen, int destino, double peso) {
    if (origen >= 0 && origen < capacidad && destino >= 0 && destino < capacidad) {
        Arista nuevaArista = {destino, peso};
        adj[origen].insertar(nuevaArista);
        
        // Si el grafo es NO-DIRIGIDO (las calles son de doble sentido)
        // añade también la arista en la otra dirección.
        Arista aristaVuelta = {origen, peso};
        adj[destino].insertar(aristaVuelta);
    }
}

NodoInfo Grafo::getNodoInfo(int id) {
    if (id >= 0 && id < capacidad) {
        return nodos[id];
    }
    // Devolver un "default" o manejar error si es necesario
    return {-1, 0, 0}; 
}

Lista& Grafo::getListaAdyacencia(int id) {
    return adj[id];
}