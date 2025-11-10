#include "../include/SVGExporter.h"
#include <fstream>
#include <iostream>
#include <limits>
#include <cmath>

using namespace std;

double SVGExporter::mapLonToX(double lon, double minLon, double maxLon, double width) {
    return (lon - minLon) / (maxLon - minLon) * width;
}

double SVGExporter::mapLatToY(double lat, double minLat, double maxLat, double height) {
    return height - ((lat - minLat) / (maxLat - minLat) * height);
}

void SVGExporter::exportarGrafo(const string& filename, Grafo& grafo, int numNodos, const vector<int>& rutaResaltada) {
    ofstream archivo(filename);
    if (!archivo.is_open()) {
        cout << "Error al crear archivo SVG: " << filename << endl;
        return;
    }

    // 1. Encontrar los límites geográficos
    double minLat = numeric_limits<double>::max(), maxLat = numeric_limits<double>::lowest();
    double minLon = numeric_limits<double>::max(), maxLon = numeric_limits<double>::lowest();

    for (int i = 0; i < numNodos; i++) {
        NodoInfo info = grafo.getNodoInfo(i);
        // Ignorar nodos no inicializados (lat/lon 0) si tu grafo tiene huecos
        if (info.lat == 0 && info.lon == 0) continue;

        if (info.lat < minLat) minLat = info.lat;
        if (info.lat > maxLat) maxLat = info.lat;
        if (info.lon < minLon) minLon = info.lon;
        if (info.lon > maxLon) maxLon = info.lon;
    }

    double margen = 0.0005;
    minLat -= margen; maxLat += margen;
    minLon -= margen; maxLon += margen;

    double width = 2000.0; // Aumentamos resolución para mejor detalle
    double height = 2000.0;
    double geoAspect = (maxLon - minLon) / (maxLat - minLat);
    if (geoAspect > 1.0) { height = width / geoAspect; }
    else { width = height * geoAspect; }

    // 2. Cabecera SVG
    archivo << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
    archivo << "<svg xmlns=\"http://www.w3.org/2000/svg\" width=\"" << width << "\" height=\"" << height << "\" viewBox=\"0 0 " << width << " " << height << "\">\n";
    archivo << "<rect width=\"100%\" height=\"100%\" fill=\"#f8f9fa\" />\n"; // Fondo ligeramente gris para que se vea moderno

    // 3. Dibujar TODAS las aristas (calles)
    archivo << "<g stroke=\"#d1d5db\" stroke-width=\"1.5\" stroke-linecap=\"round\">\n";
    for (int i = 0; i < numNodos; i++) {
        NodoInfo u_info = grafo.getNodoInfo(i);
        if (u_info.lat == 0) continue;

        double x1 = mapLonToX(u_info.lon, minLon, maxLon, width);
        double y1 = mapLatToY(u_info.lat, minLat, maxLat, height);

        Lista& adj = grafo.getListaAdyacencia(i);
        NodoLista* actual = adj.getCabeza();
        while (actual != nullptr) {
            int v = actual->arista.destino;
            if (i < v) {
                NodoInfo v_info = grafo.getNodoInfo(v);
                 if (v_info.lat != 0) {
                    double x2 = mapLonToX(v_info.lon, minLon, maxLon, width);
                    double y2 = mapLatToY(v_info.lat, minLat, maxLat, height);
                    archivo << "<line x1=\"" << x1 << "\" y1=\"" << y1 << "\" x2=\"" << x2 << "\" y2=\"" << y2 << "\" />\n";
                 }
            }
            actual = actual->siguiente;
        }
    }
    archivo << "</g>\n";

    // --- NUEVO: DIBUJAR TODOS LOS NODOS COMO PUNTOS ---
    archivo << "<g fill=\"#6b7280\">\n"; // Color gris oscuro para los nodos
    for (int i = 0; i < numNodos; i++) {
        NodoInfo info = grafo.getNodoInfo(i);
        if (info.lat == 0) continue;
        double cx = mapLonToX(info.lon, minLon, maxLon, width);
        double cy = mapLatToY(info.lat, minLat, maxLat, height);
        // Dibujamos un círculo pequeño (radio 2)
        archivo << "<circle cx=\"" << cx << "\" cy=\"" << cy << "\" r=\"2\" />\n";
    }
    archivo << "</g>\n";
    // --------------------------------------------------

    // 4. Dibujar Ruta Resaltada (si existe)
    if (!rutaResaltada.empty()) {
        // "Halo" blanco alrededor de la ruta para que destaque más sobre el fondo de calles
        archivo << "<polyline points=\"";
        for (int id : rutaResaltada) {
            NodoInfo info = grafo.getNodoInfo(id);
            archivo << mapLonToX(info.lon, minLon, maxLon, width) << ","
                    << mapLatToY(info.lat, minLat, maxLat, height) << " ";
        }
        archivo << "\" style=\"fill:none;stroke:white;stroke-width:8;stroke-opacity:0.7\" />\n";

        // La línea de ruta real
        archivo << "<polyline points=\"";
        for (int id : rutaResaltada) {
            NodoInfo info = grafo.getNodoInfo(id);
            archivo << mapLonToX(info.lon, minLon, maxLon, width) << ","
                    << mapLatToY(info.lat, minLat, maxLat, height) << " ";
        }
        archivo << "\" style=\"fill:none;stroke:#ef4444;stroke-width:4\" />\n"; // Rojo intenso

        // Inicio y Fin
        NodoInfo inicio = grafo.getNodoInfo(rutaResaltada.front());
        NodoInfo fin = grafo.getNodoInfo(rutaResaltada.back());
        archivo << "<circle cx=\"" << mapLonToX(inicio.lon, minLon, maxLon, width)
                << "\" cy=\"" << mapLatToY(inicio.lat, minLat, maxLat, height) << "\" r=\"8\" fill=\"#22c55e\" stroke=\"white\" stroke-width=\"2\"/>\n"; // Verde
        archivo << "<circle cx=\"" << mapLonToX(fin.lon, minLon, maxLon, width)
                << "\" cy=\"" << mapLatToY(fin.lat, minLat, maxLat, height) << "\" r=\"8\" fill=\"#3b82f6\" stroke=\"white\" stroke-width=\"2\"/>\n"; // Azul
    }

    archivo << "</svg>\n";
    archivo.close();
    cout << "\n[VISUALIZADOR] Mapa exportado exitosamente a: " << filename << endl;
}