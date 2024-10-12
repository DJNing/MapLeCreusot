#ifndef MODELDATASTRUCTURE_H
#define MODELDATASTRUCTURE_H

#include <string>
#include <vector>
#include "osmium/osm.hpp"
#include <Qt>
#include "RenderEnum.h"

using namespace std;
using tagPair = pair<string,string>;

using idType = osmium::unsigned_object_id_type;

struct relationMember
{
    osmium::item_type type;
    idType ref;
    string role;
};

struct relationData
{
    vector<relationMember> memberList;
    vector<tagPair> tagList;
    bool isPolygon = false;
    osmium::item_type osmType = osmium::item_type::relation;
};

struct nodeData
{
    vector<tagPair> tagList;
    osmium::item_type osmType = osmium::item_type::node;
};

struct wayData
{
    vector<idType> nodeRefList;
    vector<tagPair> tagList;
    bool isRelation = false;
    bool isClosed = false;
    bool isPolygon = false;
    polygonType pType = invalid;
    roadType rType = Invalid;
    osmium::item_type osmType = osmium::item_type::way;
};

struct catagoryData
{
    idType id;
    osmium::item_type itemType;
    vector<string> name;
    string type;
};

#endif // MODELDATASTRUCTURE_H
