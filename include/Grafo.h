#ifndef GRAFO_H
#define GRAFO_H

struct NodoInfo {
    int idInterno; 
    double lat;
    double lon;
};

class Grafo {
private:
    NodoInfo* nodos; 
    int capacidad;   
    // Aquí irá la lista de adyacencia


public:
    Grafo(int numNodos);
    ~Grafo();

    void agregarNodo(int id, double lat, double lon);

    // (Aquí irán 'agregarArista', 'dijkstra', etc.)
};

#endif