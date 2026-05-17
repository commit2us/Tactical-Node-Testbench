#include "Network.h"
#include "Link.h"
#include <vector>
#include <queue>
#include <unordered_map>
#include <cmath>
#include <algorithm>
#include <string>

using namespace std;


struct AStarNode {
    int index;
    float fScore; // f = g + h
    
    // Hace que el nodo con menor costo quede hasta arriba en la cola
    bool operator>(const AStarNode& other) const {
        return fScore > other.fScore;
    }
};

// Heurística de A*: Distancia geométrica en línea recta hasta la meta
float calculateHeuristic(Location from, Location to) {
    return std::hypot(from.x - to.x, from.y - to.y);
}

Network::Network(vector<Base*>& bases, vector<Link*>& edges) {
    // Copiar la información del entorno para poder acceder a ella
    this->bases = bases;
    this->bases.shrink_to_fit();
    this->links = edges;
    this->links.shrink_to_fit();

    // Mapear el ID de texto a un índice numérico para acceso O(1)
    for (int i = 0; i < this->bases.size(); i++) {
        baseIdToOrder.emplace(this->bases[i]->getId(), i);
    }
}

vector<vector<Edge>> Network::buildGraph(
    vector<Base*>& bases,
    vector<Link*>& links,
    Mode mode
) {
    // Construir la lista de adyacencia usando las bases y conexiones almacenadas
    vector<vector<Edge>> newGraph;

    size_t n = this->bases.size();
    newGraph.resize(n);

    for (size_t i = 0; i < this->links.size(); i++) {
        Link* l = this->links[i];

        // Encontrar los índices de ambos extremos usando baseIdToOrder
        auto itA = baseIdToOrder.find(l->nodes[0]->getId());
        auto itB = baseIdToOrder.find(l->nodes[1]->getId());
        if (itA == baseIdToOrder.end() || itB == baseIdToOrder.end()) continue;

        int idxA = itA->second;
        int idxB = itB->second;

        float costAB = 0.0f;
        float costBA = 0.0f;

        switch (mode) {
            case Mode::Optimal:
                costAB = (l->distance * 0.5f) + (l->nodes[1]->getRisk() * 0.5f);
                costBA = (l->distance * 0.5f) + (l->nodes[0]->getRisk() * 0.5f);
                break;
            case Mode::Safest:
                costAB = l->distance + l->nodes[1]->getRisk();
                costBA = l->distance + l->nodes[0]->getRisk();
                break;
            case Mode::Shortest:
                costAB = l->distance;
                costBA = l->distance;
                break;
        }

        newGraph[idxA].push_back(Edge{ this->bases[idxB], costAB });
        newGraph[idxB].push_back(Edge{ this->bases[idxA], costBA });
    }

    this->graph = newGraph; 
    return newGraph;
}

Base* Network::baseExists(int order) const {
    if (order < 0 || order >= bases.size()) return nullptr;
    return bases[order];
}

string Network::getBaseByOrder(int order) const {
    Base* base;
    if (!(base = baseExists(order))) return "";
    return base->getId();
}

Base* Network::getOrderByBaseId(string baseId) const {
    auto it = baseIdToOrder.find(baseId);
    if (it == baseIdToOrder.end()) return nullptr;
    return baseExists(it->second);
}


vector<Base*> Network::findRoute(Base* from, Base* to) const {
    vector<Base*> route;

    
    if (!from || !to) return route;
    
    auto itStart = baseIdToOrder.find(from->getId());
    auto itGoal = baseIdToOrder.find(to->getId());
    
    if (itStart == baseIdToOrder.end() || itGoal == baseIdToOrder.end()) return route;
    
    int startIdx = itStart->second;
    int goalIdx = itGoal->second;

    if (startIdx == goalIdx) {
        route.push_back(from);
        return route;
    }

    
    unordered_map<int, float> gScore; // Almacena el costo real acumulado
    unordered_map<int, int> parent;   // Rastrea el camino de regreso
    priority_queue<AStarNode, vector<AStarNode>, greater<AStarNode>> openSet; // Cola de prioridad

    // Inicializar el punto de partida
    gScore[startIdx] = 0.0f;
    float hStart = calculateHeuristic(from->getLocation(), to->getLocation());
    openSet.push(AStarNode{ startIdx, hStart });

    bool found = false;

    // 3. Bucle principal de exploración
    while (!openSet.empty()) {
        AStarNode current = openSet.top();
        openSet.pop();

        int currIdx = current.index;

        // Si encontramos la meta, terminamos la búsqueda inmediatamente
        if (currIdx == goalIdx) {
            found = true;
            break;
        }

        // Si ya encontramos un camino mejor para este nodo antes, saltamos esta evaluación
        float currentH = calculateHeuristic(bases[currIdx]->getLocation(), to->getLocation());
        if (gScore.find(currIdx) != gScore.end() && current.fScore > gScore[currIdx] + currentH) {
            continue;
        }

        // Evaluar cada camino conectado (vecino) en el grafo
        for (const Edge& edge : graph[currIdx]) {
            auto itNeighbor = baseIdToOrder.find(edge.target->getId());
            if (itNeighbor == baseIdToOrder.end()) continue;
            
            int neighborIdx = itNeighbor->second;
            float tentativeGScore = gScore[currIdx] + edge.cost;

            
            if (gScore.find(neighborIdx) == gScore.end() || tentativeGScore < gScore[neighborIdx]) {
                parent[neighborIdx] = currIdx;
                gScore[neighborIdx] = tentativeGScore;
                
                float hNeighbor = calculateHeuristic(edge.target->getLocation(), to->getLocation());
                float fNeighbor = tentativeGScore + hNeighbor;
                
                openSet.push(AStarNode{ neighborIdx, fNeighbor });
            }
        }
    }

    // 4. Reconstrucción del camino desde el final hacia el principio
    if (found) {
        int curr = goalIdx;
        while (curr != startIdx) {
            route.push_back(bases[curr]);
            curr = parent[curr];
        }
        route.push_back(bases[startIdx]);
        
        // Invertimos la ruta para que empiece en 'from' y termine en 'to'
        reverse(route.begin(), route.end());
    }

    return route;
}
