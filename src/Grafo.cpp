#include "../include/Grafo.h"
#include "../include/MinHeap.h"
#include <limits>   
#include <iostream> 

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
        
        Arista aristaVuelta = {origen, peso};
        adj[destino].insertar(aristaVuelta);
    }
}

NodoInfo Grafo::getNodoInfo(int id) {
    if (id >= 0 && id < capacidad) {
        return nodos[id];
    }

    return {-1, 0, 0}; 
}

Lista& Grafo::getListaAdyacencia(int id) {
    return adj[id];
}

void imprimirRutaRecursivo(int actual, int* predecesor) {
    if (predecesor[actual] == -1) {
        std::cout << actual;
        return;
    }
    imprimirRutaRecursivo(predecesor[actual], predecesor);
    std::cout << " -> " << actual;
}

//Dijkstra
void Grafo::encontrarRutaMasCorta(int id_origen, int id_destino) {
    double* distancias = new double[capacidad];
    int* predecesor = new int[capacidad];
    MinHeap pq(capacidad);

    for (int i = 0; i < capacidad; i++) {
        distancias[i] = std::numeric_limits<double>::infinity();
        predecesor[i] = -1;
    }

    distancias[id_origen] = 0.0;
    pq.insertar(id_origen, 0.0);

    std::cout << "Calculando ruta desde " << id_origen << " hasta " << id_destino << "..." << std::endl;

    while (!pq.isEmpty()) {
        
        NodoHeap u_nodo = pq.extractMin();
        int u = u_nodo.idVertice;

        if (u == id_destino) break;

        Lista& vecinos = getListaAdyacencia(u);
        NodoLista* vecino = vecinos.getCabeza();

        while (vecino != nullptr) {
            int v = vecino->arista.destino;
            double peso_uv = vecino->arista.peso;
            double nueva_dist = distancias[u] + peso_uv;

            if (nueva_dist < distancias[v]) {
                distancias[v] = nueva_dist;
                predecesor[v] = u;

                if (!pq.estaEnHeap(v)) {
                    pq.insertar(v, nueva_dist);
                } else {
                    pq.actualizarPrioridad(v, nueva_dist);
                }
            }
            vecino = vecino->siguiente;
        }
    }

    if (distancias[id_destino] == std::numeric_limits<double>::infinity()) {
        std::cout << "No se encontro ruta desde " << id_origen << " hasta " << id_destino << std::endl;
    } else {
        std::cout << "¡Ruta encontrada!" << std::endl;
        std::cout << "Distancia total: " << distancias[id_destino] << " metros." << std::endl;
        std::cout << "Camino: ";
        imprimirRutaRecursivo(id_destino, predecesor);
        std::cout << std::endl;
    }

    delete[] distancias;
    delete[] predecesor;
}