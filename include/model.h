#ifndef MODEL_H
#define MODEL_H

#include <modelDataStructure.h>
#include <modelDataHandler.h>
#include <osmium/osm.hpp>
#include <osmium/index/map/flex_mem.hpp>
#include <osmium/io/pbf_input.hpp>
//#include <osmium/io/xml_input.hpp>
#include <osmium/visitor.hpp>
#include <modeldata.h>
#include <boost/algorithm/string.hpp>
#include <QPoint>
#include "projection.h"
//class Model
//{
//public:
//    Model(){}
//};

class Model
{

    using locationType = osmium::index::map::FlexMem<osmium::unsigned_object_id_type, osmium::Location>;
    using tagPair = std::pair<std::string,std::string>;
    using idType = osmium::unsigned_object_id_type;

    bool m_isFileLoaded;
    string m_filePath;

    QPointF m_bottomLeft, m_topRight;

    modelData *m_Data;


    void loadFile()
    {
        if(m_isFileLoaded)
        {
            m_Data->clear();
        }
        std::cout << "loading file" << std::endl;
        osmium::io::File inputFile(m_filePath);
        modelDataHandler handler(m_Data);
        osmium::io::Reader reader(inputFile, osmium::io::read_meta::no);


        osmium::io::Header header = reader.header();

        m_bottomLeft = projection(header.box().bottom_left());
        m_topRight = projection(header.box().top_right());

        std::cout << header.box().bottom_left().x() << "::"
                  << header.box().top_right().y()
                  << std::endl;

        osmium::apply(reader, handler);
        reader.close();
    }


public:
    Model()
    {
        m_Data = new modelData;
        m_isFileLoaded = false;
        m_filePath = "";
    }


    // everytime you set a new path to a file, the model will re-load the data
    void setFilePath(string filePath){
        if(filePath != m_filePath)
        {
            m_filePath = filePath;
            loadFile();
            m_isFileLoaded = true;
        }
    }

    const osmium::Location getNodeLoaction(idType id)
    {
        return m_Data->getNodeLoaction(id);
    }

    const nodeData getNodeData(idType id)
    {
        return m_Data->getNodeData(id);
    }

    const wayData getWayData(idType id)
    {
        return m_Data->getWayData(id);
    }

    const relationData getRelationData(idType id)
    {
        return m_Data->getRelationData(id);
    }

    const map<idType,nodeData> getNodeMap()
    {
        return m_Data->getNodeMap();
    }

    const map<idType,wayData> getWayMap()
    {
        return m_Data->getWayMap();
    }

    const map<idType,relationData> getRelationMap()
    {
        return m_Data->getRelationMap();
    }

    const map<idType,vector<idType>> getMPMap()
    {
        return m_Data->getMultipolygon();
    }

    void buildAmenityCatalog()
    {
        m_Data->buildAmenityCatagory();
    }

    vector<catagoryData> searchAmenityByName(string name)
    {
        return m_Data->searchAmenityByName(name);
    }

    vector<catagoryData> searchAmenityByType(string name)
    {
        return m_Data->searchAmenityByType(name);
    }

    bool isAmenityTypeExist(string name)
    {
        return m_Data->isAmenityTypeExist(name);
    }

    QPointF getCenter()
    {

        return (m_bottomLeft + m_topRight)/2;
    }


};

#endif // MODEL_H
