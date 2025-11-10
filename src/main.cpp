#include <iostream>
#include <fstream>
#include <string>   
#include <cmath>    
#include "../include/TablaHash.h"
#include "../include/Grafo.h"
#include "../include/AVL.h"

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

void Clear(){
    for (int i = 0; i < 3; i++)
    {   cout<<"\n";
        /* code */
    }
    
}

void ClearBuffer(){
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

//Programa Principal
int main() {
    
    const int MAX_NODOS = 3000;
    TablaHash traductor(MAX_NODOS * 2 + 1); 
    Grafo miMapa(MAX_NODOS);
    AVL registroEmergencias;

    ifstream archivo("../data/map.osm");
    if (!archivo.is_open()) {
        cout << "Error: No se pudo abrir el archivo map.osm" << endl;
        return 1;
    }
    cout << "Leyendo archivo map.osm..." << endl;
    string linea;
    int contador_nodos = 0;

    while (getline(archivo, linea)) {
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
            }else{
                break; //Sale del bucle 
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
    int aristas_cargadas = 0;

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
                    int id_A = traductor.buscar(osm_ids_buffer[i]);
                    int id_B = traductor.buscar(osm_ids_buffer[i+1]);
                    if (id_A != -1 && id_B != -1) {
                        NodoInfo nodoA = miMapa.getNodoInfo(id_A);
                        NodoInfo nodoB = miMapa.getNodoInfo(id_B);
                        double peso = calcularDistancia(nodoA.lat, nodoA.lon, nodoB.lat, nodoB.lon);
                        miMapa.agregarArista(id_A, id_B, peso);
                        aristas_cargadas++;
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
    cout << "Se cargaron " << aristas_cargadas << " aristas." << endl;
    cout << "¡Sistema cargado! Bienvenido." << endl;
    cout << "Presione Enter para continuar...";
    cin.get();

    int opcion = 0;
    while (true) {
        Clear();
        cout << "=================================================" << endl;
        cout << "   Sistema de Asignacion de Rutas de Emergencia" << endl;
        cout << "=================================================" << endl;
        cout << "MAPA: " << contador_nodos << " nodos y " << aristas_cargadas << " aristas cargadas." << endl;
        cout << "-------------------------------------------------" << endl;
        cout << "1. Calcular Ruta (Dijkstra + MinHeap + Grafo)" << endl;
        cout << "2. Registrar Emergencia (AVL)" << endl;
        cout << "3. Marcar Zona como Congestionada (AVL)" << endl;
        cout << "4. Desmarcar Zona como Congestionada (AVL)" << endl;
        cout << "5. Ver Reportes de Zonas (AVL)" << endl;
        cout << "6. Salir" << endl;
        cout << "=================================================" << endl;
        cout << "Ingrese su opcion: ";

        cin >> opcion;
        if (cin.fail()) { // Si el usuario no ingresa un número
            cin.clear();
            ClearBuffer();
            opcion = 0; // Opción inválida
        }
        ClearBuffer(); // Limpia el 'Enter'

        switch (opcion) {
            case 1: { // CALCULAR RUTA
                int origen, destino;
                cout << "--- Calcular Ruta ---" << endl;
                cout << "Ingrese ID de nodo Origen (0-" << contador_nodos - 1 << "): ";
                cin >> origen;
                cout << "Ingrese ID de nodo Destino (0-" << contador_nodos - 1 << "): ";
                cin >> destino;

                if (origen >= 0 && origen < contador_nodos && destino >= 0 && destino < contador_nodos) {
                    // ¡Aquí llamamos a Dijkstra pasándole el AVL!
                    miMapa.encontrarRutaMasCorta(origen, destino, registroEmergencias);
                } else {
                    cout << "Error: IDs de nodo fuera de rango." << endl;
                }
                break;
            }
            case 2: { // REGISTRAR EMERGENCIA
                int zona;
                cout << "--- Registrar Emergencia ---" << endl;
                cout << "Ingrese ID de la zona (0-" << contador_nodos - 1 << "): ";
                cin >> zona;
                if (zona >= 0 && zona < contador_nodos) {
                    registroEmergencias.registrarEmergencia(zona);
                } else {
                    cout << "Error: ID de zona fuera de rango." << endl;
                }
                break;
            }
            case 3: { // MARCAR CONGESTIÓN
                int zona;
                cout << "--- Marcar Congestion ---" << endl;
                cout << "Ingrese ID de la zona (0-" << contador_nodos - 1 << "): ";
                cin >> zona;
                registroEmergencias.marcarCongestion(zona, true);
                break;
            }
            case 4: { // DESMARCAR CONGESTIÓN
                int zona;
                cout << "--- Desmarcar Congestion ---" << endl;
                cout << "Ingrese ID de la zona (0-" << contador_nodos - 1 << "): ";
                cin >> zona;
                registroEmergencias.marcarCongestion(zona, false);
                break;
            }
            case 5: { // REPORTES
                Clear();
                cout << "--- Reportes de Zonas de Emergencia ---" << endl;
                registroEmergencias.mostrarZonasPorFrecuencia();
                cout << endl;
                registroEmergencias.mostrarZonaMasFrecuente();
                cout << endl;
                registroEmergencias.mostrarArbolGrafico();
                break;
            }
            case 6: { // SALIR
                cout << "Saliendo del sistema. ¡Gracias!" << endl;
                return 0; // Termina el programa
            }
            default: {
                cout << "Opcion invalida. Por favor, intente de nuevo." << endl;
                break;
            }
        } // Fin del switch

        if (opcion != 6) {
            cout << "\nPresione Enter para volver al menu principal...";
            cin.get();
        }

    } // Fin del while(true)
    return 0;

}