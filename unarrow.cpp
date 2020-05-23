#include "unarrow.h"
#include <QPainter>

UnArrow::UnArrow(QObject *parent):
    Arrow(parent)
{

}

void UnArrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    painter->setPen(Qt::black);
    painter->setBrush(brush);
    painter->drawRect(25, 3, length-50, 4);

    QPolygon polygon;
    polygon << QPoint(length-30,0) << QPoint(length-30,10) << QPoint(length-20,5);
    painter->drawPolygon(polygon);

    QPolygon polygon2;
    polygon2 << QPoint(30,0) << QPoint(30,10) << QPoint(20,5);
    painter->drawPolygon(polygon2);
    Q_UNUSED(option);
    Q_UNUSED(widget);
}

int UnArrow::type() const{
    return Type;
}
