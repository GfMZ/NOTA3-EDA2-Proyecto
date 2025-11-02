#ifndef LISTA_H
#define LISTA_H

struct Arista {
    int destino;
    double peso; 
};

struct NodoLista {
    Arista arista;
    NodoLista* siguiente;

    NodoLista(Arista a) : arista(a), siguiente(nullptr) {}
};

class Lista {
private:
    NodoLista* cabeza;
public:
    Lista() : cabeza(nullptr) {}

    ~Lista() {
        NodoLista* actual = cabeza;
        while (actual != nullptr) {
            NodoLista* temp = actual;
            actual = actual->siguiente;
            delete temp;
        }
    }

    void insertar(Arista a) {
        NodoLista* nuevo = new NodoLista(a);
        nuevo->siguiente = cabeza;
        cabeza = nuevo;
    }

    NodoLista* getCabeza() {
        return cabeza;
    }
};
#endif