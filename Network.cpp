#include "Network.h"
#include "Link.h"
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>
using namespace std;

Network::Network(vector<Base*>& bases, vector<Link*>& edges) {
    // Copy environment information to access it
    this->bases = bases;
    this->bases.shrink_to_fit();
    this->links = edges;
    this->links.shrink_to_fit();

    // Map description to index for O(1) later access
    for (int i = 0; i < this->bases.size(); i++) {
        baseIdToOrder.emplace(this->bases[i]->getId(), i);
    }
}

vector<vector<Edge>> Network::buildGraph(
    vector<Base*>& bases,
    vector<Link*>& links,
    Mode mode
) {
    // Build adjacency list using the currently stored bases/links
    vector<vector<Edge>> graph;

    size_t n = this->bases.size();
    graph.resize(n);

    for (size_t i = 0; i < this->links.size(); i++) {
        Link* l = this->links[i];

        // Find indices for both endpoints using baseIdToOrder
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

        graph[idxA].push_back(Edge{ this->bases[idxB], costAB });
        graph[idxB].push_back(Edge{ this->bases[idxA], costBA });
    }

    return graph;
}

inline Base* Network::baseExists(int order) const {
    if (order < 0 or order >= bases.size()) return nullptr;

    return bases[order];
}

inline string Network::getBaseByOrder(int order) const {
    Base* base;

    if (!(base = baseExists(order))) return "";

    return base->getId();
}

inline Base* Network::getOrderByBaseId(int baseId) const {
    return baseExists(baseId);
}

vector<Base*> Network::findRoute(Base* from, Base* to) const {
    struct  AStar {
        int index;
        float fscore;
        bool operator >(const AStar Node& other) const
        {
            return fscore > other.fscore;
        }

        
    };
    
}
