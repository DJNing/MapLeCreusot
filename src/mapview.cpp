/*
* @author Deng Jianning
* @contact Jianning_Deng@etu.u-bourgogne.fr
* @date  30-11-2019
*/
#include "mapview.h"

void MapView::mousePressEvent(QMouseEvent *event)
{
    if(m_state != null)
    {
        if(event->button() == Qt::LeftButton)
            QGraphicsView::mousePressEvent(event);
        else if(event->button() == Qt::RightButton)
        {
            auto pos = event->pos();
            auto scenePos = mapToScene(pos);
    //            std::cout << "position from mapview is " << pos.x() << ", " << pos.y() << std::endl;
            auto item = this->scene()->itemAt(scenePos, QTransform());
            if(qgraphicsitem_cast<Multipolygon *>(item))
            {
                m_selectedItem = qgraphicsitem_cast<Multipolygon *>(item);
                if(m_selectedItem->getPolyType() == building)
                    m_isBuilding = true;
            }
            QGraphicsView::mousePressEvent(event);
        }
        else
            QGraphicsView::mousePressEvent(event);
    }
    else
        QGraphicsView::mousePressEvent(event);
}

void MapView::contextMenuEvent(QContextMenuEvent *event)
{
    //================= UI FSM ==============
    QMenu menu;
    if(m_state == init)
    {
        if(m_isBuilding)
        {
            menu.addAction("select as source place");
            menu.addAction("select as destiantion place");
            //======== this is set for debug ============
            menu.addAction("show detail");
            m_isBuilding = false;
        }
    }
    else if(m_state == sourceSel || m_state == destSel)
    {
        menu.addAction("cancel");
        if(m_isBuilding)
        {
            if(m_state == sourceSel)
                menu.addAction("select as destiantion place");
            else
                menu.addAction("select as source place");
        }
    }
    else if(m_state == search || m_state == routing)
        menu.addAction("cancel");
    if(m_state != search && m_state != routing && m_state != null)
        menu.addAction("search place");

    QAction *a = menu.exec(event->globalPos());
    if(a != nullptr)
    {
        //=========== this is set for debug ==========
        if(a->text() == "show detail")
        {
            std::cout << "the id of the building:" << m_selectedItem->getId() << std::endl;
        }
        if(a->text() == "select as source place")
        {
            emit setSource(m_selectedItem);
            if(m_state == destSel)
            {
                m_state = routing;
                emit makeRoute();
            }
            else if(m_state == init)
                m_state = sourceSel;
        }
        else if(a->text() == "select as destiantion place")
        {
            emit setDest(m_selectedItem);
            if(m_state == sourceSel)
            {
                m_state = routing;
                emit makeRoute();
            }
            else if(m_state == init)
                m_state = destSel;
        }
        else if(a->text() == "search place")
        {
            emit searchPlace();
//            m_state = init;
            // reset the state to init, state will be changed to search when user input is valid
            // state changed in slot: changeToSearch
        }
        else if(a->text() == "cancel")
        {
            emit canecl();
            changeToInit();
//            m_srcId = 0;
//            m_destId = 0;
//            m_state = init;
//            m_isBuilding = false;
        }
    }

    //============ end of state control ==============

    m_selectedItem = nullptr;
}

void MapView::wheelEvent(QWheelEvent *event)
{
    if(event->angleDelta().y() > 0)
    {
        if(m_scale < MAX_SCALE)
        {
            std::cout << m_scale << std::endl;
            this->scale(ZOOM_STEP, ZOOM_STEP);
            m_scale *= ZOOM_STEP;
        }
    }
    else if(event->angleDelta().y() < 0)
    {
        if(m_scale >= MIN_SCALE)
        {
            std::cout << m_scale << std::endl;
            this->scale(1/ZOOM_STEP, 1/ZOOM_STEP);
            m_scale *= 1/ZOOM_STEP;
        }
    }
}

void MapView::changeToSearch()
{
    m_state = search;
    std::cout << "state changed to search" << std::endl;
}

void MapView::changeToInit()
{
    //set state to init when file is loaded
    m_srcId = 0;
    m_destId = 0;
    m_state = init;
    m_isBuilding = false;
    std::cout << "state changed to init" << std::endl;
}

void MapView::changeToRoute()
{
    m_state = routing;
}

MapView::userState MapView::getUserState()
{
    return m_state;
}

MapView::MapView(QWidget *parent) : QGraphicsView(parent)
{
    m_scale = 0.001;
    m_isBuilding = false;
    m_pressPos = QPoint(0,0);
    m_state = null;
}

MapView::~MapView(){}
