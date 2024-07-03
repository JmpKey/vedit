#include "paintscene.h"

paintscene::paintscene(QObject* _parent) : QGraphicsScene(_parent) {}

paintscene::~paintscene() {}

void paintscene::mousePressEvent(QGraphicsSceneMouseEvent* _event)
{
    QGraphicsScene::mousePressEvent(_event);

    if(arrow_tool) {
        addEllipse(_event->scenePos().x() - 5,
               _event->scenePos().y() - 5,
               10,
               10,
               QPen(Qt::NoPen),
               QBrush(Qt::red));
        previousPoint = _event->scenePos();
    }
    else { QGraphicsScene::mousePressEvent(_event); }
}

void paintscene::mouseMoveEvent(QGraphicsSceneMouseEvent* _event)
{
    QGraphicsScene::mouseMoveEvent(_event);

    if(arrow_tool) {
        addLine(previousPoint.x(),
            previousPoint.y(),
            _event->scenePos().x(),
            _event->scenePos().y(),
            QPen(Qt::red,10,Qt::SolidLine,Qt::RoundCap));
        previousPoint = _event->scenePos();
    }
    else { QGraphicsScene::mouseMoveEvent(_event); }
}

void paintscene::mouseReleaseEvent(QGraphicsSceneMouseEvent* _event)
{
    if(arrow_tool) {
        QGraphicsScene::mouseReleaseEvent(_event);
        previousPoint = _event->scenePos();
    }
    else { QGraphicsScene::mouseReleaseEvent(_event);
    }
}

QGraphicsItem* paintscene::createItemGroup(const QList<QGraphicsItem*>& _items)
{
    QGraphicsItem* group = QGraphicsScene::createItemGroup(_items);

    foreach(QGraphicsItem* item, _items) {
        item->setFlag(QGraphicsItem::ItemIsMovable, false);
        item->setFlag(QGraphicsItem::ItemIsSelectable, false);
    }

    return group;
}

bool paintscene::getArrowTool() { return arrow_tool; }
void paintscene::setArrowTool(bool _arrow_tool) { arrow_tool = _arrow_tool; }
