
#include <boost/graph/adjacency_matrix.hpp>
#include <boost/graph/graph_utility.hpp>
#include <iostream>

#include "Graph.h"

DirectedGraph::DirectedGraph(int n) { 
    boost_graph = std::make_unique<BoostDirectedGraph>(n);
}

void DirectedGraph::Print() const {
    auto name_map = boost::identity_property_map();
    std::cout << "vertex set: ";
    boost::print_vertices(*boost_graph, name_map);
    std::cout << std::endl;
    std::cout << "edges set: ";
    boost::print_edges(*boost_graph, name_map);
    std::cout << std::endl;
}

void DirectedGraph::AddEdge(int i, int j) {
    add_edge(i, j, *boost_graph);
}