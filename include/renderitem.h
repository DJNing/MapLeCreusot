#ifndef RENDERITEM_H
#define RENDERITEM_H

#include <QGraphicsPolygonItem>
#include <QGraphicsPathItem>
#include <QGraphicsLayoutItem>
#include <QPen>
#include <QPainter>
#include <vector>
#include <string>
#include "modelDataHandler.h"
#include <QMouseEvent>
#include <QGraphicsView>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneContextMenuEvent>
#include <QMenu>
#include <iostream>

// use style reference from P0267_RefImpl, but not exactly the same
static QColor getPolygonColor(polygonType type)
{
    switch (type) {
    case building:
        return QColor(255, 190, 190);
    case water:
        return QColor(0, 201, 215);
    case grass:
        return QColor(197, 236, 148);
    case forest:
        return QColor(46, 139, 87);
    case industrial:
        return QColor(223, 197, 220);
    case leisure:
        return QColor(189, 252, 193);
    case railway:
        return QColor(193, 193, 193);
    case commercial:
        return QColor(233, 195, 196);
    case residential:
        return QColor(209, 209, 209);
    default:
        return QColor(100, 100, 100);

    }
}

static QColor getPathColor(roadType type)
{
    switch(type) {
    case Route:         return QColor{255, 0, 0};
        case Railway:       return QColor{100, 100, 100};
        case Motorway:      return QColor{226, 122, 143};
        case Trunk:         return QColor{245, 161, 136};
        case Primary:       return QColor{249, 207, 144};
        case Secondary:     return QColor{244, 251, 173};
        case Tertiary:      return QColor{244, 251, 173};
        case Residential:   return QColor{254, 254, 254};
        case Service:       return QColor{254, 254, 254};
        case Footway:       return QColor{241, 106, 96};
        case Unclassified:  return QColor{254, 254, 254};
        default:            return QColor(190, 190, 190);
    }
}

static float getPathWidth(roadType type)
{
    switch( type ) {
        case Route:         return 7.f;
        case Railway:       return 6.f;
        case Motorway:      return 6.f;
        case Trunk:         return 6.f;
        case Primary:       return 5.f;
        case Secondary:     return 5.f;
        case Tertiary:      return 4.f;
        case Residential:   return 2.5f;
        case Unclassified:  return 2.5f;
        case Service:       return 1.f;
        case Footway:       return 0.f;
        default:            return 1.f;
    }
}

class Multipolygon : public QGraphicsItem
{

    polygonType m_PolygonType;    //this will determin the z-value
    QColor m_brushColor;
    idType m_wayId;
    QPolygonF m_poly;

public:
    Multipolygon(){}
    ~Multipolygon(){}
    enum { Type = UserType + 1 };

    int type() const
    {
       // Enable the use of qgraphicsitem_cast with this item.
       return Type;
    }
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override
    {
        Q_UNUSED(option);
        Q_UNUSED(widget);
        painter->setBrush(m_brushColor);
        painter->drawPolygon(this->m_poly);
    }

    QRectF boundingRect() const
    {
        return m_poly.boundingRect();
    }

    void setPolyType(polygonType type)
    {

        m_PolygonType = type;
        m_brushColor = getPolygonColor(m_PolygonType);

        setZValue(type);
    }
    void setId(idType id)
    {
        m_wayId = id;
    }

    idType getId()
    {
        return m_wayId;
    }
    void setPolygon(QPolygonF poly)
    {
        m_poly = poly;
    }

    polygonType getPolyType()
    {
        return m_PolygonType;
    }

};

class Road : public QGraphicsPolygonItem
{
    roadType m_rtype;
    QColor m_RoadStyle;
    idType m_wayId;
    QPen m_pen;

public:
    Road(){}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
    QWidget *widget) override
    {

        Q_UNUSED(option)
        Q_UNUSED(widget)
        m_pen.setCapStyle(Qt::RoundCap);
        painter->setPen(m_pen);
        painter->drawPolyline(this->polygon());
    }
    idType getId()
    {
        return m_wayId;
    }
    void setId(idType id)
    {
        m_wayId = id;
    }

    void setPenStyle(roadType rType)
    {
        m_pen.setBrush(getPathColor(rType));
        m_pen.setWidth(getPathWidth(rType)+3.0);
        setZValue(leisure + static_cast<int>(rType) * 0.1);
    }

};

// this class is used to draw a pin on the selected item
class Pin : public QGraphicsItem
{
    QPolygonF m_poly;
    QPointF m_bias;
    QColor m_brushColor;
    idType m_Id;

public:

    enum pinType {source, dest, search};
    Pin(idType wayID, pinType type = search)
    {

        m_poly << QPointF(-5,-10)<< QPointF(5,-10) << QPointF(5,-5)
               << QPointF(0,0) << QPointF(-5,-5);
        m_bias = QPointF(0,0);
        m_Id = wayID;
        switch(type)
        {
        case search:
            m_brushColor = QColor(10, 150, 10);
            break;
        case source:
            m_brushColor = QColor(50, 150, 255);
            break;
        case dest:
            m_brushColor = QColor(255, 70, 70);
            break;
        default:
            m_brushColor = QColor(50, 150, 255);
        }
        this->setZValue(10);
        this->setScale(2.5);
        QGraphicsItem::setFlag(QGraphicsItem::ItemIgnoresTransformations,true);
    }
    ~Pin() override{}

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget) override
    {
        Q_UNUSED(option)
        Q_UNUSED(widget)
        painter->setBrush(m_brushColor);
        painter->drawPolygon(this->m_poly);

    }

    void setBias(QPointF bias)
    {
        m_bias = bias;

//        this->setPos(0,0);
        for(auto it = m_poly.begin(); it != m_poly.end(); it++)
            *(it) += m_bias;
        return;
    }

    QRectF boundingRect() const override
    {
        return m_poly.boundingRect();
    }

    idType getId()
    {
        return m_Id;
    }

};


#endif // RENDERITEM_H
