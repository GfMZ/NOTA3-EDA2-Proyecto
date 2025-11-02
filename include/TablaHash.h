#ifndef TABLAHASH_H
#define TABLAHASH_H

struct HashEntry {
    long long key; // El ID de OSM
    int value;     // El ID interno 
    bool ocupado;  

    HashEntry() : key(0), value(-1), ocupado(false) {}
};

class TablaHash {
private:
    HashEntry* tabla;
    int tamano; 
    int ocupados; 

    int funcionHash(long long key) {
        return key % tamano;
    }

public:
    TablaHash(int tam);
    ~TablaHash();

    void insertar(long long key, int value);

    int buscar(long long key); 
};

#endif