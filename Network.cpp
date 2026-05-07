#include "Network.h"
#include <vector>

using namespace std;

Network::Network(vector<Base*> bases) {
    // TODO
}

float inline Network::findLink(int baseNode, int linkedNode) const {
    auto node = graph.find(baseNode);

    if (node == graph.end()) return -1;

    auto link = node->second.find(linkedNode);

    if (link == node->second.end()) return -1;
            
    return link->second;

}
