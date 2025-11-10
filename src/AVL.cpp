#include "../include/AVL.h"
#include <iostream>

using namespace std;

AVL::AVL() {
    raiz = nullptr;
}

AVL::~AVL() {
    destruirArbol(raiz);
}

void AVL::destruirArbol(Nodo* nodo) {
    if (nodo) {
        destruirArbol(nodo->izq);
        destruirArbol(nodo->der);
        delete nodo;
    }
}

Nodo* AVL::buscarRecursivo(Nodo* nodo, int id_nodo) {
    if (!nodo) {
        return nullptr;
    }
    if (id_nodo == nodo->id_nodo) {
        return nodo;
    }
    if (id_nodo < nodo->id_nodo) {
        return buscarRecursivo(nodo->izq, id_nodo);
    } else {
        return buscarRecursivo(nodo->der, id_nodo);
    }
}

void AVL::registrarEmergencia(int id_nodo) {
    Nodo* existente = buscarRecursivo(raiz, id_nodo);

    if (existente) {
        existente->frecuencia++;
        cout << "[AVL] Zona " << id_nodo << " ya registrada. Frecuencia aumentada a " << existente->frecuencia << "." << endl;
    } else {
        raiz = insertarRecursivo(raiz, id_nodo);
        cout << "[AVL] Nueva zona de emergencia " << id_nodo << " registrada." << endl;
    }
}

void AVL::marcarCongestion(int id_nodo, bool estado) {
    Nodo* nodo = buscarRecursivo(raiz, id_nodo);
    if (nodo) {
        nodo->emergencia = estado;
        cout << "[AVL] Zona " << id_nodo << " marcada como " << (estado ? "CONGESTIONADA" : "LIBRE") << "." << endl;
    } else {
        cout << "[AVL] Error: No se puede marcar la zona " << id_nodo << " (no existe)." << endl;
    }
}

bool AVL::estaCongestionado(int id_nodo) {
    Nodo* nodo = buscarRecursivo(raiz, id_nodo);
    if (nodo) {
        return nodo->emergencia;
    }
    return false;
}

Nodo* AVL::insertarRecursivo(Nodo* nodo, int id_nodo) {
    //Inserción
    if (!nodo) {
        return new Nodo(id_nodo);
    }

    if (id_nodo < nodo->id_nodo) {
        nodo->izq = insertarRecursivo(nodo->izq, id_nodo);
        if (nodo->izq) nodo->izq->padre = nodo; 
    } else {
        nodo->der = insertarRecursivo(nodo->der, id_nodo);
        if (nodo->der) nodo->der->padre = nodo; 
    }

    actualizarFactor(nodo);
    return rebalance(nodo);
}

int AVL::_max(int a, int b) {
    return (a > b) ? a : b;
}

int AVL::calcularAltura(Nodo* nodo) {
    if (!nodo) return -1;
    return 1 + _max(calcularAltura(nodo->izq), calcularAltura(nodo->der));
}

void AVL::actualizarFactor(Nodo* nodo) {
    if (nodo) {
        nodo->factor_balance = calcularAltura(nodo->der) - calcularAltura(nodo->izq);
    }
}

Nodo* AVL::rebalance(Nodo* nodo) {
    // Desbalanceo Izquierda-Izquierda (Rotación Derecha)
    if (nodo->factor_balance < -1 && nodo->izq->factor_balance <= 0) {
        return rotarDerecha(nodo);
    }
    // Desbalanceo Izquierda-Derecha (Rotación Izquierda-Derecha)
    if (nodo->factor_balance < -1 && nodo->izq->factor_balance > 0) {
        nodo->izq = rotarIzquierda(nodo->izq);
        return rotarDerecha(nodo);
    }
    // Desbalanceo Derecha-Derecha (Rotación Izquierda)
    if (nodo->factor_balance > 1 && nodo->der->factor_balance >= 0) {
        return rotarIzquierda(nodo);
    }
    // Desbalanceo Derecha-Izquierda (Rotación Derecha-Izquierda)
    if (nodo->factor_balance > 1 && nodo->der->factor_balance < 0) {
        nodo->der = rotarDerecha(nodo->der);
        return rotarIzquierda(nodo);
    }

    return nodo;
}

