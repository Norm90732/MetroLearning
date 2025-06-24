///
/// Uses boost graph library to model stations as a directed, weighted graph
/// Weights are time to traverse.
///

#ifndef METROGRAPH_H
#define METROGRAPH_H
#include <unordered_map>

#include "Station.h"
#include <boost/graph/adjacency_list.hpp> //Adjacency list
#include <boost/graph/graph_traits.hpp> //Graph type
#include <boost/property_map/property_map.hpp> //Map of weights
#include <boost/graph/properties.hpp> //Property tag



class MetroGraph {
private:
    using GraphType = boost::adjacency_list<
        boost::vecS,                                          //edge container
        boost::vecS,                                          //vertex container
        boost::directedS,                                     //directed graph dec
       Station,   //vertex prop
        boost::property<boost::edge_weight_t, double>         //edge prop
    >;

    using Vertex = boost::graph_traits<GraphType>::vertex_descriptor;
    using Edge = boost::graph_traits<GraphType>::edge_descriptor;

    GraphType graph;
    std::unordered_map<std::string, Vertex> nameToVertex;

public:
    MetroGraph() = default;
    ~MetroGraph() = default;

    Vertex addStation(const Station& station) {
        Vertex v = boost::add_vertex(station,graph);
        nameToVertex[station.getStationName()] = v;
        return v;
    }

};
#endif //METROGRAPH_H
