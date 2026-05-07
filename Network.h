#ifndef NETWORK_H
#define NETWORK_H

#include "Base.h"
#include "Link.h"
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Network{
private:
    // Lightweigh translation from Base objects to O(1) access structures
    std::unordered_map<int, std::unordered_map<int, float>> graph;
    std::unordered_map<int, Base*> idBases;
    std::unordered_map<std::string, int> basesDescriptionId;

public:
    Network(std::vector<Base*> bases);

    // Return the cost of the link if found, -1 if not
    inline float findLink(int baseNode, int linkedNode) const;

    // Return base description from the bases' map {id : obj}
    inline string getBaseDescriptionById(int baseId) const;

    // Return full object from the bases' map {id : obj}
    inline Base* getBaseByID(int baseId) const;

    // Return the optimal path to communicate two bases
    inline std::vector<Base*> findRoute(Base* from, Base* to) const;
};

#endif