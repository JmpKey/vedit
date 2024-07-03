#include "svgread.h"
QColor SvgRead::color_svg = QColor(Qt::white);

SvgRead::SvgRead() {}

QList<QGraphicsRectItem*> SvgRead::getRectElements(const QString _filename)
{
    QList<QGraphicsRectItem*> rectList;

    QDomDocument doc;
    QFile file(_filename);

    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return rectList;

    QDomNodeList gList = doc.elementsByTagName("g");

    for (int i = 0; i < gList.size(); i++) {
        QDomNode gNode = gList.item(i);
        QDomElement rectangle = gNode.firstChildElement("rect");

        if (rectangle.isNull()){
            continue;
        } else {
            QGraphicsRectItem* rect = new QGraphicsRectItem();

            rect->setFlag(QGraphicsItem::ItemIsMovable);

            QDomElement gElement = gNode.toElement();
            rect->setRect(rectangle.attribute("x").toInt(),
                          rectangle.attribute("y").toInt(),
                          rectangle.attribute("width").toInt(),
                          rectangle.attribute("height").toInt());

            QColor fillColor(gElement.attribute("fill", "#ffffff"));
            fillColor.setAlphaF(gElement.attribute("fill-opacity","0").toFloat());
            rect->setBrush(QBrush(fillColor));
            color_svg = fillColor;


            QColor strokeColor(gElement.attribute("stroke", "#000000"));
            strokeColor.setAlphaF(gElement.attribute("stroke-opacity").toFloat());

            rect->setPen(QPen(strokeColor,gElement.attribute("stroke-width", "0").toInt()));
            rectList.append(rect);
        }
    }
    file.close();
    return rectList;
}

QRectF SvgRead::getSizes(const QString _filename)
{
    QDomDocument doc;
    QFile file(_filename);

    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return QRectF(0,0,200,200);

    QDomNodeList list = doc.elementsByTagName("svg");
    if(list.length() > 0) {
        QDomElement svgElement = list.item(0).toElement();
        QStringList parameters = svgElement.attribute("viewBox").split(" ");
        return QRectF(parameters.at(0).toInt(),
                      parameters.at(1).toInt(),
                      parameters.at(2).toInt(),
                      parameters.at(3).toInt());
    }
    return QRectF(0, 0, 200, 200);
}

QList<QGraphicsEllipseItem*> SvgRead::getCircleElements(const QString _filename)
{
    QList<QGraphicsEllipseItem*> circleList;

    QDomDocument doc;
    QFile file(_filename);
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return circleList;

    QDomNodeList gList = doc.elementsByTagName("g");
    for (int i = 0; i < gList.size(); i++) {
        QDomNode gNode = gList.item(i);
        QDomElement circle = gNode.firstChildElement("circle");
        if (circle.isNull()) {
            continue;
        } else {
            QGraphicsEllipseItem* ellipse = new QGraphicsEllipseItem();
            ellipse->setFlag(QGraphicsItem::ItemIsMovable);
            QDomElement gElement = gNode.toElement();
            ellipse->setRect(circle.attribute("cx").toInt() - circle.attribute("r").toInt(),
                             circle.attribute("cy").toInt() - circle.attribute("r").toInt(),
                             circle.attribute("r").toInt() * 2,
                             circle.attribute("r").toInt() * 2);

            ellipse->setData(Qt::UserRole, i);

            QColor fillColor(gElement.attribute("fill", "#ffffff"));
            fillColor.setAlphaF(gElement.attribute("fill-opacity", "0").toFloat());
            ellipse->setBrush(QBrush(fillColor));
            color_svg = fillColor;

            QColor strokeColor(gElement.attribute("stroke", "#000000"));
            strokeColor.setAlphaF(gElement.attribute("stroke-opacity").toFloat());

            ellipse->setPen(QPen(strokeColor, gElement.attribute("stroke-width", "0").toInt()));
            circleList.append(ellipse);
        }
    }
    file.close();
    return circleList;
}