Nodo* AVL::rotarIzquierda(Nodo* nodo) {
    Nodo* nuevaRaiz = nodo->der;
    nodo->der = nuevaRaiz->izq;
    if (nuevaRaiz->izq) nuevaRaiz->izq->padre = nodo;
    nuevaRaiz->izq = nodo;
    nuevaRaiz->padre = nodo->padre;
    nodo->padre = nuevaRaiz;

    actualizarFactor(nodo);
    actualizarFactor(nuevaRaiz);
    return nuevaRaiz;
}

Nodo* AVL::rotarDerecha(Nodo* nodo) {
    Nodo* nuevaRaiz = nodo->izq;
    nodo->izq = nuevaRaiz->der;
    if (nuevaRaiz->der) nuevaRaiz->der->padre = nodo;
    nuevaRaiz->der = nodo;
    nuevaRaiz->padre = nodo->padre;
    nodo->padre = nuevaRaiz;

    actualizarFactor(nodo);
    actualizarFactor(nuevaRaiz);

    return nuevaRaiz;
}

void AVL::mostrarZonasPorFrecuencia() {
    if (!raiz) {
        cout << "[AVL] No hay zonas de emergencia registradas." << endl;
        return;
    }
    cout << "--- Reporte de Zonas de Emergencia ---" << endl;
    inorden(raiz);
    cout << "----------------------------------------" << endl;
}

void AVL::inorden(Nodo* nodo) {
    if (nodo) {
        inorden(nodo->izq);
        cout << "  - Zona " << nodo->id_nodo 
             << ": " << nodo->frecuencia << " emergencias"
             << ", Congestion: " << (nodo->emergencia ? "SI" : "NO") 
             << endl;
        inorden(nodo->der);
    }
}

void AVL::mostrarZonaMasFrecuente() {
    Nodo* maxNodo = nullptr;
    encontrarMaxFrecuencia(raiz, maxNodo);

    if (maxNodo) {
        cout << "--- Zona con Mayor Nivel de Emergencia ---" << endl;
        cout << "  ID de Zona: " << maxNodo->id_nodo << endl;
        cout << "  Frecuencia: " << maxNodo->frecuencia << " emergencias" << endl;
        cout << "  Estado: " << (maxNodo->emergencia ? "CONGESTIONADA" : "LIBRE") << endl;
        cout << "--------------------------------------------" << endl;
    } else {
        cout << "[AVL] No hay zonas de emergencia registradas." << endl;
    }
}

void AVL::encontrarMaxFrecuencia(Nodo* nodo, Nodo*& maxNodo) {
    if (!nodo) {
        return;
    }

    // Compara el nodo actual con el máximo encontrado hasta ahora
    if (maxNodo == nullptr || nodo->frecuencia > maxNodo->frecuencia) {
        maxNodo = nodo;
    }

    // Recorre el resto del árbol
    encontrarMaxFrecuencia(nodo->izq, maxNodo);
    encontrarMaxFrecuencia(nodo->der, maxNodo);
}

void AVL::mostrarArbolGrafico() {
    if (!raiz) {
        cout << "[AVL] El arbol esta vacio." << endl;
        return;
    }
    cout << "--- Visualizacion Grafica del Arbol AVL ---" << endl;
    mostrarEchado(raiz, 0);
    cout << "-------------------------------------------" << endl;
}

void AVL::mostrarEchado(Nodo* nodo, int nivel) {
    if (nodo) {
        mostrarEchado(nodo->der, nivel + 1);
        for (int i = 0; i < nivel; i++) cout << "    ";
        cout << nodo->id_nodo << " (F:" << nodo->frecuencia << ", C:" << nodo->emergencia << ")\n";
        mostrarEchado(nodo->izq, nivel + 1);
    }
}