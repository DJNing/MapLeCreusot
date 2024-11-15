#include <shortpath.h>

// Default Constructor
ShortPath::ShortPath()
{
  cout << "Dear User careful, This is Empty Path !" << endl;
}
// Parameters Constructor
ShortPath::ShortPath(idType Source1, idType Destination1, Model mModel)
{
  setSource(Source1);
  setDestination(Destination1);
  setModel(mModel);
  MyGraphBuilder builder(getModel());
  builder.generateGraph();
  MyAlgorithm algo(builder.getGraph(), builder.getGraphMap(), Source_ID);
  if (algo.getFlag())
  {
    setMyPath(algo.getShortPath(builder.getGraphMap(), Destination_ID));
  }
  else
    cout << "please enter valid node\t" << endl;
}
ShortPath::~ShortPath() {} // Destructor

Model ShortPath::getModel() { return OurModel; }
idType ShortPath::getSource() { return Source_ID; }
idType ShortPath::getDestination() { return Destination_ID; }
Path ShortPath::getYourPath() { return mypath; }
//----------------------------------------------------------------
// Mutator
void ShortPath::setMyPath(Path ThePath) { mypath = ThePath; }
void ShortPath::setSource(idType Src) { Source_ID = Src; }
void ShortPath::setDestination(idType Des) { Destination_ID = Des; }
void ShortPath::setModel(Model YourModel) { OurModel = YourModel; }
//---------------------------------------------------------------------
// Print Function
void ShortPath::printMyPath()
{
  for (vector<idType>::iterator it = mypath.begin(); it != mypath.end(); ++it)
  {
    cout << "MyPath is \t" << *it << endl;
  }
}
