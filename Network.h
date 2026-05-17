#ifndef NETWORK_H
#define NETWORK_H

#include "Base.h"
#include "Link.h"

#include <unordered_map>
#include <vector>
#include <string>

class Network{
private:

    std::vector<std::vector<Edge>> graph;
    std::vector<Base*> bases;
    std::vector<Link*> links;
    std::unordered_map<std::string, int> baseIdToOrder;

public:
    Network(std::vector<Base*>& bases, std::vector<Link*>& edges);

    std::vector<std::vector<Edge>> buildGraph(
        std::vector<Base*>& bases,
        std::vector<Link*>& links
    );
   
    Base* baseExists(int baseId) const;

    std::string getBaseByOrder(int order) const;

    int getOrderByBaseId(std::string id);

    std::vector<Base*> findRoute(Base* from, Base* to, float& costoTotal);
};

#endif