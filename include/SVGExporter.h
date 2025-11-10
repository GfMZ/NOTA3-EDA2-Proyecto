#ifndef SVGEXPORTER_H
#define SVGEXPORTER_H

#include <string>
#include <vector>
#include "Grafo.h" // Necesita conocer la estructura del Grafo

class SVGExporter {
public:
    // Función principal: Genera el archivo 'filename' (ej. "mapa.svg")
    // Recibe el grafo completo y opcionalmente una ruta (lista de IDs de nodos) para resaltar.
    static void exportarGrafo(const std::string& filename, Grafo& grafo, int numNodos, const std::vector<int>& rutaResaltada = {});

private:
    // Funciones auxiliares para mapear lat/lon a coordenadas de pantalla (x,y)
    static double mapLonToX(double lon, double minLon, double maxLon, double width);
    static double mapLatToY(double lat, double minLat, double maxLat, double height);
};

#endif