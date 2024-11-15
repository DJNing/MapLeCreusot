#include <math.h>

#include <mygraphbuilder.h>

using namespace std;
using namespace boost;


MyGraphBuilder::MyGraphBuilder() // default Constructor
{
  //Model OurModel;
  cout<<"\nDear User Be Careful This is Empty Graph !\n";
  //==========================================================
} // end of Default Constructor
//===========================================================================
MyGraphBuilder::MyGraphBuilder(Model YourModel){  // Parameters Constructor

  //==========================================================
  OurModel = &YourModel;

}// end of Parameters Constructor

MyGraphBuilder::~MyGraphBuilder (){ // default Destructor

}
//================================================================
// Graph Generator
//================================================================
void MyGraphBuilder::generateGraph() {
  // starting a clock to measure time
  double duration;
  clock_t start, stop;
  start = clock();
  //===================================================
  //Getting The WayMap
  WayMap MyWayMap = OurModel->getWayMap();
  // Building a map between Graph Vertices and OSM Nodes
  GraphMap BelalMap;
  //Iterators for looping the maps
  WayMap::iterator it;
  //GraphMap::iterator ite;
  // Counter of all Nodes in OSM File
  unsigned int Nodecount = 0;
  // IdMap Index
  unsigned int IdMapIndex = 0;
  //unsigned int NodesOfWayIndex =0;
  unsigned int NodesOfWayIndex; //define Index
  unsigned int Node1 = 0;
  unsigned int Node2 = 0;
  //===================================================
  //===================================================
  // counting My Nodes
  for ( it = MyWayMap.begin(); it != MyWayMap.end(); it++ ){
      // Loop The Nodes of Each way
      auto &nodes = it->second.nodeRefList;
      for (auto i1 = nodes.begin(), i2 = i1 + 1 ; i2 != nodes.end(); ++i1, ++i2) {
          Nodecount++;
        }// end of Inner Loop

    }// end of Outer Loop
  //---------------------------------------------------
  // graph with node count vertices
  MyGraph = graph_t(Nodecount);
  idType VertexID = 0; // define Variable to Store Vertix Index
  // define a map to store Nodes ID
  //auto idmap = get(boost::vertex_name, MyGraph);
  int WayCounter = 0;
  //===================================================
  // Loop the Whole Map of Ways
  for ( it = MyWayMap.begin(); it != MyWayMap.end(); it++ ){
      WayCounter++;
      NodesOfWayIndex = 0; //reset Index
      //define a vector of nodes with size of Way
      vector<Vertex> WayNodes(it->second.nodeRefList.size());
      //======================================================
      // Loop The Nodes of Each way
      for(auto j = it->second.nodeRefList.begin(); j != it->second.nodeRefList.end(); ++j){

          VertexID = it->second.nodeRefList.at(NodesOfWayIndex);
          //declare Variable for Eucledean Distance
          double dist = 0;
          WayNodes[NodesOfWayIndex] = VertexID ;
          //---------------------------------------------------------------------
          // if the VertexId doesn't exist yet give back do the following
          if(BelalMap.find(VertexID) == BelalMap.end()){
              // assign idType values to the idmap
              //idmap[IdMapIndex] = VertexID;
              IdMapIndex++;
              // Fill BelalMap
              BelalMap.insert({VertexID,IdMapIndex});
              if(NodesOfWayIndex == 0) Node1 = IdMapIndex;
              else {
                  Node2 = IdMapIndex ;
                  dist = distance(WayNodes[NodesOfWayIndex - 1], WayNodes[NodesOfWayIndex]);
                  add_edge(Node1, Node2,dist,MyGraph);
                  add_edge(Node2, Node1,dist,MyGraph);
                  Node1 = Node2 ;
                } // end else
            }//end of outer if
          //--------------------------------------------------------------------
          //if the VertexId already exists give back it's Vertex number
          else {
              if(NodesOfWayIndex == 0) Node1 = BelalMap.find(VertexID)->second;
              else {
                  // Calculating Eucledan Distance Between Nodes
                  dist = distance(WayNodes[NodesOfWayIndex - 1], WayNodes[NodesOfWayIndex]);
                  Node2 = BelalMap.find(VertexID)->second;
                  add_edge(Node1, Node2,dist,MyGraph);
                  add_edge(Node2, Node1,dist,MyGraph);
                  Node1 = Node2 ;
                }// end of inner else
            }//end of outer else

          //======================================================
          // increase The indexs after iterating each node.
          NodesOfWayIndex++;
        }// end of Nodes of Way Loop
    }// end of Ways of Map Loop
  MyGraphMap = BelalMap ;
  //  cout<<"size of Belal Map is :\t"<<BelalMap.size()<<endl;
  cout<<"\nCount of Ways is :\t"<<WayCounter<<endl;
  cout<<"Graph Was Built ..."<<endl;
  //=======================
  stop = clock();
  duration = double(stop - start);
  //=======================
  cout<<"\nUsed time for Building The Graph: \t"<<(duration/CLOCKS_PER_SEC)<<endl;
}//end of Genrate Function

//================================================================
// Function to calculate Euclidean Distance between Vertices
double MyGraphBuilder::distance(Vertex Nod1_ID, Vertex Nod2_ID){
  double dist = 0; // distance
  auto  L1 = OurModel->getNodeLoaction(Nod1_ID) ; // get first location
  auto  L2 = OurModel->getNodeLoaction(Nod2_ID) ; // get second location
  //-------------------------------------------------
  double x1 = L1.lat(); // first location latitude
  double x2 = L2.lat(); // second location latitude
  double y1 = L1.lon(); // first location longitude
  double y2 = L2.lon(); // second location longitude
  //-------------------------------------------------
  dist = sqrt(pow((x1-x2),2)+pow((y1-y2),2));

  return 1000000*dist;
}
//================================================================
// Accessors
graph_t const&  MyGraphBuilder::getGraph() const { return MyGraph; }
graph_t&        MyGraphBuilder::getGraph()       { return MyGraph; }
GraphMap const& MyGraphBuilder::getGraphMap() const { return MyGraphMap; }
GraphMap        MyGraphBuilder::getGraphMap()       { return MyGraphMap; }
//================================================================
// Mutators
void MyGraphBuilder::setGraph(graph_t YourGraph){
  MyGraph = YourGraph;
}
void MyGraphBuilder::setGraphMap(GraphMap YourGraph){
  MyGraphMap = YourGraph;
}
//================================================================
// Print Function
void MyGraphBuilder::printGraph()const{
  cout << "Number of Vertices is:" << num_vertices(MyGraph) << "\n";
  cout << "Number of Edges is:   " << num_edges(MyGraph)    << "\n";
  //To Print Graph
  //print_graph(MyGraph, get(boost::vertex_name, MyGraph), cout);

  // to print with edge weights:
  //  for (auto v : make_iterator_range(vertices(MyGraph))) {
  //      for (auto oe : make_iterator_range(out_edges(v, MyGraph))) {
  //          cout << "Edge " << oe << " weight " << boost::get(boost::edge_weight, MyGraph, oe) << "\n";

  //        }
  //    }
} // end of Print Function
//================================================================
