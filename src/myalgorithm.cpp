#include <myalgorithm.h>
#include<mygraphbuilder.h>
//===============================================
using namespace std;
using namespace boost;
//===============================================

MyAlgorithm::MyAlgorithm()// Default constructor
{
  //graph_t MyGraph2;
  cout<<"\nDear User This Algorithm Needs Graph to Work with!\n";
}
//===========================================================================
MyAlgorithm::MyAlgorithm(graph_t const& AnyGraph,GraphMap AnyGraphMap ,idType VSource2){
  // starting a clock to measure time
  double duration;
  clock_t start, stop;
  start = clock();
  //===================================================
  unsigned int VSource = AnyGraphMap.find(VSource2)->second;
  MyGraph2 = AnyGraph;
  src      = VSource ;
  predecessors.resize(num_vertices(MyGraph2));
  distances.resize(num_vertices(MyGraph2)) ;

  dijkstra_shortest_paths(MyGraph2, src,
                          predecessor_map(make_iterator_property_map(predecessors.begin(), get(boost::vertex_index, MyGraph2)))
                          .distance_map(boost::make_iterator_property_map(distances.begin(), get(boost::vertex_index, MyGraph2))));
  //=====================================================================
  stop = clock();
  duration = double(stop - start);
  //=====================================================================
  cout<<"\n Algorithm was running for: \t"<<(duration/CLOCKS_PER_SEC)<<endl;
}// End of Parameters Constructor
//===========================================================================
MyAlgorithm::~MyAlgorithm(){ //Destructur
}
//===========================================================================
// Accessors
// function to get ShortPath
Path MyAlgorithm::getShortPath(GraphMap AnyGraphMap,idType destination2) {
  ResetShortPath();
  unsigned int destination;
  idType Source = 0;
  GraphMap::iterator it;
  destination = AnyGraphMap.find(destination2)->second;
  while (destination != MyGraph2.null_vertex()) {
// index to idType Look-up table
      for (it = AnyGraphMap.begin(); it != AnyGraphMap.end(); it++)
        {
          if (it->second == destination)
            setShortPath(it->first);
        }
      if (destination == src){
          // index to idType Look-up table
          for (it = AnyGraphMap.begin(); it != AnyGraphMap.end(); it++)
            {
              if (it->second == src)
                Source = it->first;
            }
          cout<<" Source \t"<< Source <<endl;
          cout<<" Destination \t"<< destination2 <<endl;
          reverse(ShortPath.begin(),ShortPath.end());
          return ShortPath;
        }
      if (predecessors.at(destination) == destination){
          break;
        }
      destination = predecessors.at(destination);
    }
  cout<<"\nThis Node is Unreachable...\n";
  ResetShortPath();
  // if No path return Acacias Node as single Node in Path, so the Program Doesn't Crush
  ShortPath.emplace_back(1540689869);
  emptyFlag = 0;
  return ShortPath;
  //throw std::runtime_error("Unreachable");
}

// function to call the Graph
graph_t MyAlgorithm::getGraph(){
  return MyGraph2;
}
// function to set empty flag
bool MyAlgorithm::getFlag(){
  return emptyFlag;
}
//===========================================================================
// Mutators
//function to inject node to Short Path
void MyAlgorithm::setShortPath(idType MyNode){
  ShortPath.emplace_back(MyNode);
}
// function to set a graph
void MyAlgorithm::setGraph(graph_t YourGraph){
  MyGraph2 = YourGraph;
}
//============================================================================
//function to reset short path
void MyAlgorithm::ResetShortPath(){
  MyAlgorithm::ShortPath.clear();
}
//============================================================================
//function to Print Out Results
void MyAlgorithm::PrintRawData() const {
  std::cout << "distances and parents:" << std::endl;
  for (auto v : make_iterator_range(vertices(MyGraph2))) {
      std::cout << "distance(" << v << ") = " << distances.at(v) << ", ";
      std::cout << "parent(" << v << ") = " << predecessors.at(v) << std::endl;
    }// End of Print Loop
}// end of Print Function
//===================================================================
// function to print shortest Path
void MyAlgorithm::PrintPath() {
  std::cout << "Path: \n"<<endl;
  for (auto i = ShortPath.begin(); i != ShortPath.end(); ++i)
      std::cout << *i << "....\t";
}
//===================================================================
