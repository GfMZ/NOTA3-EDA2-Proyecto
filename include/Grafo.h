#ifndef GRAFO_H
#define GRAFO_H

#include "Lista.h"
#include <vector> 
#include <string> 

struct NodoInfo {
    int idInterno;
    double lat;
    double lon;
};

class AVL; 

class Grafo {
private:
    NodoInfo* nodos;
    int capacidad;
    Lista* adj;

public:
    Grafo(int numNodos);
    ~Grafo();
    void agregarNodo(int id, double lat, double lon);
    void agregarArista(int origen, int destino, double peso);
    NodoInfo getNodoInfo(int id);
    Lista& getListaAdyacencia(int id);
    std::vector<int> encontrarRutaMasCorta(int id_origen, int id_destino, AVL& registro, bool ignorarCongestion = false);
    void exportarMatrizAdyacencia(const std::string& filename);
};

#endif