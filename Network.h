#ifndef NETWORK_H
#define NETWORK_H

#include "Base.h"
#include "Link.h"

#include <unordered_map>
#include <vector>

class Network{
private:
    // Lightweigh translation from Base objects to O(1) access structures
    std::vector<std::vector<Edge>> graph;
    std::vector<Base*> bases;
    std::vector<Link*> links;
    std::unordered_map<std::string, int> baseIdToOrder;

    

public:
    // Translate Base objects to internal structures
    Network(std::vector<Base*>& bases, std::vector<Link*>& edges);

    std::vector<std::vector<Edge>> buildGraph(
        std::vector<Base*>& bases,
        std::vector<Link*>& links,
        Mode mode
    );
    
    inline Base* baseExists(int baseId) const;

    // Return base description from the bases' map {id : obj}
    inline std::string getBaseByOrder(int order) const;

    // Return full object from the bases' map {id : obj}
    inline Base* getOrderByBaseId(string id) const;

    // Return the optimal path to communicate two bases
    inline std::vector<Base*> findRoute(Base* from, Base* to) const;
};

#endif