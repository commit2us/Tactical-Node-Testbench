#include "Network.h"
#include "Link.h"
#include <vector>

using namespace std;

Network::Network(vector<Base*>& bases, vector<Link*>& edges) {
    // Copy environment information to access it
    bases = bases;
    bases.shrink_to_fit();
    edges = edges;
    edges.shrink_to_fit();

    // Map description to index for O(1) later access
    for (int i = 0; i < bases.size(); i++) {
        baseIdToOrder.emplace(bases[i]->getId(), i);
    }
}

vector<vector<Edge>> Network::buildGraph(
    vector<Base*>& bases,
    vector<Link*>& links,
    Mode mode
) {
    vector<vector<Edge>> graph;

    for (int i = 0; i < links.size(); i++) {
        float finalCost = 0.0f;

        switch (mode) {
            case Mode::Optimal:
                finalCost = ((links[i]->distance * 0.5) 
                    + (links[i]->nodes[1]->getRisk() * 0.5));
            break;
            
            case Mode::Safest:
                finalCost = (links[i]->distance 
                    + links[i]->nodes[1]->getRisk())
            break;

            case Mode::Shortest:
                finalCost = links[i]->distance;
            break;
        }
    }
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

inline vector<Base*> Network::findRoute(Base* from, Base* to) const {
    // Dijkstra
}
