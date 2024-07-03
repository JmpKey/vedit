#include "moveitem.h"

MoveItem::MoveItem(QObject* _parent) :
    QObject(_parent), QGraphicsItem()
{
    setFlag(QGraphicsItem::ItemIsMovable, true);
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsFocusable, true);

    connect(this, &MoveItem::clicked, this, &MoveItem::remove_item);
}

MoveItem::~MoveItem() {}

QRectF MoveItem::boundingRect() const
{
    return QRectF(-30, -30, 60, 60);
}

void MoveItem::paint(QPainter* _painter, const QStyleOptionGraphicsItem* _option, QWidget* _widget)
{
    _painter->setPen(Qt::black);
    _painter->setBrush(color);

    switch (arrow_shape) {
    case 'e':
        _painter->drawEllipse(-30, -30, 60, 60);
        break;
    case 'p':
        _painter->drawPolygon(QPolygon() << QPoint(0, -30) << QPoint(30, 0) << QPoint(0, 30) << QPoint(-30, 0));
        break;
    case 'r':
        _painter->drawRect(-30, -30, 60, 60);
        break;
    case 'R':
        _painter->drawRect(-30, -20, 60, 40);
        break;
    case 'E':
        _painter->drawEllipse(-30, -20, 60, 40);
        break;
    case 't':
        _painter->drawPolygon(QPolygon() << QPoint(0, -30) << QPoint(30, 30) << QPoint(-30, 30));
        break;
    }
}

void MoveItem::remove_item()
{
    scene()->removeItem(this);
    deleteLater();
}

void MoveItem::on_clicked() {
    emit clicked();
}

void MoveItem::scaleUp()
{
    scale *= 1.1;
    setTransform(QTransform::fromScale(scale, scale));
}

void MoveItem::scaleDown()
{
    scale /= 1.1;
    setTransform(QTransform::fromScale(scale, scale));
}

void MoveItem::rotate() {
    rotation += 90;
    QTransform transform = QTransform::fromScale(scale, scale);
    transform.rotate(rotation);
    setTransform(transform);
}

void MoveItem::stretchXP() {
    scaleX *= 1.1;
    setTransform(QTransform::fromScale(scaleX, scaleY));
}

void MoveItem::stretchYP() {
    scaleY *= 1.1;
    setTransform(QTransform::fromScale(scaleX, scaleY));
}

void MoveItem::stretchXM() {
    scaleX /= 1.1;
    setTransform(QTransform::fromScale(scaleX, scaleY));
}

void MoveItem::stretchYM() {
    scaleY /= 1.1;
    setTransform(QTransform::fromScale(scaleX, scaleY));
}

void MoveItem::keyPressEvent(QKeyEvent* _event)
{
    if(_event->key() == Qt::Key_Plus) { scaleUp(); }
    else if(_event->key() == Qt::Key_Minus) { scaleDown(); }
    else if(_event->key() == Qt::Key_C) {
        QColor newColor = QColorDialog::getColor();
        if (newColor.isValid()) {
            color = newColor;
            update();
        }
    }
    if (_event->key() == Qt::Key_R) { rotate(); }
    if (_event->key() == Qt::Key_X) { stretchXP(); }
    if (_event->key() == Qt::Key_Y) { stretchYP(); }
    if (_event->key() == Qt::Key_Z) { stretchXM(); }
    if (_event->key() == Qt::Key_T) { stretchYM(); }
}

void MoveItem::mousePressEvent(QGraphicsSceneMouseEvent* _event)
{
    if(_event->button() == Qt::RightButton){ on_clicked(); }
    QGraphicsItem::mousePressEvent(_event);
}

void MoveItem::mouseMoveEvent(QGraphicsSceneMouseEvent* _event)
{
    QGraphicsItem::mouseMoveEvent(_event);
}

void MoveItem::mouseReleaseEvent(QGraphicsSceneMouseEvent* _event)
{
    QGraphicsItem::mouseReleaseEvent(_event);
}

void MoveItem::setColor(QColor _col) { color = _col; }

char MoveItem::getArrowShape() { return arrow_shape; }

void MoveItem::setArrowShape(char _arrow_shape) { arrow_shape = _arrow_shape; }
