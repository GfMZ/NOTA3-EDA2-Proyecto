#ifndef AVL_H
#define AVL_H

#include <iostream> 

struct Nodo {//tipo de dato que agrupa variables de distintos tipos y son p�blicas
    //se crearon los campos:
    int id_nodo;        //se agregaron el id del nodo, su frecuencia y una variable binaria para ddeterminar si hay emergencia o no  
    int frecuencia;      
    bool emergencia;   

    int factor_balance;
    Nodo* der;
    Nodo* izq;
    Nodo* padre;//para eliminar un nodo, �ste debe saber qui�n es su padre

    // Constructor 
    Nodo(int id) : id_nodo(id), frecuencia(1), emergencia(false), 
                   factor_balance(0), izq(nullptr), der(nullptr), padre(nullptr) {}
};


class AVL {
private:
    Nodo* raiz;
    // Función de búsqueda (O(log n))
    Nodo* buscarRecursivo(Nodo* nodo, int id_nodo);
    
    Nodo* insertarRecursivo(Nodo* nodo, int id_nodo);
    
    int calcularAltura(Nodo* nodo);
    void actualizarFactor(Nodo* nodo);
    Nodo* rebalance(Nodo* nodo);
    Nodo* rotarIzquierda(Nodo* nodo);
    Nodo* rotarDerecha(Nodo* nodo);

    void inorden(Nodo* nodo); // Para mostrar nodos por frecuencia
    void encontrarMaxFrecuencia(Nodo* nodo, Nodo*& maxNodo);
    void mostrarEchado(Nodo* nodo, int nivel);
    void destruirArbol(Nodo* nodo);

    // Función auxiliar para evitar incluir <algorithm>
    int _max(int a, int b);

public:
    // Constructor
    AVL();
    // Destructor 
    ~AVL();

    void registrarEmergencia(int id_nodo);
    void marcarCongestion(int id_nodo, bool estado);
    bool estaCongestionado(int id_nodo);
    void mostrarZonasPorFrecuencia();
    void mostrarZonaMasFrecuente();
    void mostrarArbolGrafico();
};

#endif 