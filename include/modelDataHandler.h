#ifndef MODELDATAHANDLER_H
#define MODELDATAHANDLER_H

#include <osmium/osm.hpp>
#include <osmium/index/map/flex_mem.hpp>
#include <osmium/handler.hpp>
#include <string>
#include <vector>
#include <iostream>
#include <modelDataStructure.h>
#include <modeldata.h>
#include "RenderEnum.h"

class modelDataHandler : public osmium::handler::Handler
{
//    using namespace osmium;
    using locationType = osmium::index::map::FlexMem<osmium::unsigned_object_id_type, osmium::Location>;
    using tagPair = std::pair<std::string,std::string>;
    using idType = osmium::unsigned_object_id_type;

    void getTags(std::vector<tagPair>& tagStore, const osmium::TagList& tagList)
    {
        for(const auto& tag:tagList)
        {
            auto tempKey = tag.key();
            auto tempValue = tag.value();
            tagStore.emplace_back(std::make_pair(tempKey,tempValue));
        }
    }

    // using tons of if-else to filter the data, it's stupid but it works
    void getTagsAndType(std::vector<tagPair>& tagStore, const osmium::TagList& tagList, wayData& wayD)
    {
        bool nonPolyFlag = false;
        for(const auto& tag:tagList)
        {
            string tempKey = tag.key();
            string tempValue = tag.value();

            if(tempKey == "highway")
            {
                nonPolyFlag = true;
                wayD.isPolygon = false;
                if (tempValue == "footway")
                    wayD.rType = Footway;
                else if(tempValue == "motorway")
                    wayD.rType = Motorway;
                else if(tempValue == "trunk")
                    wayD.rType = Trunk;
                else if(tempValue == "primary")
                    wayD.rType = Primary;
                else if(tempValue == "secondary")
                    wayD.rType = Secondary;
                else if (tempValue == "tertiary")
                    wayD.rType = Tertiary;
                else if (tempValue == "service")
                    wayD.rType = Service;
                else if (tempValue == "residential")
                    wayD.rType = Residential;
                else if (tempValue == "unclassified")
                    wayD.rType = Unclassified;
                else
                    wayD.rType = Invalid;
            }
            else if(tempKey == "railway")
            {
                wayD.isPolygon = false;
                nonPolyFlag = true;
                wayD.rType = Railway;
            }
            else if(tempKey == "boundary" || tempKey == "barrier")
            {
                wayD.isPolygon = false;
                nonPolyFlag = true;
            }
            else if(wayD.isClosed)
            {
                if(!nonPolyFlag)
                    wayD.isPolygon = true;
                if(tempKey == "building" || tempKey == "tourism" || tempKey == "man_made" || tempKey == "area" || tempKey == "old_building")
                    wayD.pType = building;
                else if(tempKey == "leisure" || tempKey == "amenity")
                    wayD.pType = leisure;
                else if(tempKey == "waterway")
                    wayD.pType = water;
                else if(tempKey == "landuse")
                {
                    if(tempValue == "water")
                        wayD.pType = water;
                    else if(tempValue == "grass" || tempValue == "grassland" || tempValue == "wood" || tempValue == "heath" || tempValue == "farmland")
                        wayD.pType = grass;
                    else if(tempValue == "industrial")
                        wayD.pType = industrial;
                    else if(tempValue == "residential")
                        wayD.pType = residential;
                    else if(tempValue == "retail")
                        wayD.pType = commercial;
                    else if(tempValue == "meadow" || tempValue == "forest")
                        wayD.pType = forest;
                }
                else if(tempKey == "natural")
                {
                    if(tempValue == "water")
                        wayD.pType = water;
                    if(tempValue == "scrub" || tempValue == "heath")
                        wayD.pType = grass;
                }
            }
            tagStore.emplace_back(std::make_pair(tempKey,tempValue));
        }
    }
    modelData* m_modelData;

public:
    modelDataHandler(modelData* Data)
    {
        m_modelData = Data;
    }

    //the following code is in the example to extract nodes, ways, and relations. just read the name
    void node(const osmium::Node& node)
    {
        const int64_t id = node.id();
        if (id >= 0)    //now only support unsign id
        {
            m_modelData->m_NodesLocation.set(static_cast<idType>(id), node.location());
            if(!node.tags().empty())
            {
                nodeData temp;
                getTags(temp.tagList, node.tags());
                m_modelData -> m_NodeMap[static_cast<idType>(id)] = temp;
//                std::cout << temp.tagList[0].first << std::endl;
            }
        }
    }

    void way(const osmium::Way &ways)
    {
        const int64_t id = ways.id();
        if (id >= 0)    //now only support unsign id
        {
            wayData temp;

            //get node reference list
            for(const auto node:ways.nodes())
                temp.nodeRefList.emplace_back(node.ref());
            //get tag list


            temp.isClosed = ways.is_closed();
            temp.isRelation = false;

//            getTags(temp.tagList, ways.tags());

            getTagsAndType(temp.tagList, ways.tags(), temp);

            m_modelData->m_WayMap[static_cast<idType>(id)] = temp;
            if(ways.is_closed())
                m_modelData->m_Multipolygon[static_cast<idType>(id)] = temp.nodeRefList;
        }
    }

    void relation(const osmium::Relation& relations)
    {
        const int64_t id = relations.id();
        if(id >= 0)
        {
            relationData tempData;

            // store memberList
            const osmium::RelationMemberList& rml = relations.members();
            for(const osmium::RelationMember& rm : rml)
            {
                relationMember tempMem;
                tempMem.ref = rm.ref();
                tempMem.type = rm.type();
                tempMem.role = rm.role();
                tempData.memberList.emplace_back(tempMem);
            }
            for(const auto& tag:relations.tags())
            {
                auto tempKey = tag.key();
                auto tempValue = tag.value();
                tempData.tagList.emplace_back(std::make_pair(tempKey,tempValue));
            }

            m_modelData->m_RelationMap[static_cast<idType>(id)] = tempData;

        }

    }

};

#endif // MODELDATAHANDLER_H
