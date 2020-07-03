#include "header/arrow.h"
#include <QPainter>
#include <QtMath>
#include <QGraphicsSceneMouseEvent>

Arrow::Arrow(QObject *parent):
    BaseArrow(parent)
{
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setPen(Qt::black);
    painter->setBrush(brush);
    painter->drawRect(20, 3, length-45, 4);

    QPolygon polygon;
    polygon << QPoint(length-30,0) << QPoint(length-30,10) << QPoint(length-20,5);
    painter->drawPolygon(polygon);
    Q_UNUSED(option)
    Q_UNUSED(widget)
}

QRectF Arrow::boundingRect() const{
    return QRectF(0, 0, length, height);
}
