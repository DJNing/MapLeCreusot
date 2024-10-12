#include "modeldata.h"


vector<string> modelData::findName(vector<modelData::tagPair> &tags)
{
    vector<string> tempVec;
    for(auto it = tags.begin(); it != tags.end(); it ++)
    {
        if(it->first.find("name") != string::npos)
        {
            tempVec.emplace_back(it->second);
        }
    }
    for(auto it = tempVec.begin(); it != tempVec.end(); it ++)
    {
        boost::algorithm::to_lower(*it);
    }
    return tempVec;
}

const osmium::Location modelData::getNodeLoaction(modelData::idType id)
{
    return m_NodesLocation.get(id);
}

void modelData::clear()
{
    m_NodeMap.clear();
    m_NodesLocation.clear();
    m_Amenity.clear();
    m_WayMap.clear();
    m_RelationMap.clear();
    m_Multipolygon.clear();
    m_AmenityType.clear();
}

const nodeData modelData::getNodeData(modelData::idType id)
{
    return m_NodeMap.at(id);
}

const wayData modelData::getWayData(modelData::idType id)
{
    return m_WayMap.at(id);
}

const relationData modelData::getRelationData(modelData::idType id)
{
    return m_RelationMap.at(id);
}

const map<modelData::idType, nodeData> modelData::getNodeMap()
{
    return m_NodeMap;
}

const map<modelData::idType, wayData> modelData::getWayMap()
{
    return m_WayMap;
}

const map<modelData::idType, relationData> modelData::getRelationMap()
{
    return m_RelationMap;
}

const map<modelData::idType, vector<modelData::idType> > modelData::getMultipolygon()
{
    return m_Multipolygon;
}

void modelData::buildAmenityCatagory()
{
    buildCatagory(m_NodeMap);
    buildCatagory(m_WayMap);
}

vector<catagoryData> modelData::searchAmenityByName(string name)
{
    vector<catagoryData> result;
    for(auto it = m_Amenity.begin(); it != m_Amenity.end(); it ++)
    {
        for(auto it2 = it->name.begin(); it2 != it->name.end(); it2 ++)
        {
            if(it2->find(name) != string::npos)
            {
                result.emplace_back(*it);
                break;
            }
        }
    }
    return result;
}

bool modelData::isAmenityTypeExist(string name)
{
    for(auto it = m_AmenityType.begin(); it != m_AmenityType.end(); it ++)
    {
        if(it->find(name) != string::npos)
            return true;
    }
    return false;

}

vector<catagoryData> modelData::searchAmenityByType(string name)
{
    vector<catagoryData> result;
    for(auto it = m_Amenity.begin(); it != m_Amenity.end(); it ++)
    {
        if(it->type.find(name) != string::npos)
            result.emplace_back(*it);
    }
    return result;
}

template<typename T>
void modelData::buildCatagory(const T &map)
{
    //loop over node map and way map
    for(auto data = map.begin(); data != map.end(); data ++)
    {
        vector<tagPair> tags = data->second.tagList;
        if(tags.size() != 0)
        {
            catagoryData temp;
            for(auto tag = tags.begin(); tag != tags.end(); tag ++)
            {
                if(tag->first == "amenity" || tag->first == "shop" || data->second.osmType == osmium::item_type::node)
                {
                    temp.itemType = data->second.osmType;
                    temp.type = tag->second;
                    boost::algorithm::to_lower(temp.type);
                    temp.name = findName(tags);
                    temp.id = data->first;
                    m_Amenity.emplace_back(temp);
                    m_AmenityType.insert(temp.type);
                    break;
                }
            }
        }
    }
}