QList<QGraphicsEllipseItem*> SvgRead::getOvalElements(const QString _filename)
{
    QList<QGraphicsEllipseItem*> ellipseList;

    QDomDocument doc;
    QFile file(_filename);
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return ellipseList;

    QDomNodeList gList = doc.elementsByTagName("g");
    for (int i = 0; i < gList.size(); i++) {
        QDomNode gNode = gList.item(i);
        QDomElement ellipse = gNode.firstChildElement("ellipse");
        if (ellipse.isNull()) {
            continue;
        } else {
            QGraphicsEllipseItem* graphicsEllipse = new QGraphicsEllipseItem();
            graphicsEllipse->setFlag(QGraphicsItem::ItemIsMovable);
            QDomElement gElement = gNode.toElement();
            graphicsEllipse->setRect(ellipse.attribute("cx").toInt() - ellipse.attribute("rx").toInt(),
                                     ellipse.attribute("cy").toInt() - ellipse.attribute("ry").toInt(),
                                     ellipse.attribute("rx").toInt() * 2,
                                     ellipse.attribute("ry").toInt() * 2);

            QColor fillColor(gElement.attribute("fill", "#ffffff"));
            fillColor.setAlphaF(gElement.attribute("fill-opacity", "0").toFloat());
            graphicsEllipse->setBrush(QBrush(fillColor));
            color_svg = fillColor;

            QColor strokeColor(gElement.attribute("stroke", "#000000"));
            strokeColor.setAlphaF(gElement.attribute("stroke-opacity").toFloat());
            graphicsEllipse->setPen(QPen(strokeColor, gElement.attribute("stroke-width", "0").toInt()));

            ellipseList.append(graphicsEllipse);
        }
    }
    file.close();
    return ellipseList;
}


QList<QGraphicsPathItem*> SvgRead::getDiamondElements(const QString _filename)
{
    QList<QGraphicsPathItem*> diamondList;

    QDomDocument doc;
    QFile file(_filename);
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return diamondList;

    QDomNodeList gList = doc.elementsByTagName("g");
    for (int i = 0; i < gList.size(); i++) {
        QDomNode gNode = gList.item(i);
        QDomElement path = gNode.firstChildElement("path");
        if (path.isNull()) {
            continue;
        } else {
            QString pathData = path.attribute("d");

            if (isDiamond(pathData)) {
                QGraphicsPathItem* diamond = new QGraphicsPathItem();
                diamond->setFlag(QGraphicsItem::ItemIsMovable);

                QPainterPath diamondPath;
                diamondPath.moveTo(0, -30);
                diamondPath.lineTo(30, 0);
                diamondPath.lineTo(0, 30);
                diamondPath.lineTo(-30, 0);
                diamondPath.closeSubpath();

                diamond->setPath(diamondPath);

                QDomElement gElement = gNode.toElement();

                QColor fillColor(gElement.attribute("fill", "#ffffff"));
                fillColor.setAlphaF(gElement.attribute("fill-opacity", "0").toFloat());
                diamond->setBrush(QBrush(fillColor));
                color_svg = fillColor;

                QColor strokeColor(gElement.attribute("stroke", "#000000"));
                strokeColor.setAlphaF(gElement.attribute("stroke-opacity").toFloat());
                diamond->setPen(QPen(strokeColor));

                diamondList.append(diamond);
            }
        }
    }

    return diamondList;
}

bool SvgRead::isDiamond(const QString& _pathData) {
    return _pathData.contains("M0,-30 L30,0 L0,30 L-30,0");
}

QList<QGraphicsPathItem*> SvgRead::getTriangleElements(const QString _filename)
{
    QList<QGraphicsPathItem*> triangleList;

    QDomDocument doc;
    QFile file(_filename);
    if (!file.open(QIODevice::ReadOnly) || !doc.setContent(&file))
        return triangleList;

    QDomElement root = doc.documentElement();
    if (root.tagName() != "svg")
        return triangleList;

    QDomNodeList gList = doc.elementsByTagName("g");
    for (int i = 0; i < gList.size(); i++) {
        QDomNode gNode = gList.item(i);
        QDomElement path = gNode.firstChildElement("path");

        if (path.isNull()){
            continue;
        } else {
            QString pathData = path.attribute("d");

            if (isTriangle(pathData)) {
                QGraphicsPathItem* triangle = new QGraphicsPathItem();
                triangle->setFlag(QGraphicsItem::ItemIsMovable);

                QPainterPath trianglePath;
                trianglePath.moveTo(0, -30);
                trianglePath.lineTo(30, 30);
                trianglePath.lineTo(-30, 30);
                trianglePath.closeSubpath();

                triangle->setPath(trianglePath);

                QDomElement gElement = gNode.toElement();

                QColor fillColor(gElement.attribute("fill", "#ffffff"));
                fillColor.setAlphaF(gElement.attribute("fill-opacity", "1").toFloat());
                triangle->setBrush(QBrush(fillColor));
                color_svg = fillColor;

                QColor strokeColor(gElement.attribute("stroke", "#000000"));
                strokeColor.setAlphaF(gElement.attribute("stroke-opacity").toFloat());
                triangle->setPen(QPen(strokeColor));

                triangleList.append(triangle);
            }
        }
    }

    return triangleList;
}

