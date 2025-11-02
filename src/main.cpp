#include <iostream>
#include <fstream>
#include <string>   
#include <cmath>    
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

double toRadians(double degree) {
    return degree * M_PI / 180.0; 
}

//Haversine
double calcularDistancia(double lat1, double lon1, double lat2, double lon2) {
    double R = 6371e3;
    double radLat1 = toRadians(lat1);
    double radLat2 = toRadians(lat2);
    double deltaLat = toRadians(lat2 - lat1);
    double deltaLon = toRadians(lon2 - lon1);

    double a = sin(deltaLat / 2) * sin(deltaLat / 2) + cos(radLat1) * cos(radLat2) * sin(deltaLon / 2) * sin(deltaLon / 2);
    double c = 2 * atan2(sqrt(a), sqrt(1 - a));

    return R * c; 
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
    cout << "Se cargaron " << contador_nodos << " nodos exitosamente"<< endl;

    cout << "Iniciando lectura de aristas..." << endl;
    archivo.open("../data/map.osm"); 
    if (!archivo.is_open()) {
        cout << "Error: No se pudo re-abrir el archivo map.osm" << endl;
        return 1;
    }
    bool dentroDeWay = false;
    long long osm_ids_buffer[500];
    int buffer_count = 0;

    while (getline(archivo, linea)) {
        if (linea.find("<way ") != string::npos) {
            dentroDeWay = true;
            buffer_count = 0; 
            continue; 
        }

        if (linea.find("</way>") != string::npos) {
            dentroDeWay = false;
            
            if (buffer_count >= 2) {
                for (int i = 0; i < buffer_count - 1; i++) {
                    long long osm_id_A = osm_ids_buffer[i];
                    long long osm_id_B = osm_ids_buffer[i+1];
                    int id_A = traductor.buscar(osm_id_A);
                    int id_B = traductor.buscar(osm_id_B);

                    if (id_A != -1 && id_B != -1) {
                        NodoInfo nodoA = miMapa.getNodoInfo(id_A);
                        NodoInfo nodoB = miMapa.getNodoInfo(id_B);
                        double peso = calcularDistancia(nodoA.lat, nodoA.lon, nodoB.lat, nodoB.lon);
                        miMapa.agregarArista(id_A, id_B, peso);
                    }
                }
            }
            buffer_count = 0;
            continue; 
        }

        if (dentroDeWay && linea.find("<nd ref=") != string::npos) {
            string osm_id_str = getAttribute(linea, "ref");
            
            if (!osm_id_str.empty() && buffer_count < 500) {
                osm_ids_buffer[buffer_count] = stoll(osm_id_str);
                buffer_count++;
            }
        }
    }

    archivo.close();
    cout << "Se cargaron todas las aristas" << endl;

    // (Aquí estara la implementacion de Dijkstra)

    return 0;
}