#pragma once

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/adjacency_matrix.hpp>
#include <memory>

typedef boost::adjacency_matrix< 
    boost::directedS,
    boost::no_property,
    boost::property< boost::edge_weight_t, int >
> BoostDirectedGraph;


//
// Wrapper around Boost Graph Library classes
//
class DirectedGraph {
    
    public:
        DirectedGraph(int n);
        ~DirectedGraph() { };
        
        void Print() const;
        void AddEdge(int i, int j);

    protected:
        std::unique_ptr<BoostDirectedGraph> boost_graph;

    private:

};