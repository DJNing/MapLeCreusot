#ifndef MYGRAPHBUILDER_H
#define MYGRAPHBUILDER_H

// Generic Libraries
//===============================================
#include <iostream>
#include <vector>
#include <map>

// Boost Libraries
//===============================================
#include <boost/config.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/property_map/property_map.hpp>
#include <boost/graph/graph_utility.hpp>
// Deng Libraries
//===============================================
#include <modelDataStructure.h>
#include <modelDataHandler.h>
#include <modeldata.h>
#include <model.h>
// Osmium Libraries
//===============================================
#include <osmium/osm.hpp>
#include <osmium/index/map/flex_mem.hpp>
#include <osmium/visitor.hpp>
#include <osmium/osm/location.hpp>
#include <osmium/handler.hpp>
#include <osmium/osm/node_ref.hpp>
//===============================================
using namespace std;
using namespace boost;
//===============================================
typedef osmium::unsigned_object_id_type idType ;
//==================================================
typedef map<idType, wayData> WayMap;
typedef adjacency_list<
    boost::vecS,
    boost::vecS,
    boost::directedS,
    boost::property<boost::vertex_name_t, idType>,
    boost::property<boost::edge_weight_t, double>> graph_t;
typedef graph_traits < graph_t >::vertex_descriptor Vertex; // Vertex declaration
typedef graph_traits < graph_t >::edge_descriptor Edge; // Edge as link between two Nodes specified by ID
typedef map<idType, unsigned int> GraphMap;
//==================================================

class MyGraphBuilder
{
private:               // This Line is Useless just for clarity
  graph_t MyGraph;
  GraphMap MyGraphMap;
  Model* OurModel;
  /////////////////////////////////////////////////////////

public:
  MyGraphBuilder(); // default Constructor
  MyGraphBuilder(Model); // Parameters Constructor
  ~MyGraphBuilder(); // Destructor
  void generateGraph();
  double distance(Vertex,Vertex); // function to calculate the distance between 2 Verices
  //===============================================
  //Accessors
  // functions to get the Graph
  graph_t const& getGraph() const;
  graph_t&       getGraph() ;
  //-------------------------------------------------------------------
  // functions to get the Mapping between Nodes ID's and Graph Vertices
  GraphMap const& getGraphMap() const;
  GraphMap        getGraphMap() ;
  //===============================================
  // Mutators
  void setGraph(graph_t);
  void setGraphMap(GraphMap);
   //===============================================
  void printGraph() const;
  //friend class MyAlgorithm;
};//end of the Class
/////////////////////////////////////
#endif // MYGRAPHBUILDER_H