bool SvgRead::isTriangle(const QString& _pathData) {
    return _pathData.contains("M0,-30 L30,30 L-30,30");
}



//void SvgRead::setColorSvg(const QColor& _col) { color_svg = _col; }
/*
MoveItem* SvgRead::createMoveItemTriangle(const QString filename)
{
    QList<QGraphicsPathItem *> triangles = getTriangleElements(filename);

    MoveItem* triangle_i = new MoveItem();
    triangle_i->arrow_shape = 't';

    triangle_i->setColor(SvgRead::color_svg);

    foreach (QGraphicsPathItem* triangle, triangles) {
        QPointF pos = triangle->pos();

        if (pos.x() < 0 && pos.y() < 0) {
            triangle_i->scaleDown();
        } else {
            triangle_i->scaleUp();
        }
    }

    return triangle_i;
}

MoveItem* SvgRead::createMoveItemRomb(const QString filename)
{
    QList<QGraphicsPathItem *> rombs = getDiamondElements(filename);
    MoveItem* romb_i = new MoveItem();
    romb_i->arrow_shape = 'p';
    romb_i->setColor(SvgRead::color_svg);

    foreach (QGraphicsPathItem* romb, rombs) {
       QPointF pos = romb->pos();
       if (pos.x() < 0 && pos.y() < 0) {
           romb_i->scaleDown();
       } else {
           romb_i->scaleUp();
       }
    }

    return romb_i;
}

MoveItem* SvgRead::createMoveItemOval(const QString filename)
{
    QList<QGraphicsEllipseItem *> ovals = getOvalElements(filename);
    MoveItem* oval_i = new MoveItem();
    oval_i->arrow_shape = 'E';
    oval_i->setColor(SvgRead::color_svg);

    foreach (QGraphicsEllipseItem* oval, ovals) {
       QPointF pos = oval->pos();
       if (pos.x() < 0 && pos.y() < 0) {
           oval_i->scaleDown();
       } else {
           oval_i->scaleUp();
       }
    }

    return oval_i;
}

MoveItem* SvgRead::createMoveItemCircle(const QString filename)
{
    QList<QGraphicsEllipseItem *> circles = getCircleElements(filename);
    MoveItem* circle_i = new MoveItem();
    circle_i->arrow_shape = 'e';
    circle_i->setColor(SvgRead::color_svg);

    foreach (QGraphicsEllipseItem* circle, circles) {
       QPointF pos = circle->pos();
       if (pos.x() < 0 && pos.y() < 0) {
           circle_i->scaleDown();
       } else {
           circle_i->scaleUp();
       }
    }

    return circle_i;
}

MoveItem* SvgRead::createMoveItemSquare(const QString filename)
{
    QList<QGraphicsRectItem *> squares = getRectElements(filename);
    MoveItem* square_i = new MoveItem();
    square_i->arrow_shape = 'r';
    //square_i->setColor(SvgRead::color_svg);

    foreach (QGraphicsRectItem* square, squares) {
       QPointF pos = square->pos();
       if (pos.x() < 0 && pos.y() < 0) {
           square_i->scaleDown();
       } else {
           square_i->scaleUp();
       }
       square_i->setColor(square->brush().color());
    }

    return square_i;
}

MoveItem* SvgRead::createMoveItemRectangle(const QString filename)
{
    QList<QGraphicsRectItem *> rects = getRectElements(filename);
    MoveItem* rect_i = new MoveItem();
    rect_i->arrow_shape = 'R';
    rect_i->setColor(SvgRead::color_svg);


    foreach (QGraphicsRectItem* rect, rects) {
       QPointF pos = rect->pos();
       if (pos.x() < 0 && pos.y() < 0) {
           rect_i->scaleDown();
       } else {
           rect_i->scaleUp();
       }
       //rect_i->setColor(rect->brush().color());
    }

    return rect_i;
}
*/
