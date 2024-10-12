#ifndef MODELDATA_H
#define MODELDATA_H

#include <map>
#include <osmium/osm.hpp>
#include <string>
#include <modelDataStructure.h>
#include <boost/algorithm/string.hpp>
#include <osmium/index/map/flex_mem.hpp>
//#include <modelDataHandler.h>
#include <set>

using namespace std;

class modelData
{
    using locationType = osmium::index::map::FlexMem<osmium::unsigned_object_id_type, osmium::Location>;
    using tagPair = pair<string,string>;
    using idType = osmium::unsigned_object_id_type;
    locationType m_NodesLocation;
    map<idType, nodeData> m_NodeMap;
    map<idType, wayData> m_WayMap;
    map<idType, relationData> m_RelationMap;
    map<idType, vector<idType>> m_Multipolygon;
    vector<catagoryData> m_Amenity;
    set<string> m_AmenityType;

    friend class modelDataHandler;


    vector<string> findName(vector<tagPair> &tags);


    template <typename T>
    void buildCatagory(const T &map);


public:
    modelData(){}

    const osmium::Location getNodeLoaction(idType id);

    void clear();

    const nodeData getNodeData(idType id);


    const wayData getWayData(idType id);

    const relationData getRelationData(idType id);

    const map<idType,nodeData> getNodeMap();

    const map<idType,wayData> getWayMap();

    const map<idType,relationData> getRelationMap();


    const map<idType, vector<idType>> getMultipolygon();


    void buildAmenityCatagory();



    //return a vector of catagory data
    vector<catagoryData> searchAmenityByName(string name);


    bool isAmenityTypeExist(string name);


    vector<catagoryData> searchAmenityByType(string name);

};

#endif // MODELDATA_H
