#ifndef PAINTSCENE_H
#define PAINTSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QTimer>
#include <QDebug>
#include <QGraphicsItem>

#include "moveitem.h"

class paintscene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit paintscene(QObject* _parent = 0);
    ~paintscene();

private:
    QPointF previousPoint;
    bool arrow_tool = true;

private:
    void mousePressEvent(QGraphicsSceneMouseEvent* _event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent* _event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* _event);

public:
    bool getArrowTool();
    void setArrowTool(bool _arrow_tool);
    QGraphicsItem* createItemGroup(const QList<QGraphicsItem*>& _items);
};

#endif // PAINTSCENE_H
