#include "SceneBuilder.h"

// created a multi-polygon from the "way" data in osm, they are basically buildings or aeras
void SceneBuilder::buildMutipolygon(wayData way, idType wayId)
{
    QPolygonF polygon;
    auto nodeRefList = way.nodeRefList;
    // build a multi-polygon from the OSM way data
    for(vector<idType>::iterator it = nodeRefList.begin();it != nodeRefList.end();it++)
    {
        auto point = projection(m_model->getNodeLoaction(*(it)).lon(), m_model->getNodeLoaction(*(it)).lat());
        polygon << point;
    }
    Multipolygon *polyItem = new Multipolygon;

    polyItem->setId(wayId);
    polyItem->setPolygon(polygon);
    polyItem->setPolyType(way.pType);
    m_polygonList.emplace_back(polyItem);
    m_scene->addItem(polyItem);
}

void SceneBuilder::buildRoad(wayData way, idType wayId)
{
    QPolygonF polyLine;
    auto nodeRefList = way.nodeRefList;
    // build a multi-polygon from the OSM way data
    for(vector<idType>::iterator it = nodeRefList.begin();it != nodeRefList.end();it++)
    {
        auto point = projection(m_model->getNodeLoaction(*(it)).lon(), m_model->getNodeLoaction(*(it)).lat());
        polyLine << point;
    }
    Road *roadItem = new Road;
    roadItem->setId(wayId);
    roadItem->setPenStyle(way.rType);
    roadItem->setPolygon(polyLine);
    m_RoadList.emplace_back(roadItem);
    m_scene->addItem(roadItem);
}


SceneBuilder::SceneBuilder(Model *model)
{
    m_scene = new QGraphicsScene;
    m_model = model;
    m_route = nullptr;
    m_dest = nullptr;
    m_source = nullptr;

}

SceneBuilder::~SceneBuilder()
{
    delete m_scene;
}

void SceneBuilder::clear()
{
    m_scene->clear();
}

void SceneBuilder::addAllItem()
{
    addPolyItem();
    addRoadItem();
}

void SceneBuilder::addPolyItem()
{
    auto wayMap = m_model->getWayMap();
    map<idType, wayData>::iterator it;
    for(it = wayMap.begin(); it != wayMap.end(); it++)
    {
        auto way = it -> second;
        if(way.isPolygon)
            buildMutipolygon(way, it -> first );
    }
}

QGraphicsScene* SceneBuilder::getScene()
{
    return m_scene;
}

void SceneBuilder::addRoadItem()
{
    auto wayMap = m_model->getWayMap();
    map<idType, wayData>::iterator it;
    for(it = wayMap.begin(); it != wayMap.end(); it++)
    {
        auto way = it -> second;
        if(!way.isPolygon)
            buildRoad(way, it->first);
    }
}

void SceneBuilder::drawRoute(std::vector<idType> refList)
{
    if(m_route == nullptr)
        m_route = new Road;
    else
        m_route->setVisible(true);
    QPolygonF polyLine;
    for(vector<idType>::iterator it = refList.begin();it != refList.end();it++)
    {
//            std::cout << it << std::endl;
        auto point = projection(m_model->getNodeLoaction(*(it)).lon(), m_model->getNodeLoaction(*(it)).lat());
        polyLine << point;
    }

    m_route->setPolygon(polyLine);
    // temporary z-value to make sure the route stays at the top of the view
    m_route->setPenStyle(Route);
    m_route->setZValue(200);
    m_scene->addItem(m_route);
}

void SceneBuilder::drawPointText()
{
    auto nodeMap = m_model->getNodeMap();
    for(auto it = nodeMap.begin();it != nodeMap.end();it ++)
    {
        auto node = it -> second;
        auto tagList = node.tagList;
        for(const auto &tag:tagList)
        {
            if(tag.first == "name")
            {
                QGraphicsTextItem *Text = new QGraphicsTextItem;
//                    QGraphicsEllipseItem *Point = new QGraphicsEllipseItem;
                string name = tag.second;
                Text->setPlainText(QString::fromStdString(name));
                auto geoPos = m_model->getNodeLoaction(it -> first);
                auto pos = projection(geoPos.lon(), geoPos.lat());
                Text->setPos(pos);
                Text->setZValue(100);
                m_textContainer.emplace_back(Text);
                m_scene->addItem(Text);
            }
        }
    }
}

//void SceneBuilder::cancelRoute()
//{
//    m_route->setVisible(false);
//}

void SceneBuilder::setSource(Multipolygon *item)
{

    QPointF centerPos = item->boundingRect().center();

    m_source = new Pin(item->getId(), Pin::pinType::source);
    m_source->setPos(centerPos);

    auto rect = m_scene->sceneRect();

    m_scene->addItem(m_source);

    m_scene->setSceneRect(rect);
    std::cout << "setSource slot connected" << std::endl;
}

