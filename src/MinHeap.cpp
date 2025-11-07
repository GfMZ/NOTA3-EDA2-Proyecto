#include "../include/MinHeap.h"

MinHeap::MinHeap(int cap) {
    capacidad = cap;
    tamanoActual = 0;
    heap = new NodoHeap[capacidad];
    posicion = new int[capacidad];
    for (int i = 0; i < capacidad; i++) {
        posicion[i] = -1;
    }
}

MinHeap::~MinHeap() {
    delete[] heap;
    delete[] posicion;
}

void MinHeap::swapNodos(int idx1, int idx2) {
    NodoHeap temp = heap[idx1];
    heap[idx1] = heap[idx2];
    heap[idx2] = temp;
    posicion[heap[idx1].idVertice] = idx1;
    posicion[heap[idx2].idVertice] = idx2;
}

void MinHeap::minHeapify(int i) {
    int l = left(i);
    int r = right(i);
    int smallest = i; 

    if (l < tamanoActual && heap[l].prioridad < heap[smallest].prioridad)
        smallest = l;

    if (r < tamanoActual && heap[r].prioridad < heap[smallest].prioridad)
        smallest = r;

    if (smallest != i) {
        swapNodos(i, smallest);
        minHeapify(smallest);
    }
}

void MinHeap::bubbleUp(int i) {
    while (i > 0 && heap[parent(i)].prioridad > heap[i].prioridad) {
        swapNodos(i, parent(i));
        i = parent(i);
    }
}

bool MinHeap::isEmpty() {
    return tamanoActual == 0;
}

bool MinHeap::estaEnHeap(int idVertice) {
    return posicion[idVertice] != -1;
}

void MinHeap::insertar(int idVertice, double prioridad) {
    if (tamanoActual == capacidad) return; 
    if (estaEnHeap(idVertice)) return; 

    int i = tamanoActual;
    heap[i].idVertice = idVertice;
    heap[i].prioridad = prioridad;
    posicion[idVertice] = i; 
    
    tamanoActual++;
    bubbleUp(i); 
}

NodoHeap MinHeap::extractMin() {
    if (isEmpty()) return {-1, -1};
    NodoHeap raiz = heap[0];
    heap[0] = heap[tamanoActual - 1];
    posicion[heap[0].idVertice] = 0;
    posicion[raiz.idVertice] = -1; 
    tamanoActual--;
    minHeapify(0);
    return raiz;
}

void MinHeap::actualizarPrioridad(int idVertice, double nuevaPrioridad) {
    if (!estaEnHeap(idVertice)) return;
    int i = posicion[idVertice];
    if (nuevaPrioridad < heap[i].prioridad) {
        heap[i].prioridad = nuevaPrioridad;
        bubbleUp(i);
    }
}