#ifndef MINHEAP_H
#define MINHEAP_H

struct NodoHeap {
    int idVertice; // ID del nodo (0-999)
    double prioridad; // peso
};

class MinHeap {
private:
    NodoHeap* heap;     
    int* posicion;      
    int capacidad;      
    int tamanoActual;   
    int parent(int i) { return (i - 1) / 2; }
    int left(int i) { return (2 * i + 1); }
    int right(int i) { return (2 * i + 2); }
    void swapNodos(int idx1, int idx2);
    void bubbleUp(int i);
    void minHeapify(int i);

public:
    MinHeap(int cap);
    ~MinHeap();

    void insertar(int idVertice, double prioridad);
    NodoHeap extractMin();
    void actualizarPrioridad(int idVertice, double nuevaPrioridad);
    bool isEmpty();
    bool estaEnHeap(int idVertice); 
};

#endif