#ifndef SVGREAD_H
#define SVGREAD_H

#include <QPen>
#include <QFile>
#include <QMessageBox>
#include <QDomDocument>
#include <QStringList>

#include "paintscene.h"

class SvgRead
{
public:
    SvgRead();
    static QList<QGraphicsRectItem*> getRectElements(const QString _filename);
    static QList<QGraphicsEllipseItem*> getCircleElements(const QString _filename);
    static QList<QGraphicsEllipseItem*> getOvalElements(const QString _filename);
    static QList<QGraphicsPathItem*> getDiamondElements(const QString _filename);
    static QList<QGraphicsPathItem*> getTriangleElements(const QString _filename);
    static bool isDiamond(const QString& _pathData);
    static bool isTriangle(const QString& _pathData);

    static QRectF getSizes(const QString _filename); // -
    static QColor color_svg;

};

#endif // SVGREAD_H

/*
 * Dependency packages:
 *         qt5-qtbase-devel
 *         add: sudo dnf install qt5-qtbase-devel
 */
