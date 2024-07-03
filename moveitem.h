#ifndef MOVEITEM_H
#define MOVEITEM_H

#include <QObject>
#include <QGraphicsItem>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QCursor>
#include <QtWidgets>

#include "paintscene.h"

class MoveItem : public QObject, public QGraphicsItem
{
    Q_OBJECT
public:
    explicit MoveItem(QObject* _parent = 0);
    ~MoveItem();

private:
    QRectF boundingRect() const;
    void paint(QPainter* _painter, const QStyleOptionGraphicsItem* _option, QWidget* _widget);

    void scaleUp();
    void scaleDown();
    void rotate();
    void stretchXP();
    void stretchYP();
    void stretchXM();
    void stretchYM();
    void keyPressEvent(QKeyEvent* _event);

private:
    qreal scale = 1.0;
    QColor color = Qt::green;
    char arrow_shape = 'r';
    qreal rotation = 1.0;
    qreal scaleX = 1.0;
    qreal scaleY = 1.0;

public:
    void mouseMoveEvent(QGraphicsSceneMouseEvent* _event);
    void mousePressEvent(QGraphicsSceneMouseEvent* _event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* _event);
    void setColor(QColor _col);
    char getArrowShape();
    void setArrowShape(char _arrow_shape);

signals:
    void clicked();

private slots:
        void on_clicked();
        void remove_item();

public slots:
};

#endif // MOVEITEM_H
