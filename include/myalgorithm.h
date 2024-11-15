#ifndef MYALGORITHM_H
#define MYALGORITHM_H

// Generic Libraries
//===============================================
#include <iostream>
#include <vector>
#include <map>
// Boost Libraries
//===============================================
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/dag_shortest_paths.hpp>
//===============================================
#include <mygraphbuilder.h>
//===============================================
using namespace boost;
using namespace std;
using Path = vector <idType>;
//===============================================

class MyAlgorithm
{
private:
  graph_t MyGraph2;
  unsigned int src;
  bool emptyFlag = 1;
  vector <unsigned int> predecessors; // Vector to Store predecessors
  vector <double> distances;    // Vector of Edge Weights(Distances)
  vector <idType> ShortPath;   // My Shortest Path is a Vector of Vertices
  //===============================================
public:
  MyAlgorithm();//Default Constructor
  MyAlgorithm(graph_t const&,GraphMap,idType);//,Vertex);//Parameters Constructor
  ~MyAlgorithm();//Default Destructor

  //===============================================
  //Accessors
  // function to call the Shortest path as Vector of Nodes
  Path getShortPath(GraphMap , idType);
  // function to get the Graph
  graph_t getGraph();
  bool getFlag();
  //===============================================
  // Mutators
  void setGraph(graph_t);
  // function to assign Shortest path Vertex by Vertex (Overloaded Function)
  void setShortPath(idType);
  //===============================================
  // function to Reset Shortest path Vector
  void ResetShortPath();
  // function to Print Out The Results
  void PrintRawData() const;
  // function to print out The Path
  void PrintPath();
};

#endif // MYALGORITHM_H