void SceneBuilder::setDest(Multipolygon *item)
{
    QPointF centerPos = item->boundingRect().center();

    m_dest = new Pin(item->getId(), Pin::pinType::dest);
    m_dest->setPos(centerPos);

    auto rect = m_scene->sceneRect();

    m_scene->addItem(m_dest);

    m_scene->setSceneRect(rect);
    std::cout << "setDestination slot connected" << std::endl;
}

bool SceneBuilder::searchPlace(QString name)
{
    std::cout << "searchPlace slot connected" << std::endl;

    // search the name in the m_model
    // get a list of catagory data
    string key = name.toLower().toStdString();

    vector<catagoryData> resultType;

    if(m_model->isAmenityTypeExist(key))
    {
        resultType = m_model->searchAmenityByType(key);
    }

    vector<catagoryData> resultName = m_model->searchAmenityByName(key);

    vector<catagoryData> result = resultType;

    result.insert(result.end(), resultName.begin(), resultName.end());

    // check if it is empty
    if(result.size() == 0)
        return false;
    else
    {
        for(auto it = result.begin(); it != result.end(); it ++)
        {
            if(it->itemType == osmium::item_type::node)
            {
                auto geoPos = this->m_model->getNodeLoaction(it->id);
                auto pos = projection(geoPos.lon(), geoPos.lat());
                auto item = m_scene->itemAt(pos, QTransform());
                if(qgraphicsitem_cast<Multipolygon *>(item))
                {
                    auto casted = qgraphicsitem_cast<Multipolygon *>(item);

                    // avoid drawing multiple text and pin
                    auto test = m_scene->itemAt(item->boundingRect().center(), QTransform());
                    if(qgraphicsitem_cast<Multipolygon *>(test))
                    {
                        drawPin(casted->getId(), casted->boundingRect().center());
                        if(it->name.size() != 0)
                            drawText(it->name[0], item->boundingRect().center());
                        else if(it->type.size() != 0)
                            drawText(it->type, item->boundingRect().center());
                    }
                }
            }
            else
            {
                auto way = this->m_model->getWayData(it->id);
                QPolygonF tempPoly;
                auto nodeRefList = way.nodeRefList;
                for(vector<idType>::iterator it = nodeRefList.begin();it != nodeRefList.end();it++)
                {
                    auto point = projection(m_model->getNodeLoaction(*(it)));
                    tempPoly << point;
                }

                auto test = m_scene->itemAt(tempPoly.boundingRect().center(), QTransform());
                if(qgraphicsitem_cast<Multipolygon *>(test))
                {
                    drawPin(it->id, tempPoly.boundingRect().center());
                    if(it->name.size() != 0)
                        drawText(it->name[0], tempPoly.boundingRect().center());
                    else if(it->type.size() != 0)
                        drawText(it->type, tempPoly.boundingRect().center());
                }
                //drawText();
            }
        }
        return true;
    }


}

void SceneBuilder::cancel()
{
    if(m_source != nullptr)
    {
        delete m_source;
        m_source = nullptr;
    }
    if(m_dest != nullptr)
    {
        delete m_dest;
        m_dest = nullptr;
    }
    deleteContainer(m_pinContainer);
    deleteContainer(m_textContainer);
    if(m_route != nullptr)
    {
        delete m_route;
        m_route = nullptr;
    }

    std::cout << "cancel slot connected" << std::endl;
}

void SceneBuilder::getSrcDestId()
{
    if(m_source == nullptr || m_dest == nullptr)
        emit routeFailed();
    else
    {
        emit routeSrcAndDest(m_source->getId(), m_dest->getId());
    }
}

void SceneBuilder::slotDrawRoute(vector<idType> route)
{
    this->drawRoute(route);
}

template<typename T>
void SceneBuilder::deleteContainer(T &vp)
{
    for(auto it = vp.begin(); it != vp.end(); it ++)
    {
        if(*it != nullptr)
        {
            delete (*it);
        }
    }
    vp.clear();
}

void SceneBuilder::drawText(string text, QPointF pos)
{
    QGraphicsTextItem *temp = new QGraphicsTextItem;
    temp->setPlainText(QString::fromStdString(text));
    temp->setPos(pos);

    QFont f;
    f.setPointSizeF(16.0);
    temp->setFont(f);
    temp->setZValue(50);
    temp->setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
    m_scene->addItem(temp);
    m_textContainer.emplace_back(temp);
}

void SceneBuilder::drawPin(idType id, QPointF pos)
{
    Pin *temp = new Pin(id);

    temp->setPos(pos);
    auto rect = m_scene->sceneRect();

    m_scene->addItem(temp);
    m_pinContainer.emplace_back(temp);

    m_scene->setSceneRect(rect);
}





