#include <iostream>
#include <fstream>  
#include <string>
#include "../include/TablaHash.h"
#include "../include/Grafo.h"

using namespace std;

string getAttribute(const string& linea, const string& attrName) {
    size_t startPos = linea.find(attrName + "=\"");
    if (startPos == string::npos) return "";

    startPos += attrName.length() + 2;

    size_t endPos = linea.find("\"", startPos);
    if (endPos == string::npos) return "";

    return linea.substr(startPos, endPos - startPos);
}

//Programa Principal
int main() {
    
    const int MAX_NODOS = 1000;
    TablaHash traductor(2003); 
    Grafo miMapa(MAX_NODOS);

    ifstream archivo("../data/map.osm");
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo map.osm" << endl;
        return 1;
    }

    cout << "Leyendo archivo map.osm..." << endl;
    
    string linea;
    int contador_nodos = 0;

    while (getline(archivo, linea) && contador_nodos < MAX_NODOS) {
        
        if (linea.find("<node ") != string::npos) {
            
            string id_str = getAttribute(linea, "id");
            string lat_str = getAttribute(linea, "lat");
            string lon_str = getAttribute(linea, "lon");

            if (!id_str.empty() && !lat_str.empty() && !lon_str.empty()) {
                
                long long osm_id = stoll(id_str);
                double lat = stod(lat_str);
                double lon = stod(lon_str);

                traductor.insertar(osm_id, contador_nodos);
                miMapa.agregarNodo(contador_nodos, lat, lon);

                contador_nodos++;
            }
        }
    }

    archivo.close();

    cout << "Se cargaron " << contador_nodos << " nodos exitosamente" << endl;
    return 0;
}