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

    this->graph = buildGraph(bases, links);
}

vector<vector<Edge>> Network::buildGraph(
    vector<Base*>& bases,
    vector<Link*>& links
) {
    // Construir la lista de adyacencia usando las bases y conexiones almacenadas
    vector<vector<Edge>> newGraph;

    size_t n = this->bases.size();
    newGraph.resize(n);

    for (size_t i = 0; i < this->links.size(); i++) {
        Link* l = this->links[i];

        // nodes[0] es el origen, nodes[1] es el destino
        auto itA = baseIdToOrder.find(l->nodes[0]->getId());
        auto itB = baseIdToOrder.find(l->nodes[1]->getId());
        
        if (itA == baseIdToOrder.end() || itB == baseIdToOrder.end()) continue;

        int idxA = itA->second; // Índice del origen
        int idxB = itB->second; // Índice del destino

        // Calculamos el costo únicamente para la dirección A -> B
        float costAB = (l->distance * 0.5f) + (l->nodes[1]->getRisk() * 0.5f);

        // Insertamos SOLAMENTE la arista que sale de A y llega a B
        newGraph[idxA].push_back(Edge{ idxB, costAB });
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

int Network::getOrderByBaseId(string baseId) {
    return baseIdToOrder[baseId];
}


vector<Base*> Network::findRoute(Base* from, Base* to, float& costoTotal)  {
    vector<Base*> route;
    costoTotal = 0.0f;

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

    size_t totalBases = bases.size();
    
    
    vector<float> gScore(totalBases, INFINITY); 
    vector<int> parent(totalBases, -1);
    vector<bool> closedSet(totalBases, false); 

    priority_queue<AStarNode, vector<AStarNode>, greater<AStarNode>> openSet;

    gScore[startIdx] = 0.0f;
    float hStart = calculateHeuristic(from->getLocation(), to->getLocation());
    openSet.push(AStarNode{ startIdx, hStart });

    bool found = false;
    
    while (!openSet.empty()) {
        AStarNode current = openSet.top();
        openSet.pop();

        int currIdx = current.index;

        
        if (currIdx == goalIdx) {
            found = true;
            break;
        }

        
        if (closedSet[currIdx]) continue;
        closedSet[currIdx] = true;

        
        for (const Edge& edge : graph[currIdx]) {
            int neighborIdx = edge.target; 

            
            if (closedSet[neighborIdx]) continue; 

            float tentativeGScore = gScore[currIdx] + edge.cost;

            
            if (tentativeGScore < gScore[neighborIdx]) {
                parent[neighborIdx] = currIdx;
                gScore[neighborIdx] = tentativeGScore;
                
                
                float hNeighbor = calculateHeuristic(bases[neighborIdx]->getLocation(), to->getLocation());
                openSet.push(AStarNode{ neighborIdx, tentativeGScore + hNeighbor });
            }
        }
    }

    if (found) {
        costoTotal = gScore[goalIdx]; 

        int curr = goalIdx;
        while (curr != startIdx) {
            route.push_back(bases[curr]); 
            curr = parent[curr];
        }
        route.push_back(bases[startIdx]);
        
        reverse(route.begin(), route.end());
    }

    return route;
}