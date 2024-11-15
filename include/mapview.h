#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGraphicsView>
#include <QMouseEvent>
#include <QWheelEvent>
#include <iostream>
#include <QGraphicsItem>
#include "renderitem.h"
#include <iostream>
#include <QContextMenuEvent>

#define MAX_SCALE 0.06
#define MIN_SCALE 0.0001
#define ZOOM_STEP 1.2

#include <QMouseEvent>
#include <QGraphicsView>
#include <QObject>
#include <QWidget>

class MapView : public QGraphicsView
{
    Q_OBJECT

public:
    MapView(QWidget *parent = nullptr);

    enum userState{init, sourceSel, destSel, routing, search, null};

    ~MapView();

private:
    qreal m_scale;
    Multipolygon* m_selectedItem;
    bool m_isBuilding;
    userState m_state;
    QPoint m_pressPos;
    idType m_srcId;
    idType m_destId;

    void mousePressEvent(QMouseEvent *event);

    void contextMenuEvent(QContextMenuEvent *event);

    void wheelEvent(QWheelEvent *event);

signals:
    void setSource(Multipolygon *item);
    void setDest(Multipolygon *item);
    void searchPlace();
    void canecl(); //delete all temporary render item
    void makeRoute();

public slots:
    void changeToSearch();
    void changeToInit();
    void changeToRoute();
    userState getUserState();

};



#endif // MAPVIEW_H
