#include "../include/Grafo.h"
#include "../include/MinHeap.h"
#include "../include/AVL.h"
#include <limits>
#include <iostream>
#include <vector>
#include <fstream>

using namespace std;

// Constructor
Grafo::Grafo(int numNodos) {
    capacidad = numNodos;
    nodos = new NodoInfo[capacidad];
    adj = new Lista[capacidad];
}

// Destructor
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
        // Grafo NO dirigido
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

// Función auxiliar para reconstruir el camino en un vector
void reconstruirCamino(int actual, int* predecesor, std::vector<int>& camino) {
    if (actual == -1) {
        return;
    }
    reconstruirCamino(predecesor[actual], predecesor, camino);
    camino.push_back(actual);
}

// Función auxiliar para imprimir el camino desde el vector
void imprimirCaminoVector(const std::vector<int>& camino) {
    for (size_t i = 0; i < camino.size(); ++i) {
        cout << camino[i];
        if (i < camino.size() - 1) {
            cout << " -> ";
        }
    }
}

//Función para crear .csv
void Grafo::exportarMatrizAdyacencia(const std::string& filename) {
    ofstream archivo(filename);
    if (!archivo.is_open()) {
        cout << "Error al crear el archivo de matriz: " << filename << endl;
        return;
    }

    cout << "Exportando matriz de adyacencia a " << filename << "..." << endl;
    cout << "Nota: Esto puede tardar unos momentos debido al tamaño del grafo." << endl;

    archivo << "Nodo";
    for (int i = 0; i < capacidad; i++) {
        if (nodos[i].lat != 0 || nodos[i].lon != 0) {
             archivo << "," << nodos[i].idInterno;
        }
    }
    archivo << "\n";


    for (int i = 0; i < capacidad; i++) {
        if (nodos[i].lat == 0 && nodos[i].lon == 0) continue;

        archivo << nodos[i].idInterno; 

        for (int j = 0; j < capacidad; j++) {
            if (nodos[j].lat == 0 && nodos[j].lon == 0) continue;

            double peso = 0.0; 

            if (i == j) {
                peso = 0.0; 
            } else {
                NodoLista* actual = adj[i].getCabeza();
                while (actual != nullptr) {
                    if (actual->arista.destino == j) {
                        peso = actual->arista.peso;
                        break; 
                    }
                    actual = actual->siguiente;
                }
            }
            archivo << "," << peso;
        }
        archivo << "\n";

        if (i % 50 == 0) cout << "." << flush;
    }

    cout << "\n¡Matriz exportada exitosamente!" << endl;
    archivo.close();
}

//Dijkstra
std::vector<int> Grafo::encontrarRutaMasCorta(int id_origen, int id_destino, AVL& registro, bool ignorarCongestion) {

    double* distancias = new double[capacidad];
    int* predecesor = new int[capacidad];
    MinHeap pq(capacidad);

    for (int i = 0; i < capacidad; i++) {
        distancias[i] = numeric_limits<double>::infinity();
        predecesor[i] = -1;
    }

    distancias[id_origen] = 0.0;
    pq.insertar(id_origen, 0.0);

    if (!ignorarCongestion) {
        cout << "Calculando ruta optima considerando congestion..." << endl;
    }

    while (!pq.isEmpty()) {
        NodoHeap u_nodo = pq.extractMin();
        int u = u_nodo.idVertice;

        if (u == id_destino) break;

        Lista& vecinos = getListaAdyacencia(u);
        NodoLista* vecino = vecinos.getCabeza();

        while (vecino != nullptr) {
            int v = vecino->arista.destino;
            double peso_uv = vecino->arista.peso;

            if (!ignorarCongestion && registro.estaCongestionado(v)) {
                peso_uv = peso_uv * 50.0; 
            }

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

    std::vector<int> rutaCalculada;
    if (distancias[id_destino] == numeric_limits<double>::infinity()) {
        cout << (ignorarCongestion ? "[Ruta Base] " : "[Ruta Optima] ") << "No se encontro camino." << endl;
    } else {
        reconstruirCamino(id_destino, predecesor, rutaCalculada);

        cout << (ignorarCongestion ? "[Ruta Base (Sin Trafico)] " : "[Ruta Optima (Con Trafico)] ") << endl;
        cout << "  Distancia (costo): " << distancias[id_destino] << endl;
        cout << "  Camino: ";
        imprimirCaminoVector(rutaCalculada);
        cout << endl;
    }

    delete[] distancias;
    delete[] predecesor;

    return rutaCalculada;

}