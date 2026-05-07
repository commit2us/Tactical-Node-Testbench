#include "Network.h"
#include <optional>
#include <vector>

using namespace std;

Network::Network(vector<Base*> bases) {
    // TODO
}

inline Base* Network::baseExists(int baseId) const {
    auto base =  orderToBase.find(baseId);

    if (base == orderToBase.end()) return nullptr;

    return base->second;
}

float inline Network::findLink(int baseNode, int linkedNode) const {
    auto node = graph.find(baseNode);

    if (node == graph.end()) return -1;

    auto link = node->second.find(linkedNode);

    if (link == node->second.end()) return -1;
            
    return link->second;
}

inline string Network::getBaseIdByOrder(int order) const {
    Base* base;

    if (!(base = baseExists(order))) return "";

    return base->getId();
}

inline Base* Network::getBaseByID(int baseId) const {
    return baseExists(baseId);
}