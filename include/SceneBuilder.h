#ifndef SCENEBUILDER_H
#define SCENEBUILDER_H

#include <QGraphicsScene>
#include <string>
#include <vector>
#include "model.h"
#include "modeldata.h"
#include <map>
#include <vector>
#include <osmium/osm.hpp>
#include <QGraphicsView>
#include <QPen>
#include <vector>
#include "RenderEnum.h"
#include "renderitem.h"
#include <QLine>
#include "projection.h"
#include <QGraphicsItem>
#include <QString>
#include <QObject>
#include <QFont>

using namespace std;

class SceneBuilder : public QObject
{
    Q_OBJECT

    QGraphicsScene *m_scene;
    //    QGraphicsView *veiw;
    Model *m_model;
    vector<Multipolygon *> m_polygonList;
    vector<Road *> m_RoadList;
    vector<QGraphicsTextItem *> m_textContainer;
    vector<QGraphicsEllipseItem *> m_Point;
    Road *m_route;
    Pin *m_source;
    Pin *m_dest;
    vector<Pin *> m_pinContainer; // a container for pin object, release them when cancel is triggered

    void buildMutipolygon(wayData way, idType wayId);

    void buildRoad(wayData way, idType wayId);

    void getBoundingRectCenter();

    template <typename T>
    void deleteContainer(T &vp);

    void drawText(string text, QPointF pos);

    void drawPin(idType, QPointF);

public:
    SceneBuilder(Model *model);

    ~SceneBuilder();

    void clear();

    void addAllItem();

    void addPolyItem();

    QGraphicsScene *getScene();

    void addRoadItem();

    void drawRoute(std::vector<idType> refList);
    void drawPointText();

public slots:
    void setSource(Multipolygon *item);
    void setDest(Multipolygon *item);

    bool searchPlace(QString name);
    void cancel();
    void getSrcDestId();
    void slotDrawRoute(vector<idType> route);

signals:
    void routeSrcAndDest(idType src, idType dest);
    void routeFailed();
};

#endif // RENDER_H
